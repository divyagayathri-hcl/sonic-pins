// Copyright 2025 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PINS_TESTS_FORWARDING_ACL_FEATURE_TEST_H_
#define PINS_TESTS_FORWARDING_ACL_FEATURE_TEST_H_

#include <memory>
#include <optional>
#include <string>

#include "dvaas/dataplane_validation.h"
#include "dvaas/test_vector.pb.h"
#include "gtest/gtest.h"
#include "p4/config/v1/p4info.pb.h"
#include "sai_p4/instantiations/google/test_tools/test_entries.h"
#include "thinkit/mirror_testbed_fixture.h"

namespace pins_test {

struct AclFeatureTestParams {
  // Using a shared_ptr because parameterized tests require objects to be
  // copyable.
  std::shared_ptr<thinkit::MirrorTestbedInterface> mirror_testbed;
  // Pushed to the SUT if given, otherwise assumes the correct one is already
  // configured.
  std::optional<p4::config::v1::P4Info> sut_p4info;
  std::string test_name;
  // ACL action variant to test out different behavior
  std::optional<sai::PuntAction> punt_action;
  dvaas::DataplaneValidationParams dvaas_params;
  std::shared_ptr<dvaas::DataplaneValidator> dvaas;
};

class AclFeatureTestFixture
    : public testing::TestWithParam<AclFeatureTestParams> {
 public:
  void SetUp() override { GetParam().mirror_testbed->SetUp(); }

  void TearDown() override { GetParam().mirror_testbed->TearDown(); }
};

}  // namespace pins_test

#endif  // PINS_TESTS_FORWARDING_ACL_FEATURE_TEST_H_
