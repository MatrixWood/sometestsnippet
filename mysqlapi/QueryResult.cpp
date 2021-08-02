#include "QueryResult.h"

QueryResult::QueryResult(MYSQL_RES* result, uint64_t row_count, uint32_t field_count)
  : field_count_(field_count), 
    row_count_(row_count)
{
  result_.reset(result);
  current_row_ = std::make_unique<Field[]>(field_count_);

  MYSQL_FIELD* fields = mysql_fetch_fields(result_.get());

  for (uint32_t i = 0; i < field_count_; ++i) {
    if (fields[i].name != nullptr) {
      field_names_[i] = fields[i].name;
      vec_field_names_.push_back(fields[i].name);
    } else {
      field_names_[i] = "";
      vec_field_names_.push_back("");
    }

    current_row_[i].setType(convertNativeType(fields[i].type));
  }
}

QueryResult::~QueryResult()
{
  endQuery();
}

bool QueryResult::nextRow()
{
  if (!result_) return false;

  MYSQL_ROW row = mysql_fetch_row(result_.get());
  if (!row) {
    endQuery();
    return false;
  }

  unsigned long int* field_length = mysql_fetch_lengths(result_.get());
  for (uint32_t i = 0; i < field_count_; ++i) {
    if (row[i] == nullptr) {
      current_row_[i].is_null_ = true;
      current_row_[i].setValue("", 0);
    } else {
      current_row_[i].is_null_ = false;
      current_row_[i].setValue(row[i], field_length[i]);
    }
    current_row_[i].setName(field_names_[i]);
  }

  return true;
}

void QueryResult::endQuery() {
  if (current_row_) {
    delete[] current_row_.release();
  }

  if (result_) {
    mysql_free_result(result_.release());
  }
}

enum Field::DataTypes QueryResult::convertNativeType(enum_field_types mysql_type) const
{
  switch (mysql_type) {
    case FIELD_TYPE_TIMESTAMP:
    case FIELD_TYPE_DATE:
    case FIELD_TYPE_TIME:
    case FIELD_TYPE_DATETIME:
    case FIELD_TYPE_YEAR:
    case FIELD_TYPE_STRING:
    case FIELD_TYPE_VAR_STRING:
    case FIELD_TYPE_BLOB:
    case FIELD_TYPE_SET:
    case FIELD_TYPE_NULL:
      return Field::DB_TYPE_STRING;
    case FIELD_TYPE_TINY:

    case FIELD_TYPE_SHORT:
    case FIELD_TYPE_LONG:
    case FIELD_TYPE_INT24:
    case FIELD_TYPE_LONGLONG:
    case FIELD_TYPE_ENUM:
      return Field::DB_TYPE_INTEGER;
    case FIELD_TYPE_DECIMAL:
    case FIELD_TYPE_FLOAT:
    case FIELD_TYPE_DOUBLE:
      return Field::DB_TYPE_FLOAT;
    default:
      return Field::DB_TYPE_UNKNOWN;
  }
}