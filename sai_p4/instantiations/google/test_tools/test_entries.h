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

// Library of common table entries in PI and IR.
//
// These are suitable for use in switch testing and unit testing, e.g. with
// BMv2.

#ifndef PINS_SAI_P4_INSTANTIATIONS_GOOGLE_TEST_TOOLS_TEST_ENTRIES_H_
#define PINS_SAI_P4_INSTANTIATIONS_GOOGLE_TEST_TOOLS_TEST_ENTRIES_H_

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/base/attributes.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"
#include "p4/v1/p4runtime.pb.h"
#include "p4_pdpi/ir.pb.h"
#include "p4_pdpi/netaddr/ipv4_address.h"
#include "p4_pdpi/netaddr/ipv6_address.h"
#include "p4_pdpi/netaddr/mac_address.h"
#include "sai_p4/instantiations/google/sai_pd.pb.h"

namespace sai {

// Different ways of punting packets to the controller.
enum class PuntAction {
  // Punts copy of packet and prevents packet from being forwarded.
  kTrap,
  // Punts copy of packet without preventing packet from being forwarded.
  kCopy,
};

// Rewrite-related options for nexthop action generation.
struct NexthopRewriteOptions {
  bool disable_decrement_ttl = false;
  // When present, source MAC will be rewritten to the given address. When
  // absent, no rewrite occurs.
  std::optional<netaddr::MacAddress> src_mac_rewrite =
      netaddr::MacAddress(1, 2, 3, 4, 5, 6);
  // When present, destination MAC will be rewritten to the given address. When
  // absent, no rewrite occurs.
  std::optional<netaddr::MacAddress> dst_mac_rewrite =
      netaddr::MacAddress(2, 2, 2, 2, 2, 2);
  bool disable_vlan_rewrite = false;
};

enum class IpVersion {
  kIpv4,
  kIpv6,
  kIpv4And6,
};

template <typename Sink>
void AbslStringify(Sink& sink, const IpVersion& ip_version) {
  switch (ip_version) {
    case IpVersion::kIpv4:
      absl::Format(&sink, "IPv4");
      break;
    case IpVersion::kIpv6:
      absl::Format(&sink, "IPv6");
      break;
    case IpVersion::kIpv4And6:
      absl::Format(&sink, "IPv4And6");
      break;
  }
}

// Parameters for generating a mirror session table entry with IPFIX
// encapsulation.
struct MirrorSessionParams {
  std::string mirror_session_id;
  std::string monitor_port;
  std::string mirror_encap_src_mac;
  std::string mirror_encap_dst_mac;
  std::string mirror_encap_vlan_id;
  std::string mirror_encap_src_ip;
  std::string mirror_encap_dst_ip;
  std::string mirror_encap_udp_src_port;
  std::string mirror_encap_udp_dst_port;
};

// Parameters for generating an ACL table entry to mark packets to be mirrored.
struct MarkToMirrorParams {
  std::string ingress_port;
  std::string mirror_session_id;
};

// Convenience struct corresponding to the protos `p4::v1::Replica` and
// `sai::ReplicateAction::Replica`.
struct Replica {
  std::string egress_port;
  int instance = 0;
};

// Match fields of an ingress mirror or redirect table entry.
struct MirrorAndRedirectMatchFields {
  std::optional<absl::string_view> in_port;
  std::optional<bool> ipmc_table_hit;
};

// Provides methods to conveniently build a set of SAI-P4 table entries for
// testing.
//
// Example usage:
// ```
//   ASSERT_OK_AND_ASSIGN(pdpi::IrEntities ir_entities,
//     EntryBuilder(ir_p4info)
//       .AddEntriesForwardingIpPacketsToGivenPort("egress_port")
//       .AddEntryPuntingAllPackets(PuntAction::kCopy)
//       .GetDedupedIrEntities());
// ```
class EntryBuilder {
 public:
  EntryBuilder() = default;
  explicit EntryBuilder(sai::TableEntries entries)
      : entries_(std::move(entries)) {}

  // Logs the current PD entries in the EntryBuilder to LOG(INFO).
  const EntryBuilder& LogPdEntries() const;
  EntryBuilder& LogPdEntries();

