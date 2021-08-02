#ifndef _MYSQLAPI_QUERYRESULT_H_
#define _MYSQLAPI_QUERYRESULT_H_

#include <mysql/mysql.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <memory>

#include "Field.h"

class QueryResult {
 public:
  using FieldNames = std::map<uint32_t, std::string>;

  QueryResult(MYSQL_RES* result, uint64_t row_count, uint32_t field_count);
  virtual ~QueryResult();

  virtual bool nextRow();

  uint32_t getFieldIdx(const std::string& name) const {
    for (const auto& iter : field_names_) {
      if (iter.second == name) {
        return iter.first;
      }
    }
    return uint32_t(-1);
  }

  Field* fetch() const { return current_row_.get(); }

  const Field& operator [] (int idx) const {
    return current_row_.get()[idx];
  }

  const Field& operator [] (const std::string& name) const {
    return current_row_.get()[getFieldIdx(name)];
  }

  uint32_t getFieldCount() const { return field_count_; }
  uint64_t getRowCount() const { return row_count_; }
  const FieldNames& getFieldNames() const { return field_names_; }
  const std::vector<std::string>& getNames() const { return vec_field_names_; }

 private:
  enum Field::DataTypes convertNativeType(enum_field_types mysql_type) const;

 public:
  void endQuery();

 protected:
  std::unique_ptr<Field[]> current_row_;
  uint32_t field_count_;
  uint64_t row_count_;
  FieldNames field_names_;
  std::vector<std::string> vec_field_names_;
  std::unique_ptr<MYSQL_RES> result_;
};

#endif