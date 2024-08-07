// Copyright 2024 Google LLC
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

#include "sai_p4/instantiations/google/test_tools/test_entries.h"

#include <algorithm>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gutil/proto_matchers.h"
#include "gutil/status_matchers.h"
#include "gutil/testing.h"
#include "p4/v1/p4runtime.pb.h"
#include "p4_pdpi/ir.pb.h"
#include "p4_pdpi/pd.h"
#include "sai_p4/instantiations/google/instantiations.h"
#include "sai_p4/instantiations/google/sai_p4info.h"
#include "sai_p4/instantiations/google/sai_pd.pb.h"

namespace sai {
namespace {

using ::gutil::EqualsProto;
using ::gutil::IsOkAndHolds;
using ::testing::_;
using ::testing::IsEmpty;
using ::testing::Pointwise;
using ::testing::SizeIs;

using TestEntriesTest = ::testing::TestWithParam<sai::Instantiation>;

TEST_P(TestEntriesTest, MakePiEntryPuntingAllPacketsDoesNotError) {
  ASSERT_THAT(MakePiEntryPuntingAllPackets(PuntAction::kCopy,
                                           sai::GetIrP4Info(GetParam())),
              IsOkAndHolds(_));
  ASSERT_THAT(MakePiEntryPuntingAllPackets(PuntAction::kTrap,
                                           sai::GetIrP4Info(GetParam())),
              IsOkAndHolds(_));
}
TEST_P(TestEntriesTest, MakeIrEntryPuntingAllPacketsDoesNotError) {
  ASSERT_THAT(MakeIrEntryPuntingAllPackets(PuntAction::kCopy,
                                           sai::GetIrP4Info(GetParam())),
              IsOkAndHolds(_));
  ASSERT_THAT(MakeIrEntryPuntingAllPackets(PuntAction::kTrap,
                                           sai::GetIrP4Info(GetParam())),
              IsOkAndHolds(_));
}
TEST_P(TestEntriesTest, MakePdEntryPuntingAllPacketsDoesNotError) {
  ASSERT_THAT(MakePdEntryPuntingAllPackets(PuntAction::kCopy), IsOkAndHolds(_));
  ASSERT_THAT(MakePdEntryPuntingAllPackets(PuntAction::kTrap), IsOkAndHolds(_));
}

TEST_P(TestEntriesTest,
       MakePiEntriesForwardingIpPacketsToGivenPortDoesNotError) {
  ASSERT_THAT(MakePiEntriesForwardingIpPacketsToGivenPort(
                  /*egress_port=*/"42", sai::GetIrP4Info(GetParam())),
              IsOkAndHolds(_));
}
TEST_P(TestEntriesTest,
       MakeIrEntriesForwardingIpPacketsToGivenPortDoesNotError) {
  ASSERT_THAT(MakeIrEntriesForwardingIpPacketsToGivenPort(
                  /*egress_port=*/"42", sai::GetIrP4Info(GetParam())),
              IsOkAndHolds(_));
}
TEST_P(TestEntriesTest,
       MakePdEntriesForwardingIpPacketsToGivenPortDoesNotError) {
  ASSERT_THAT(MakePdEntriesForwardingIpPacketsToGivenPort(/*egress_port=*/"42"),
              IsOkAndHolds(_));
}

INSTANTIATE_TEST_SUITE_P(, TestEntriesTest,
                         testing::ValuesIn(sai::AllSaiInstantiations()),
                         [](const auto& info) -> std::string {
                           return sai::InstantiationToString(info.param);
                         });

// -- EntryBuilder tests --------------------------------------------------

TEST(EntryBuilder,
     GetDedupedIrEntitiesReturnsNothingForDefaultConstructedBuilder) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  ASSERT_OK_AND_ASSIGN(
      pdpi::IrEntities entities,
      EntryBuilder().LogPdEntries().GetDedupedIrEntities(kIrP4Info));
  EXPECT_THAT(entities.entities(), IsEmpty());
}

TEST(EntryBuilder, GetDedupedEntitiesReturnsEntriesPassedToConstructor) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  auto entries = gutil::ParseProtoOrDie<sai::TableEntries>(
      R"pb(
        entries {
          l3_admit_table_entry {
            action { admit_to_l3 {} }
            priority: 1
          }
        }
      )pb");

  // Construct IrEntities.
  ASSERT_OK_AND_ASSIGN(pdpi::IrEntities ir_entities,
                       pdpi::PdTableEntriesToIrEntities(kIrP4Info, entries));
  EXPECT_THAT(
      EntryBuilder(entries).LogPdEntries().GetDedupedIrEntities(kIrP4Info),
      IsOkAndHolds(EqualsProto(ir_entities)));

  // Construct Pi Entities.
  ASSERT_OK_AND_ASSIGN(std::vector<p4::v1::Entity> pi_entities,
                       pdpi::PdTableEntriesToPiEntities(kIrP4Info, entries));
  EXPECT_THAT(
      EntryBuilder(entries).LogPdEntries().GetDedupedPiEntities(kIrP4Info),
      IsOkAndHolds(Pointwise(EqualsProto(), pi_entities)));
}

