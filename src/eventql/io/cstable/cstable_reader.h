/**
 * Copyright (c) 2016 zScale Technology GmbH <legal@zscale.io>
 * Authors:
 *   - Paul Asmuth <paul@zscale.io>
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
#include <eventql/util/io/file.h>
#include <eventql/io/cstable/ColumnReader.h>
#include <eventql/io/cstable/cstable.h>
#include <eventql/io/cstable/page_manager.h>
#include <eventql/io/cstable/cstable_file.h>
#include <eventql/io/cstable/cstable_writer.h>

namespace cstable {

class CSTableReader : public RefCounted {
public:

  static RefPtr<CSTableReader> openFile(const String& filename);

  static RefPtr<CSTableReader> openFile(
      const CSTableFile* arena,
      uint64_t limit = -1);

  ~CSTableReader();

  bool hasColumn(const String& column_name) const ;

  RefPtr<ColumnReader> getColumnReader(
      const String& column_name,
      ColumnReader::Visibility visibility = ColumnReader::Visibility::SHARED);

  ColumnType getColumnType(const String& column_name);
  ColumnEncoding getColumnEncoding(const String& column_name);

  const Vector<ColumnConfig>& columns() const;

  size_t numRecords() const;

  void copyTo(
      RefPtr<CSTableWriter> target_table,
      Vector<ColumnConfig> column_list,
      Vector<bool> copy_record);

  const PageManager* getPageManager() const {
    return page_mgr_;
  }

protected:

  CSTableReader(
      BinaryFormatVersion version,
      const PageManager* page_mgr,
      bool page_mgr_owned,
      Vector<ColumnConfig> columns,
      Vector<Function<RefPtr<ColumnReader> ()>> column_reader_factories,
      uint64_t num_rows,
      int fd);

  BinaryFormatVersion version_;
  const PageManager* page_mgr_;
  bool page_mgr_owned_;
  Vector<ColumnConfig> columns_;
  Vector<Function<RefPtr<ColumnReader> ()>> column_reader_factories_;
  HashMap<String, size_t> columns_by_name_;
  Vector<RefPtr<ColumnReader>> column_readers_shared_;
  List<RefPtr<ColumnReader>> column_readers_private_;
  uint64_t num_rows_;
  int fd_;
};

} // namespace cstable


