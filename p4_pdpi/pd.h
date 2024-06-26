// Copyright 2020 Google LLC
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

#ifndef PINS_P4_PDPI_PD_H_
#define PINS_P4_PDPI_PD_H_

#include <string>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/types/span.h"
#include "google/protobuf/message.h"
#include "gutil/status.h"
#include "p4/config/v1/p4info.pb.h"
#include "p4/v1/p4runtime.pb.h"
#include "p4_pdpi/ir.h"
#include "p4_pdpi/ir.pb.h"
#include "p4_pdpi/translation_options.h"

namespace pdpi {

// This file contains functions that translate from and to PD.
// Since the exact form of PD is not known until run time, we need to pass in
// a generic google::protobuf::Message and use GetReflection() to access the PD
// proto.

// The kinds of entities that can be declared in P4, e.g. tables and actions.
enum P4EntityKind {
  kP4Table,
  kP4Action,
  kP4Parameter,
  kP4MatchField,
  kP4MetaField,
};

// Given a P4 name for a given entity kind, returns the name of the
// corresponding protobuf message name.
absl::StatusOr<std::string> P4NameToProtobufMessageName(
    absl::string_view p4_name, P4EntityKind entity_kind);

// Given a P4 name for a given entity kind, returns the name of the
// corresponding protobuf field name.
absl::StatusOr<std::string> P4NameToProtobufFieldName(absl::string_view p4_name,
                                                      P4EntityKind entity_kind);

// Given a PD table entry, returns a PD table entry with only the key part set.
absl::Status PdTableEntryToOnlyKeyPd(const IrP4Info &info,
                                     const google::protobuf::Message &pd,
                                     google::protobuf::Message *key_only_pd);

// -- Conversions from PI to PD ------------------------------------------------

absl::Status PiTableEntryToPd(const IrP4Info &info,
                              const p4::v1::TableEntry &pi,
                              google::protobuf::Message *pd,
                              TranslationOptions options = {});

// Like `PiTableEntryToPd`, but for a sequence of `pi` entries.
// Assumes that `pd` has a `repeated TableEntry entries` field.
absl::Status PiTableEntriesToPd(const IrP4Info &info,
                                const absl::Span<const p4::v1::TableEntry> &pi,
                                google::protobuf::Message *pd,
                                TranslationOptions options = {});

absl::Status PiPacketInToPd(const IrP4Info &info,
                            const p4::v1::PacketIn &pi_packet,
                            google::protobuf::Message *pd_packet);

absl::Status PiPacketOutToPd(const IrP4Info &info,
                             const p4::v1::PacketOut &pi_packet,
                             google::protobuf::Message *pd_packet);

absl::Status PiReadRequestToPd(const IrP4Info &info,
                               const p4::v1::ReadRequest &pi,
                               google::protobuf::Message *pd);

absl::Status PiReadResponseToPd(const IrP4Info &info,
                                const p4::v1::ReadResponse &pi,
                                google::protobuf::Message *pd,
                                TranslationOptions options = {});

absl::Status PiUpdateToPd(const IrP4Info &info, const p4::v1::Update &pi,
                          google::protobuf::Message *pd,
                          TranslationOptions options = {});

absl::Status PiWriteRequestToPd(const IrP4Info &info,
                                const p4::v1::WriteRequest &pi,
                                google::protobuf::Message *pd,
                                TranslationOptions options = {});

absl::Status PiStreamMessageRequestToPd(const IrP4Info &info,
                                        const p4::v1::StreamMessageRequest &pi,
                                        google::protobuf::Message *pd);

absl::Status PiStreamMessageResponseToPd(
    const IrP4Info &info, const p4::v1::StreamMessageResponse &pi,
    google::protobuf::Message *pd);

// -- Conversions from PD to PI ------------------------------------------------

// Translates a PdTableEntry into an IrTableEntry. 'Partial' functions do not
// support 'MulticastGroupTableEntry', returning an InvalidArgumentError if one
// is provided. Use of this function is discouraged since P4 infrastructure has
// moved towards units of Entity instead of TableEntry.
//
// TODO: Remove deprecated functions from header file.
ABSL_DEPRECATED("Use PdTableEntryToPiEntity instead")
absl::StatusOr<p4::v1::TableEntry> PartialPdTableEntryToPiTableEntry(
    const IrP4Info &info, const google::protobuf::Message &pd,
    TranslationOptions options = {});
ABSL_DEPRECATED("Use PdTableEntriesToPiEntities instead")
absl::StatusOr<std::vector<p4::v1::TableEntry>>
PartialPdTableEntriesToPiTableEntries(const IrP4Info &info,
                                      const google::protobuf::Message &pd,
                                      TranslationOptions options = {});

absl::StatusOr<p4::v1::PacketIn> PdPacketInToPi(
    const IrP4Info &info, const google::protobuf::Message &packet);

absl::StatusOr<p4::v1::PacketOut> PdPacketOutToPi(
    const IrP4Info &info, const google::protobuf::Message &packet);

absl::StatusOr<p4::v1::ReadRequest> PdReadRequestToPi(
    const IrP4Info &info, const google::protobuf::Message &pd);

absl::StatusOr<p4::v1::ReadResponse> PdReadResponseToPi(
    const IrP4Info &info, const google::protobuf::Message &pd,
    TranslationOptions options = {});

absl::StatusOr<p4::v1::Update> PdUpdateToPi(const IrP4Info &info,
                                            const google::protobuf::Message &pd,
                                            TranslationOptions options = {});

absl::StatusOr<p4::v1::WriteRequest> PdWriteRequestToPi(
    const IrP4Info &info, const google::protobuf::Message &pd,
    TranslationOptions options = {});

absl::StatusOr<p4::v1::StreamMessageRequest> PdStreamMessageRequestToPi(
    const IrP4Info &info,
    const google::protobuf::Message &stream_message_request);

absl::StatusOr<p4::v1::StreamMessageResponse> PdStreamMessageResponseToPi(
    const IrP4Info &info,
    const google::protobuf::Message &stream_message_response);

// -- Conversions to and from grpc::Status -------------------------------------

absl::Status GrpcStatusToPd(const grpc::Status &status,
                            int number_of_updates_in_write_request,
                            google::protobuf::Message *pd);

absl::StatusOr<grpc::Status> PdWriteRpcStatusToGrpcStatus(
    const google::protobuf::Message &pd);

// -- Conversions from IR (intermediate representation) to PD ------------------

absl::Status IrEntityToPdTableEntry(const IrP4Info &info, const IrEntity &ir,
                                    google::protobuf::Message *pd,
                                    TranslationOptions options = {});

absl::Status IrTableEntryToPd(const IrP4Info &ir_p4info, const IrTableEntry &ir,
                              google::protobuf::Message *pd,
                              TranslationOptions options = {});
absl::Status IrTableEntriesToPd(const IrP4Info &ir_p4info,
                                const IrTableEntries &ir,
                                google::protobuf::Message *pd,
                                TranslationOptions options = {});
absl::Status IrTableEntriesToPd(const IrP4Info &ir_p4info,
                                absl::Span<const IrTableEntry> ir,
                                google::protobuf::Message *pd,
                                TranslationOptions options = {});

absl::Status IrPacketInToPd(const IrP4Info &info, const IrPacketIn &packet,
                            google::protobuf::Message *pd_packet);

absl::Status IrPacketOutToPd(const IrP4Info &info, const IrPacketOut &packet,
                             google::protobuf::Message *pd_packet);

absl::Status IrReadRequestToPd(const IrP4Info &info, const IrReadRequest &ir,
                               google::protobuf::Message *pd);

absl::Status IrReadResponseToPd(const IrP4Info &info, const IrReadResponse &ir,
                                google::protobuf::Message *read_response,
                                TranslationOptions options = {});

absl::Status IrUpdateToPd(const IrP4Info &info, const IrUpdate &ir,
                          google::protobuf::Message *update,
                          TranslationOptions options = {});

absl::Status IrWriteRequestToPd(const IrP4Info &info, const IrWriteRequest &ir,
                                google::protobuf::Message *write_reques,
                                TranslationOptions options = {});

absl::Status IrStreamMessageRequestToPd(
    const IrP4Info &info, const IrStreamMessageRequest &ir,
    google::protobuf::Message *stream_message);

absl::Status IrStreamMessageResponseToPd(
    const IrP4Info &info, const IrStreamMessageResponse &ir,
    google::protobuf::Message *stream_message);

absl::Status IrRpcResponseToPd(const IrWriteResponse &ir_rpc_response,
                               google::protobuf::Message *pd_rpc_response);

absl::Status IrWriteRpcStatusToPd(const IrWriteRpcStatus &ir_write_status,
                                  google::protobuf::Message *pd);

// -- Conversions from PD to IR (intermediate representation) ------------------

// Translates a PdTableEntry into a PI TableEntry. 'Partial' functions do not
// support 'MulticastGroupTableEntry', returning an InvalidArgumentError if one
// is provided. Use of this function is discouraged since P4 infrastructure has
// moved towards units of Entity instead of TableEntry.
//
// TODO: Remove deprecated functions from header file.
ABSL_DEPRECATED("Use PdTableEntryToIrEntity instead")
absl::StatusOr<IrTableEntry> PartialPdTableEntryToIrTableEntry(
    const IrP4Info &ir_p4info, const google::protobuf::Message &pd,
    TranslationOptions options = {});
ABSL_DEPRECATED("Use PdTableEntriesToIrEntities instead")
absl::StatusOr<IrTableEntries> PartialPdTableEntriesToIrTableEntries(
    const IrP4Info &ir_p4info, const google::protobuf::Message &pd,
    TranslationOptions options = {});

absl::StatusOr<IrPacketIn> PdPacketInToIr(
    const IrP4Info &info, const google::protobuf::Message &packet);

absl::StatusOr<IrPacketOut> PdPacketOutToIr(
    const IrP4Info &info, const google::protobuf::Message &packet);

absl::StatusOr<IrReadRequest> PdReadRequestToIr(
    const IrP4Info &info, const google::protobuf::Message &read_request);

absl::StatusOr<IrReadResponse> PdReadResponseToIr(
    const IrP4Info &info, const google::protobuf::Message &read_response,
    TranslationOptions options = {});

absl::StatusOr<IrUpdate> PdUpdateToIr(const IrP4Info &info,
                                      const google::protobuf::Message &update,
                                      TranslationOptions options = {});

absl::StatusOr<IrWriteRequest> PdWriteRequestToIr(
    const IrP4Info &info, const google::protobuf::Message &write_request,
    TranslationOptions options = {});

absl::StatusOr<IrStreamMessageRequest> PdStreamMessageRequestToIr(
    const IrP4Info &info, const google::protobuf::Message &stream_message);

absl::StatusOr<IrStreamMessageResponse> PdStreamMessageResponseToIr(
    const IrP4Info &info, const google::protobuf::Message &stream_message);

absl::StatusOr<IrWriteRpcStatus> PdWriteRpcStatusToIr(
    const google::protobuf::Message &pd);

// -- PD getters/setters -------------------------------------------------------

// Get Enum field in proto message's field with `field_name`.
absl::StatusOr<int> GetEnumField(const google::protobuf::Message &message,
                                 const std::string &field_name);

// Set Enum field in proto message's field with `field_name` to value with
// tag number == `enum_value`.
absl::Status SetEnumField(google::protobuf::Message *message,
                          const std::string &enum_field_name, int enum_value);

}  // namespace pdpi

#endif  // PINS_P4_PDPI_PD_H_
