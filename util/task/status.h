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

#ifndef UTIL_TASK_STATUS_H__
#define UTIL_TASK_STATUS_H__

namespace util {

namespace error {
    // These values must match error codes defined in codes.proto.
    enum Code {
        OK = 0,
        CANCELLED = 1,
        UNKNOWN = 2,
        INVALID_ARGUMENT = 3,
        DEADLINE_EXCEEDED = 4,
        NOT_FOUND = 5,
        ALREADY_EXISTS = 6,
        PERMISSION_DENIED = 7,
        UNAUTHENTICATED = 16,
        RESOURCE_EXHAUSTED = 8,
        FAILED_PRECONDITION = 9,
        ABORTED = 10,
        OUT_OF_RANGE = 11,
        UNIMPLEMENTED = 12,
        INTERNAL = 13,
        UNAVAILABLE = 14,
        DATA_LOSS = 15,
    };
}  // namespace error

// A Status is a combination of an error code and a string message (for non-OK
// error codes).
class Status {
 public:
  // Creates an OK status
  Status();

  // Make a Status from the specified error and message.
  // The error message must have static lifetime.
  Status(::util::error::Code error, const char *error_message);

  Status(const Status &other);
  Status &operator=(const Status &other);

  // Some pre-defined Status objects
  static const Status& OK;             // Identical to 0-arg constructor
  static const Status& CANCELLED;
  static const Status& UNKNOWN;

  // Accessors
  bool ok() const { return code_ == ::util::error::OK; }
  int error_code() const {return code_; }
  ::util::error::Code CanonicalCode() const { return code_; }
  const char *error_message() const { return message_; }

  bool operator==(const Status& x) const;
  bool operator!=(const Status& x) const;

  // NoOp
  void IgnoreError() const {}

  const char *ToString() const;

 private:
  ::util::error::Code code_;
  const char *message_;
};

inline bool Status::operator==(const Status &other) const {
  return (this->code_ == other.code_) && (this->message_ == other.message_);
}

inline bool Status::operator!=(const Status &other) const {
  return !(*this == other);
}

}  // namespace util

#endif  // UTIL_TASK_STATUS_H__
