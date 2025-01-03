#!/bin/bash
# Copyright 2021 Google LLC
#
# Please read carefully before adding new dependencies:
#
# System dependencies are disallowed by default, and the bar for exceptions is
# high.
#
# pins-infra strives for a hermetic build, i.e. one that is insensitive to the
# libraries and other software installed on your machine, other than Bazel and
# the compilers. This ensure the build is reproducible and portable.
#
# Before adding a new system dependency, consider the following:
#
# 1. Please read the note on dependencies in pins_infra_deps.bzl.
#
# 2. Can the dependency be avoided altogether? Consider that there is a
#    non-trival cost to maintaining dependencies over time.
#
# 3. Can the dependency be built with Bazel instead?
#   - For many libraries, there are existing Bazel BUILD files. Try a quick
#     Google search.
#   - If there is no existing BUILD file, can you write your own BUILD file?
#     See the bazel/ folder for examples. Ideally, we strive to upstream such
#     BUILD files so everyone can benefit and share the maintenance burden.
#   - If it's too hard to write a native BUILD file, try writing a BUILD file
#     using rules_foreign_cc (https://github.com/bazelbuild/rules_foreign_cc).
#.    See the bazel/ folder for examples.

# Please read carefully before adding new dependencies:
#
# System dependencies are disallowed by default, and the bar for exceptions is
# high.
#
# pins-infra strives for a hermetic build, i.e. one that is insensitive to the
# libraries and other software installed on your machine, other than Bazel and
# the compilers. This ensure the build is reproducible and portable.
#
# Before adding a new system dependency, consider the following:
#
# 1. Please read the note on dependencies in pins_infra_deps.bzl.
#
# 2. Can the dependency be avoided altogether? Consider that there is a
#    non-trival cost to maintaining dependencies over time.
#
# 3. Can the dependency be built with Bazel instead?
#   - For many libraries, there are existing Bazel BUILD files. Try a quick
#     Google search.
#   - If there is no existing BUILD file, can you write your own BUILD file?
#     See the bazel/ folder for examples. Ideally, we strive to upstream such
#     BUILD files so everyone can benefit and share the maintenance burden.
#   - If it's too hard to write a native BUILD file, try writing a BUILD file
#     using rules_foreign_cc (https://github.com/bazelbuild/rules_foreign_cc).
#.    See the bazel/ folder for examples.

# TODO: Avoid system dependencies like libnl, currently these are
# coming because Sonic swss common depends on them.
sudo apt-get update
sudo apt-get install \
  bison \
  flex \
  libfl-dev \
  libgmp-dev \
  libhiredis-dev \
  libnl-3-dev \
  libnl-genl-3-dev \
  libnl-route-3-dev \
  libnl-nf-3-dev \
  libboost-dev \
  libboost-serialization-dev \
  libyang-dev \
  libzmq3-dev \
  uuid-dev \
  nlohmann-json3-dev
