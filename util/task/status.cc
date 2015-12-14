// Copyright 2013 Google Inc. All Rights Reserved.
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

#include "util/task/status.h"

namespace util {

namespace {

const Status &GetOk() {
  static const Status status;
  return status;
}

const Status &GetCancelled() {
  static const Status status(::util::error::CANCELLED, "");
  return status;
}

const Status &GetUnknown() {
  static const Status status(::util::error::UNKNOWN, "");
  return status;
}

}  // namespace

Status::Status() : code_(::util::error::OK), message_("") {}

Status::Status(::util::error::Code error, const char *error_message)
    : code_(error), message_(error_message) {
  if (code_ == ::util::error::OK) {
    message_ = nullptr;
  }
}

Status::Status(const Status& other)
    : code_(other.code_), message_(other.message_) {}

Status& Status::operator=(const Status& other) {
  code_ = other.code_;
  message_ = other.message_;
  return *this;
}

const Status &Status::OK = GetOk();
const Status &Status::CANCELLED = GetCancelled();
const Status &Status::UNKNOWN = GetUnknown();

const char *Status::ToString() const {
  if (code_ == ::util::error::OK) {
    return "OK";
  }

  return message_;
}

}  // namespace util