TEST(EntryBuilder, GetDedupedEntitiesRemovesDuplicates) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  auto entries = gutil::ParseProtoOrDie<sai::TableEntries>(
      R"pb(
        entries {
          l3_admit_table_entry {
            action { admit_to_l3 {} }
            priority: 1
          }
        }
        entries {
          l3_admit_table_entry {
            action { admit_to_l3 {} }
            priority: 1
          }
        }
      )pb");

  // Construct deduped IrEntities.
  auto deduped_entries = gutil::ParseProtoOrDie<sai::TableEntries>(
      R"pb(
        entries {
          l3_admit_table_entry {
            action { admit_to_l3 {} }
            priority: 1
          }
        }
      )pb");
  ASSERT_OK_AND_ASSIGN(
      pdpi::IrEntities deduped_entities,
      pdpi::PdTableEntriesToIrEntities(kIrP4Info, deduped_entries));
  EXPECT_THAT(
      EntryBuilder(entries).LogPdEntries().GetDedupedIrEntities(kIrP4Info),
      IsOkAndHolds(EqualsProto(deduped_entities)));

  // Construct Deduped Pi Entities.
  ASSERT_OK_AND_ASSIGN(
      std::vector<p4::v1::Entity> deduped_pi_entities,
      pdpi::PdTableEntriesToPiEntities(kIrP4Info, deduped_entries));
  EXPECT_THAT(
      EntryBuilder(entries).LogPdEntries().GetDedupedPiEntities(kIrP4Info),
      IsOkAndHolds(Pointwise(EqualsProto(), deduped_pi_entities)));
}

TEST(EntryBuilder, AddEntryPuntingAllPacketsDoesNotAddEntry) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  ASSERT_OK_AND_ASSIGN(pdpi::IrEntities entities,
                       EntryBuilder()
                           .AddEntryPuntingAllPackets(PuntAction::kCopy)
                           .AddEntryPuntingAllPackets(PuntAction::kTrap)
                           .LogPdEntries()
                           .GetDedupedIrEntities(kIrP4Info));
  EXPECT_THAT(entities.entities(), SizeIs(2));
}

TEST(EntryBuilder, AddEntriesForwardingIpPacketsToGivenPortAddsEntries) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  ASSERT_OK_AND_ASSIGN(
      pdpi::IrEntities entities,
      EntryBuilder()
          .AddEntriesForwardingIpPacketsToGivenPort("egress port")
          .LogPdEntries()
          .GetDedupedIrEntities(kIrP4Info));
  EXPECT_THAT(entities.entities(), SizeIs(8));
}

TEST(EntryBuilder, AddVrfEntryAddsEntry) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  ASSERT_OK_AND_ASSIGN(pdpi::IrEntities entities,
                       EntryBuilder()
                           .AddVrfEntry("vrf-1")
                           .AddVrfEntry("vrf-2")
                           .AddVrfEntry("vrf-3")
                           .LogPdEntries()
                           .GetDedupedIrEntities(kIrP4Info));
  EXPECT_THAT(entities.entities(), SizeIs(3));
}

TEST(EntryBuilder, AddEntryAdmittingAllPacketsToL3AddsEntry) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  ASSERT_OK_AND_ASSIGN(pdpi::IrEntities entities,
                       EntryBuilder()
                           .AddEntryAdmittingAllPacketsToL3()
                           .LogPdEntries()
                           .GetDedupedIrEntities(kIrP4Info));
  EXPECT_THAT(entities.entities(), SizeIs(1));
}

TEST(EntryBuilder, AddDefaultRouteForwardingAllPacketsToGivenPortAddsEntries) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  ASSERT_OK_AND_ASSIGN(pdpi::IrEntities entities,
                       EntryBuilder()
                           .AddDefaultRouteForwardingAllPacketsToGivenPort(
                               "egress port 1", IpVersion::kIpv4, "vrf-1")
                           .AddDefaultRouteForwardingAllPacketsToGivenPort(
                               "egress port 2", IpVersion::kIpv6, "vrf-2")
                           .AddDefaultRouteForwardingAllPacketsToGivenPort(
                               "egress port 3", IpVersion::kIpv4And6, "vrf-3")
                           .LogPdEntries()
                           .GetDedupedIrEntities(kIrP4Info));
  EXPECT_THAT(entities.entities(), SizeIs(13));
}

TEST(EntryBuilder, AddPreIngressAclEntryAssigningVrfForGivenIpTypeAddsEntry) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  ASSERT_OK_AND_ASSIGN(pdpi::IrEntities entities,
                       EntryBuilder()
                           .AddPreIngressAclEntryAssigningVrfForGivenIpType(
                               "vrf-1", IpVersion::kIpv4)
                           .AddPreIngressAclEntryAssigningVrfForGivenIpType(
                               "vrf-1", IpVersion::kIpv6)
                           .AddPreIngressAclEntryAssigningVrfForGivenIpType(
                               "vrf-1", IpVersion::kIpv4And6)
                           .LogPdEntries()
                           .GetDedupedIrEntities(kIrP4Info));
  EXPECT_THAT(entities.entities(), SizeIs(3));
}

TEST(EntryBuilder, AddEntryDecappingAllIpInIpv6PacketsAndSettingVrfAddsEntry) {
  pdpi::IrP4Info kIrP4Info = GetIrP4Info(Instantiation::kFabricBorderRouter);
  ASSERT_OK_AND_ASSIGN(
      pdpi::IrEntities entities,
      EntryBuilder()
          .AddEntryDecappingAllIpInIpv6PacketsAndSettingVrf("vrf-1")
          .AddEntryDecappingAllIpInIpv6PacketsAndSettingVrf("vrf-2")
          .AddEntryDecappingAllIpInIpv6PacketsAndSettingVrf("vrf-3")
          .LogPdEntries()
          .GetDedupedIrEntities(kIrP4Info, /*allow_unsupported=*/true));
  EXPECT_THAT(entities.entities(), SizeIs(3));
}

}  // namespace
}  // namespace sai
