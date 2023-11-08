// Copyright (c) 2024, Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "lib/validator/pins_backend.h"

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "grpcpp/impl/codegen/client_context.h"
#include "grpcpp/impl/codegen/status_code_enum.h"
#include "gutil/status.h"
#include "lib/gnmi/gnmi_helper.h"
#include "lib/validator/validator_backend.h"
#include "p4_pdpi/p4_runtime_session.h"
#include "proto/gnmi/gnmi.pb.h"

namespace pins_test {
PINSBackend::PINSBackend(std::vector<std::unique_ptr<thinkit::Switch>> switches)
    : ValidatorBackend({}), switches_map_() {
  devices_.reserve(switches.size());
  switches_map_.reserve(switches.size());
  for (auto it = switches.begin(); it != switches.end(); it++) {
    std::string chassis_name((*it)->ChassisName());
    switches_map_.insert({chassis_name, std::move(*it)});
    devices_.insert(std::move(chassis_name));
  }
}

absl::Status PINSBackend::CanEstablishP4RuntimeSession(
    absl::string_view chassis, absl::Duration timeout) {
  // TODO: Remove kDeviceId once device ID is set through gNMI in
  // P4RT app.
  static constexpr uint64_t kDeviceId = 183807201;
  auto sut = switches_map_.find(chassis);
  if (sut == switches_map_.end()) {
    return absl::InternalError(
        absl::StrCat("ValidatorBackend passed invalid chassis: ", chassis));
  }
  auto& [sut_name, sut_switch] = *sut;
  ASSIGN_OR_RETURN(auto p4runtime_stub, sut_switch->CreateP4RuntimeStub());
  return pdpi::P4RuntimeSession::Create(std::move(p4runtime_stub), kDeviceId)
      .status();

  return absl::OkStatus();
}

absl::Status PINSBackend::CanGetAllInterfaceOverGnmi(absl::string_view chassis,
                                                     absl::Duration timeout) {
  auto sut = switches_map_.find(chassis);
  if (sut == switches_map_.end()) {
    return absl::InternalError(
        absl::StrCat("ValidatorBackend passed invalid chassis: ", chassis));
  }
  auto& [sut_name, sut_switch] = *sut;
  ASSIGN_OR_RETURN(auto gnmi_stub, sut_switch->CreateGnmiStub());
  ASSIGN_OR_RETURN(auto req, BuildGnmiGetRequest("", gnmi::GetRequest::ALL));

  gnmi::GetResponse resp;
  grpc::ClientContext context;
  context.set_deadline(absl::ToChronoTime(absl::Now() + timeout));
  context.set_wait_for_ready(true);
  return gutil::GrpcStatusToAbslStatus(gnmi_stub->Get(&context, req, &resp));
}

}  // namespace pins_test