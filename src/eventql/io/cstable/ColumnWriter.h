/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License ("the license") as
 * published by the Free Software Foundation, either version 3 of the License,
 * or any later version.
 *
 * In accordance with Section 7(e) of the license, the licensing of the Program
 * under the license does not imply a trademark license. Therefore any rights,
 * title and interest in our trademarks remain entirely with us.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you develop
 * commercial activities involving this program without disclosing the source
 * code of your own applications
 */
#pragma once
#include <eventql/util/stdtypes.h>
#include <eventql/util/exception.h>
#include <eventql/util/autoref.h>
#include <eventql/util/protobuf/MessageObject.h>
#include <eventql/io/cstable/cstable.h>
#include <eventql/io/cstable/page_manager.h>
#include <eventql/io/cstable/columns/page_writer_uint64.h>

namespace cstable {

class ColumnWriter : public RefCounted {
public:

  ColumnWriter(size_t r_max, size_t d_max);

  virtual void writeNull(uint64_t rlvl, uint64_t dlvl) = 0;

  virtual void writeBoolean(
      uint64_t rlvl,
      uint64_t dlvl,
      bool value) = 0;

  virtual void writeUnsignedInt(
      uint64_t rlvl,
      uint64_t dlvl,
      uint64_t value) = 0;

  virtual void writeSignedInt(
      uint64_t rlvl,
      uint64_t dlvl,
      int64_t value) = 0;

  virtual void writeFloat(
      uint64_t rlvl,
      uint64_t dlvl,
      double value) = 0;

  void writeString(
      uint64_t rlvl,
      uint64_t dlvl,
      const String& value);

  virtual void writeString(
      uint64_t rlvl,
      uint64_t dlvl,
      const char* data,
      size_t size) = 0;

  virtual void writeDateTime(
      uint64_t rlvl,
      uint64_t dlvl,
      UnixTime value);

  virtual ColumnType type() const = 0;
  virtual ColumnEncoding encoding() const = 0;

  virtual void flush() = 0;

  size_t maxRepetitionLevel() const;
  size_t maxDefinitionLevel() const;

protected:
  size_t r_max_;
  size_t d_max_;
};

class DefaultColumnWriter : public ColumnWriter {
public:

  DefaultColumnWriter(
      ColumnConfig config,
      PageManager* page_mgr);

  void writeNull(uint64_t rlvl, uint64_t dlvl) override;

  ColumnEncoding encoding() const override {
    return config_.storage_type;
  }

  ColumnType type() const override {
    return config_.logical_type;
  }

  void flush() override {
    if (rlevel_writer_.get()) rlevel_writer_->flush();
    if (dlevel_writer_.get()) dlevel_writer_->flush();
  }

protected:
  ColumnConfig config_;
  ScopedPtr<UnsignedIntPageWriter> rlevel_writer_;
  ScopedPtr<UnsignedIntPageWriter> dlevel_writer_;
};

} // namespace cstable