  absl::StatusOr<std::vector<p4::v1::Entity>> GetDedupedPiEntities(
      const pdpi::IrP4Info& ir_p4info, bool allow_unsupported = false) const;
  absl::StatusOr<pdpi::IrEntities> GetDedupedIrEntities(
      const pdpi::IrP4Info& ir_p4info, bool allow_unsupported = false) const;

  // Convenience struct corresponding to the proto
  // `MulticastRouterInterfaceTableEntry`
  // in `sai_pd.proto`.
  struct MulticastRouterInterfaceTableEntry {
    std::string multicast_replica_port;
    int multicast_replica_instance = 0;
    netaddr::MacAddress src_mac;
  };

  EntryBuilder& AddEntryPuntingAllPackets(PuntAction action);
  // Note: Cannot be combined with other entries that forward *all* IP packets
  // in a specific way.
  EntryBuilder& AddEntriesForwardingIpPacketsToGivenPort(
      absl::string_view egress_port,
      IpVersion ip_version = IpVersion::kIpv4And6, NexthopRewriteOptions = {});
  // Note: Cannot be combined with other entries that forward *all* IP packets
  // in a specific way.
  EntryBuilder& AddEntriesForwardingIpPacketsToGivenMulticastGroup(
      int multicast_group_id);
  EntryBuilder& AddVrfEntry(absl::string_view vrf);
  EntryBuilder& AddEntryAdmittingAllPacketsToL3();
  EntryBuilder& AddDefaultRouteForwardingAllPacketsToGivenPort(
      absl::string_view egress_port, IpVersion ip_version,
      absl::string_view vrf,
      const NexthopRewriteOptions& nexthop_rewrite_options = {},
      std::optional<absl::string_view> vlan_hexstr = std::nullopt);
  EntryBuilder& AddMulticastRoute(absl::string_view vrf,
                                  const netaddr::Ipv4Address& dst_ip,
                                  int multicast_group_id);
  EntryBuilder& AddMulticastRoute(absl::string_view vrf,
                                  const netaddr::Ipv6Address& dst_ip,
                                  int multicast_group_id);
  EntryBuilder& AddPreIngressAclEntryAssigningVrfForGivenIpType(
      absl::string_view vrf, IpVersion ip_version);
  EntryBuilder& AddEntryDecappingAllIpInIpv6PacketsAndSettingVrf(
      absl::string_view vrf);
  EntryBuilder& AddEntryPuntingPacketsWithTtlZeroAndOne();
  EntryBuilder& AddMulticastGroupEntry(int multicast_group_id,
                                         absl::Span<const Replica> replicas);
  EntryBuilder& AddMulticastGroupEntry(
      int multicast_group_id, absl::Span<const std::string> egress_ports);
  EntryBuilder& AddMulticastRouterInterfaceEntry(
      const MulticastRouterInterfaceTableEntry& entry);
  EntryBuilder& AddIngressAclDroppingAllPackets();
  EntryBuilder& AddDisableVlanChecksEntry();
  EntryBuilder& AddEntrySettingVrfBasedOnVlanId(
      absl::string_view vlan_id_hexstr, absl::string_view vrf);
  EntryBuilder& AddEntrySettingVrfForAllPackets(absl::string_view vrf);
  EntryBuilder& AddEntrySettingVlanIdInPreIngress(
      absl::string_view set_vlan_id_hexstr,
      std::optional<absl::string_view> match_vlan_id_hexstr = std::nullopt);
  EntryBuilder& AddNexthopRifNeighborEntries(
      absl::string_view nexthop_id, absl::string_view egress_port,
      const NexthopRewriteOptions& nexthop_rewrite_options = {},
      std::optional<absl::string_view> vlan_hexstr = std::nullopt);
  EntryBuilder& AddIngressAclEntryRedirectingToNexthop(
      absl::string_view nexthop_id,
      std::optional<absl::string_view> in_port_match = std::nullopt);
  EntryBuilder& AddIngressAclEntryRedirectingToMulticastGroup(
      int multicast_group_id,
      const MirrorAndRedirectMatchFields& match_fields = {});
  EntryBuilder& AddMirrorSessionTableEntry(const MirrorSessionParams& params);
  EntryBuilder& AddMarkToMirrorAclEntry(const MarkToMirrorParams& params);

 private:
  sai::TableEntries entries_;
};

}  // namespace sai

#endif  // PINS_SAI_P4_INSTANTIATIONS_GOOGLE_TEST_TOOLS_TEST_ENTRIES_H_
