// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "p4_fuzzer/switch_state.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iterator>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "absl/strings/substitute.h"
#include "glog/logging.h"
#include "google/protobuf/util/message_differencer.h"
#include "gutil/collections.h"
#include "gutil/proto.h"
#include "gutil/status.h"
#include "p4/config/v1/p4info.pb.h"
#include "p4/v1/p4runtime.pb.h"
#include "p4_pdpi/entity_keys.h"
#include "p4_pdpi/internal/ordered_map.h"
#include "p4_pdpi/ir.h"
#include "p4_pdpi/ir.pb.h"

namespace p4_fuzzer {

using ::gutil::FindOrDie;
using ::gutil::FindPtrOrStatus;
using ::gutil::PrintTextProto;
using ::p4::v1::TableEntry;
using ::p4::v1::Update;
using ::pdpi::IrP4Info;
using ::pdpi::IrTableEntry;


absl::StatusOr<TableEntry> CanonicalizeTableEntry(const IrP4Info& info,
                                                  const TableEntry& entry,
                                                  bool key_only) {
  auto pdpi_options = pdpi::TranslationOptions{
      .key_only = key_only,
  };
  ASSIGN_OR_RETURN(IrTableEntry ir_entry,
                   pdpi::PiTableEntryToIr(info, entry, pdpi_options),
                   _ << "Could not canonicalize: PiToIr Error\n"
                     << entry.DebugString());
  ASSIGN_OR_RETURN(TableEntry canonical_entry,
                   IrTableEntryToPi(info, ir_entry, pdpi_options),
                   _ << "Could not canonicalize: IrToPi Error\n"
                     << entry.DebugString());
  return canonical_entry;
}

SwitchState::SwitchState(IrP4Info ir_p4info)
    : ir_p4info_(std::move(ir_p4info)) {
  for (auto& [table_id, table] : ir_p4info_.tables_by_id()) {
    ordered_tables_[table_id] = OrderedTableEntries();
    unordered_tables_[table_id] = UnorderedTableEntries();
  }
}

void SwitchState::ClearTableEntries() {
  *this = SwitchState(std::move(ir_p4info_));
}

bool SwitchState::AllTablesEmpty() const {
  for (auto table_id : AllTableIds()) {
    if (!IsTableEmpty(table_id)) {
      return false;
    }
  }

  return true;
}

bool SwitchState::IsTableFull(const uint32_t table_id) const {
  return !CanAccommodateInserts(table_id, 1);
}

int64_t SwitchState::GetNumTableEntries(const uint32_t table_id) const {
  return FindOrDie(ordered_tables_, table_id).size();
}

int64_t SwitchState::GetNumTableEntries() const {
  int result = 0;
  for (const auto& [key, table] : ordered_tables_) {
    result += table.size();
  }
  return result;
}

const std::vector<uint32_t> SwitchState::AllTableIds() const {
  std::vector<uint32_t> table_ids;
  for (auto& [key, table] : ir_p4info_.tables_by_id()) {
    table_ids.push_back(key);
  }
  // absl::c_sort(table_ids);

  return table_ids;
}

bool SwitchState::CanAccommodateInserts(const uint32_t table_id,
                                        const int n) const {
  return (FindOrDie(ir_p4info_.tables_by_id(), table_id).size() -
          GetNumTableEntries(table_id)) >= n;
}

bool SwitchState::IsTableEmpty(const uint32_t table_id) const {
  return FindOrDie(ordered_tables_, table_id).empty();
}

std::optional<TableEntry> SwitchState::GetTableEntry(
    const TableEntry& entry) const {
  const UnorderedTableEntries& table =
      FindOrDie(unordered_tables_, entry.table_id());

  if (auto table_iter = table.find(pdpi::TableEntryKey(entry));
      table_iter != table.end()) {
    auto [table_key, table_entry] = *table_iter;
    return table_entry;
  }

  // If no entry found, canonicalize the entry and try again.
  absl::StatusOr<TableEntry> canonical_entry =
      CanonicalizeTableEntry(ir_p4info_, entry, /*key_only=*/true);
  if (!canonical_entry.ok()) return std::nullopt;

  if (auto table_iter = table.find(pdpi::TableEntryKey(*canonical_entry));
      table_iter != table.end()) {
    auto [table_key, table_entry] = *table_iter;
    return table_entry;
  }

  return absl::nullopt;
}

absl::Status SwitchState::ApplyUpdate(const Update& update) {
  const int table_id = update.entity().table_entry().table_id();

  auto& ordered_table = FindOrDie(ordered_tables_, table_id);
  auto& unordered_table = FindOrDie(unordered_tables_, table_id);

  const TableEntry& table_entry = update.entity().table_entry();
  // TODO: PDPI IR Update translation currently does not properly
  // ignore non-key fields on DELETE updates. Therefore, information to ignore
  // non-key fields is explitcitly passed for canonicalization.
  ASSIGN_OR_RETURN(
      const TableEntry& canonical_table_entry,
      CanonicalizeTableEntry(ir_p4info_, table_entry,
                             /*key_only=*/update.type() == Update::DELETE));

  switch (update.type()) {
    case Update::INSERT: {
      auto [ordered_iter, ordered_not_present] = ordered_table.insert(
          /*value=*/{pdpi::TableEntryKey(canonical_table_entry),
                     canonical_table_entry});

      auto [unordered_iter,
            unordered_not_present] = unordered_table.insert(/*value=*/{
          pdpi::TableEntryKey(canonical_table_entry), canonical_table_entry});

      if (ordered_not_present != unordered_not_present) {
        return gutil::InternalErrorBuilder()
               << "Ordered Table and Unordered Table out of sync. Entry "
               << (ordered_not_present ? "not present" : "present")
               << " in Ordered Table but "
               << (unordered_not_present ? "not present" : "present")
               << " in Unordered Table.\n"
               << "Offending Entry Update\n"
               << update.DebugString();
      }

      if (!ordered_not_present) {
        return gutil::InvalidArgumentErrorBuilder()
               << "Cannot install the same table entry multiple times. Update: "
               << update.DebugString();
      }

      break;
    }

    case Update::DELETE: {
      int ordered_entries_erased = ordered_tables_[table_id].erase(
          pdpi::TableEntryKey(canonical_table_entry));
      int unordered_entries_erased = unordered_tables_[table_id].erase(
          pdpi::TableEntryKey(canonical_table_entry));

      if (ordered_entries_erased != unordered_entries_erased) {
        return gutil::InternalErrorBuilder()
               << "Ordered Table and Unordered Table out of sync. Entry "
               << (ordered_entries_erased == 0 ? "not present" : "present")
               << " in Ordered Table but "
               << (unordered_entries_erased == 0 ? "not present" : "present")
               << " in Unordered Table.\n"
               << "Offending Update\n"
               << update.DebugString();
      }

      if (ordered_entries_erased != 1) {
        return gutil::InvalidArgumentErrorBuilder()
               << "Cannot erase non-existent table entries. Update: "
               << update.DebugString();
      }

      break;
    }

    case Update::MODIFY: {
      auto [ordered_iter, ordered_not_present] = ordered_table.insert_or_assign(
          /*k=*/pdpi::TableEntryKey(canonical_table_entry),
          /*obj=*/canonical_table_entry);

      auto [unordered_iter, unordered_not_present] =
          unordered_table.insert_or_assign(/*k=*/
                                           pdpi::TableEntryKey(
                                               canonical_table_entry),
                                           /*obj=*/canonical_table_entry);

      if (ordered_not_present != unordered_not_present) {
        return gutil::InternalErrorBuilder()
               << "Ordered Table and Unordered Table out of sync. Entry "
               << (ordered_not_present ? "not present" : "present")
               << " in Ordered Table but "
               << (unordered_not_present ? "not present" : "present")
               << " in Unordered Table.\n"
               << "Offending Update\n"
               << update.DebugString();
      }

      if (ordered_not_present) {
        return gutil::InvalidArgumentErrorBuilder()
               << "Cannot modify a non-existing update. Update: "
               << update.DebugString();
      }

      break;
    }

    default:
      LOG(FATAL) << "Update of unsupported type: " << update.DebugString();
  }
  return absl::OkStatus();
}

absl::Status SwitchState::SetTableEntries(
    absl::Span<const p4::v1::TableEntry> table_entries) {
  ClearTableEntries();

  for (const p4::v1::TableEntry& entry : table_entries) {
    auto ordered_table = ordered_tables_.find(entry.table_id());
    auto unordered_table = unordered_tables_.find(entry.table_id());
    bool ordered_present = ordered_table != ordered_tables_.end();
    bool unordered_present = unordered_table != unordered_tables_.end();
    if (ordered_present != unordered_present) {
      return gutil::InternalErrorBuilder() << absl::StrFormat(
                 "ordered and unordered tables out of sync. Table with ID "
                 "'%d' %s in ordered tables but '%s' in unordered tables",
                 entry.table_id(),
                 (ordered_present ? "present" : "not present"),
                 (unordered_present ? "present" : "not present"));
    }
    if (!ordered_present) {
      return gutil::InvalidArgumentErrorBuilder()
             << "table entry with unknown table ID '" << entry.table_id()
             << "'";
    }
    ASSIGN_OR_RETURN(
        TableEntry canonical_entry,
        CanonicalizeTableEntry(ir_p4info_, entry, /*key_only=*/false));
    ordered_table->second.insert(
        {pdpi::TableEntryKey(canonical_entry), canonical_entry});
    unordered_table->second.insert(
        {pdpi::TableEntryKey(canonical_entry), canonical_entry});
  }

  return absl::OkStatus();
}

std::string SwitchState::SwitchStateSummary() const {
  if (ordered_tables_.empty()) return std::string("EmptyState()");
  std::string res = "";
  // Ordered is used to get a deterministic order for the summary.
  for (const auto& [table_id, table] : Ordered(ordered_tables_)) {
    const pdpi::IrTableDefinition& table_def =
        FindOrDie(ir_p4info_.tables_by_id(), table_id);
    const std::string& table_name = table_def.preamble().alias();
    int max_size = table_def.size();
    int current_size = table.size();

    absl::StrAppendFormat(&res, "\n % 12d% 18d    %s", current_size, max_size,
                          table_name);

    // Mark tables where we have exceeded their resource limits.
    if (current_size > max_size) {
      absl::StrAppend(&res, "*");
    }

    // If the table is a WCMP table, then we also print its total weight,
    // max weight per group, total members, and max members per group. Only WCMP
    // tables using one-shot programming are supported.
    if (table_def.implementation_id_case() ==
        pdpi::IrTableDefinition::kActionProfileId) {
      int total_weight = 0;
      int max_weight_per_group = 0;
      int total_members = 0;
      int max_members_per_group = 0;
      for (auto& [_, table_entry] : table) {
        int this_entry_weight = 0;
        int this_entry_members = table_entry.action()
                                     .action_profile_action_set()
                                     .action_profile_actions_size();
        for (const p4::v1::ActionProfileAction& action :
             table_entry.action()
                 .action_profile_action_set()
                 .action_profile_actions()) {
          this_entry_weight += action.weight();
        }
        total_weight += this_entry_weight;
        max_weight_per_group =
            std::max(max_weight_per_group, this_entry_weight);
        total_members += this_entry_members;
        max_members_per_group =
            std::max(max_members_per_group, this_entry_members);
      }

      const p4::config::v1::ActionProfile& action_profile =
          FindOrDie(ir_p4info_.action_profiles_by_id(),
                    table_def.action_profile_id())
              .action_profile();

      bool uses_weight_semantics = !action_profile.has_sum_of_members();
      absl::StrAppendFormat(
          &res, "\n % 12d% 18d    %s.total_weight", total_weight,
          uses_weight_semantics ? action_profile.size() : 0, table_name);
      // Mark if we have exceeded the total weight and use weight semantics.
      if (uses_weight_semantics && total_weight > action_profile.size()) {
        absl::StrAppend(&res, "*");
      }
      absl::StrAppendFormat(
          &res, "\n % 12d% 18d    %s.max_group_weight", max_weight_per_group,
          uses_weight_semantics ? action_profile.max_group_size() : 0,
          table_name);
      // Mark if we have exceeded the max weight for a group and use weight
      // semantics.
      if (uses_weight_semantics &&
          max_weight_per_group > action_profile.max_group_size()) {
        absl::StrAppend(&res, "*");
      }

      bool uses_member_semantics = action_profile.has_sum_of_members();
      absl::StrAppendFormat(
          &res, "\n % 12d% 18d    %s.total_members", total_members,
          uses_member_semantics ? action_profile.size() : 0, table_name);
      // Mark if we have exceeded the total members and use member semantics.
      if (uses_member_semantics && total_members > action_profile.size()) {
        absl::StrAppend(&res, "*");
      }
      absl::StrAppendFormat(
          &res, "\n % 12d% 18d    %s.max_members_per_group",
          max_members_per_group,
          uses_member_semantics ? action_profile.max_group_size() : 0,
          table_name);
      // Mark if we have exceeded the max members for a group and use member
      // semantics.
      if (uses_member_semantics &&
          max_members_per_group > action_profile.max_group_size()) {
        absl::StrAppend(&res, "*");
      }
    }
  }
  return absl::StrFormat(
      "State(\n % 12s% 18s    table_name\n % 12d% 18s    total number of table "
      "entries%s\n * marks tables where current size > guaranteed size.\n)",
      "current size", "guaranteed size", GetNumTableEntries(), "N/A", res);
}

absl::StatusOr<std::vector<ReferableEntry>> SwitchState::GetReferableEntries(
    absl::string_view table,
    const absl::flat_hash_set<std::string>& fields) const {
  std::vector<ReferableEntry> result;

  ASSIGN_OR_RETURN(const pdpi::IrTableDefinition* table_definition,
                   FindPtrOrStatus(ir_p4info_.tables_by_name(), table),
                   _ << "Table '" << table << "'does not exist in p4info.");

  if (fields.empty()) {
    return gutil::InvalidArgumentErrorBuilder()
           << "Cannot get referable entries if no fields are being referenced.";
  }

  // PI representation uses fields ids, so map ids back to names.
  absl::flat_hash_map<uint32_t, std::string> field_id_to_field_name;
  for (const std::string& field : fields) {
    ASSIGN_OR_RETURN(
        const pdpi::IrMatchFieldDefinition* field_definition,
        FindPtrOrStatus(table_definition->match_fields_by_name(), field),
        _ << "Table '" << table << "' has no field named '" << field << "'.");
    p4::config::v1::MatchField match_field = field_definition->match_field();
    // References must only be to fields of type exact or optional.
    if (match_field.match_type() != p4::config::v1::MatchField::EXACT &&
        match_field.match_type() != p4::config::v1::MatchField::OPTIONAL) {
      return gutil::InvalidArgumentErrorBuilder()
             << "References must only be to fields with type exact or "
                "optional. Field '"
             << field << "' in table '" << table << "' is of a different type.";
    }
    field_id_to_field_name.insert({match_field.id(), field});
  }

  // Loop over all table entries to construct ReferableEntries.
  ASSIGN_OR_RETURN(
      const OrderedTableEntries* ordered_entries,
      FindPtrOrStatus(ordered_tables_, table_definition->preamble().id()),
      _ << "Table '" << table << "' exists in p4 info but has no ordered "
        << "entry in switch state.");
  for (const auto& [key, table_entry] : *ordered_entries) {
    ReferableEntry result_entry;
    // Fill out ReferableEntry mapping.
    for (const auto& match : table_entry.match()) {
      if (auto it = field_id_to_field_name.find(match.field_id());
          it != field_id_to_field_name.end()) {
        std::string field_name = it->second;
        if (match.has_exact()) {
          result_entry.insert({field_name, match.exact().value()});
        } else if (match.has_optional()) {
          result_entry.insert({field_name, match.optional().value()});
        }
      }
    }
    // Only include entries where all referenced fields are present.
    if (result_entry.size() == fields.size()) {
      result.push_back(result_entry);
    }
  }

  return result;
}

absl::StatusOr<std::vector<ReferableEntry>> SwitchState::GetReferableEntries(
    absl::string_view table,
    const absl::flat_hash_set<std::string>& fields) const {
  std::vector<ReferableEntry> result;

  ASSIGN_OR_RETURN(const pdpi::IrTableDefinition& table_definition,
                   FindOrStatus(ir_p4info_.tables_by_name(), table));

  if (fields.empty()) {
    return gutil::InvalidArgumentErrorBuilder()
           << "Cannot get referable entries if no fields are being referenced.";
  }

  // PI representation uses fields ids, so map ids back to names.
  absl::flat_hash_map<uint32_t, std::string> field_id_to_field_name;
  for (const std::string& field : fields) {
    ASSIGN_OR_RETURN(
        const pdpi::IrMatchFieldDefinition& field_definition,
        FindOrStatus(table_definition.match_fields_by_name(), field));
    p4::config::v1::MatchField match_field = field_definition.match_field();
    // References must only be to fields of type exact or optional.
    if (match_field.match_type() != p4::config::v1::MatchField::EXACT &&
        match_field.match_type() != p4::config::v1::MatchField::OPTIONAL) {
      return gutil::InvalidArgumentErrorBuilder()
             << "References must only be to fields with type exact or "
                "optional. Field '"
             << field << "' in table '" << table << "' is of a different type.";
    }
    field_id_to_field_name.insert({match_field.id(), field});
  }

  // Loop over all table entries to construct ReferableEntries.
  ASSIGN_OR_RETURN(
      OrderedTableEntries ordered_entries,
      FindOrStatus(ordered_tables_, table_definition.preamble().id()));
  for (const auto& [key, table_entry] : ordered_entries) {
    ReferableEntry result_entry;
    // Fill out ReferableEntry mapping.
    for (const auto& match : table_entry.match()) {
      if (auto it = field_id_to_field_name.find(match.field_id());
          it != field_id_to_field_name.end()) {
        std::string field_name = it->second;
        if (match.has_exact()) {
          result_entry.insert({field_name, match.exact().value()});
        } else if (match.has_optional()) {
          result_entry.insert({field_name, match.optional().value()});
        }
      }
    }
    // Only include entries where all referenced fields are present.
    if (result_entry.size() == fields.size()) {
      result.push_back(result_entry);
    }
  }

  return result;
}

absl::Status SwitchState::CheckConsistency() const {
  if (ordered_tables_.size() != unordered_tables_.size()) {
    return absl::InternalError(absl::StrFormat(
        "Size of `ordered_tables_` and `unordered_tables_` is "
        "different. `ordered_tables_`: '%d'  `unordered_tables_`: '%d'",
        ordered_tables_.size(), unordered_tables_.size()));
  }

  // Ensure that every `table_id` in `ordered_tables_` is also present in
  // `unordered_tables_` and that the corresponding tables are the same size.
  for (const auto& [table_id, ordered_table] : ordered_tables_) {
    if (!unordered_tables_.contains(table_id)) {
      return absl::InternalError(absl::StrFormat(
          "`unordered_tables_` is missing table with id '%d'", table_id));
    }

    const UnorderedTableEntries& unordered_table =
        unordered_tables_.at(table_id);

    if (unordered_table.size() != ordered_table.size()) {
      return absl::InternalError(absl::StrFormat(
          "Number of ordered entries differs from number of unordered entries "
          "in table with id %d. Ordered Entries: %d  Unordered Entries: %d",
          table_id, ordered_table.size(),
          unordered_tables_.at(table_id).size()));
    }

    // Ensure that every `table_entry` in an ordered table has a corresponding
    // `table_entry` in the unordered table.
    for (const auto& [table_key, ordered_table_entry] : ordered_table) {
      std::optional<TableEntry> unordered_table_entry =
          GetTableEntry(ordered_table_entry);
      if (!unordered_table_entry.has_value()) {
        return absl::InternalError(
            absl::StrFormat("Ordered table entry %s is missing corresponding "
                            "unordered table entry",
                            ordered_table_entry.DebugString()));
      }

      google::protobuf::util::MessageDifferencer differ;
      differ.TreatAsSet(TableEntry::descriptor()->FindFieldByName("match"));
      if (!gutil::ProtoEqual(ordered_table_entry, *unordered_table_entry,
                             differ)) {
        return absl::InternalError(
            absl::StrFormat("Ordered entry differs from unordered entry\n "
                            "Ordered entry: %s Unordered Entry: %s",
                            ordered_table_entry.DebugString(),
                            unordered_table_entry->DebugString()));
      }
    }
  }
  return absl::OkStatus();
}

absl::Status SwitchState::AssertEntriesAreEqualToState(
    const std::vector<p4::v1::TableEntry>& switch_entries,
    std::optional<std::function<bool(const IrTableEntry&, const IrTableEntry&)>>
        TreatAsEqualDueToKnownBug) const {
  std::string status_message = "";

  // Condition that requires a search for unique entries in switchstate.
  bool entry_unique_to_switch = false;

  if (switch_entries.size() != GetNumTableEntries()) {
    absl::StrAppendFormat(&status_message,
                          "Number of entries on switch does not match number "
                          "of entries in Fuzzer\n"
                          "Entries on switch: %d\n"
                          "Entries in Fuzzer: %d\n",
                          switch_entries.size(), GetNumTableEntries());
  }

  // Message differencer for PI Table Entries.
  google::protobuf::util::MessageDifferencer differ;
  differ.TreatAsSet(
      p4::v1::TableEntry::GetDescriptor()->FindFieldByName("match"));
  differ.TreatAsSet(p4::v1::Action::GetDescriptor()->FindFieldByName("params"));
  differ.TreatAsSet(
      p4::v1::ActionProfileActionSet::GetDescriptor()->FindFieldByName(
          "action_profile_actions"));

  // Message differencer for IR Table Entries.
  google::protobuf::util::MessageDifferencer ir_differ;
  ir_differ.TreatAsSet(
      IrTableEntry::GetDescriptor()->FindFieldByName("matches"));
  ir_differ.TreatAsSet(
      pdpi::IrActionInvocation::GetDescriptor()->FindFieldByName("params"));
  ir_differ.TreatAsSet(
      pdpi::IrActionSet::GetDescriptor()->FindFieldByName("actions"));

  for (const auto& switch_entry : switch_entries) {
    ASSIGN_OR_RETURN(
        TableEntry canonical_switch_entry,
        CanonicalizeTableEntry(ir_p4info_, switch_entry, /*key_only=*/false));

    std::optional<p4::v1::TableEntry> fuzzer_entry =
        GetTableEntry(canonical_switch_entry);

    // Is there an entry on the switch that does not exist in the Fuzzer?
    if (!fuzzer_entry.has_value()) {
      entry_unique_to_switch = true;
      ASSIGN_OR_RETURN(
          IrTableEntry ir_entry,
          pdpi::PiTableEntryToIr(ir_p4info_, canonical_switch_entry));
      absl::StrAppend(
          &status_message,
          "The following entry exists on switch but not in Fuzzer:\n",
          PrintTextProto(ir_entry));
      continue;
    }

    std::string differences = "";
    differ.ReportDifferencesToString(&differences);
    std::string ir_differences = "";
    ir_differ.ReportDifferencesToString(&ir_differences);

    // Is there an entry with the same key on both the switch and in the Fuzzer,
    // but they differ in other ways?
    if (!differ.Compare(*fuzzer_entry, canonical_switch_entry)) {
      ASSIGN_OR_RETURN(
          IrTableEntry ir_switch_entry,
          pdpi::PiTableEntryToIr(ir_p4info_, canonical_switch_entry));
      ASSIGN_OR_RETURN(IrTableEntry ir_fuzzer_entry,
                       pdpi::PiTableEntryToIr(ir_p4info_, *fuzzer_entry));

      if (ir_differ.Compare(ir_fuzzer_entry, ir_switch_entry)) {
        return absl::InternalError(absl::StrFormat(
            "PI 'entries' were not equal but IR 'entries' were\n"
            "IR Entry\n"
            "%s"
            "Differences in PI 'entries'\n"
            "%s\n"
            "Switch PI Entry\n"
            "%s"
            "Fuzzer PI Entry\n"
            "%s",
            PrintTextProto(ir_switch_entry), differences,
            PrintTextProto(canonical_switch_entry),
            PrintTextProto(*fuzzer_entry)));

        // If there is a difference, are known bugs being masked and is the
        // difference caused by a known bug?
      } else if (!TreatAsEqualDueToKnownBug.has_value() ||
                 !(*TreatAsEqualDueToKnownBug)(ir_fuzzer_entry,
                                               ir_switch_entry)) {
        absl::StrAppendFormat(
            &status_message,
            "Entry exists in both switch and Fuzzer, but is different:\n"
            "%s\n"
            "Entry on switch:\n"
            "%s"
            "Entry in Fuzzer:\n"
            "%s",
            ir_differences, PrintTextProto(ir_switch_entry),
            PrintTextProto(ir_fuzzer_entry));
      }
    }
  }

  // Are there entries in the Fuzzer that do not exist on the switch?
  if (switch_entries.size() != GetNumTableEntries() || entry_unique_to_switch) {
    absl::flat_hash_map<int, UnorderedTableEntries> fuzzer_state_copy =
        unordered_tables_;

    // Removes all entries from the `fuzzer_state_copy` that exist on the
    // switch.
    for (const auto& switch_entry : switch_entries) {
      if (GetTableEntry(switch_entry).has_value()) {
        fuzzer_state_copy.at(switch_entry.table_id())
            .erase(pdpi::TableEntryKey(switch_entry));
      }
    }

    // All remaining entries exist only in the fuzzer.
    for (const auto& [table_id, table] : fuzzer_state_copy) {
      for (const auto& [key, fuzzer_entry] : table) {
        ASSIGN_OR_RETURN(IrTableEntry ir_entry,
                         pdpi::PiTableEntryToIr(ir_p4info_, fuzzer_entry));
        absl::StrAppend(
            &status_message,
            "The following entry exists in Fuzzer but not on switch:\n",
            PrintTextProto(ir_entry));
      }
    }
  }

  if (status_message.empty()) return absl::OkStatus();

  return absl::FailedPreconditionError(status_message);
}

}  // namespace p4_fuzzer
