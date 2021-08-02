#ifndef _MYSQLAPI_FIELD_H_
#define _MYSQLAPI_FIELD_H_

#include <algorithm>
#include <string>

inline void toLowerString(std::string& str) {
  for (auto& i : str) {
    if (i >= 'A' && i <= 'Z') {
      i = i + ('a' - 'A');
    }
  }
}

class Field {
 public:
  enum DataTypes {
    DB_TYPE_UNKNOWN = 0x00,
    DB_TYPE_STRING  = 0x01,
    DB_TYPE_INTEGER = 0x02,
    DB_TYPE_FLOAT   = 0x03,
    DB_TYPE_BOOL    = 0x04
  };

  Field()
    : type_(DB_TYPE_UNKNOWN)
  {
    is_null_ = false;
  }
  Field(const Field& f) {
    value_ = f.value_;
    field_name_ = f.field_name_;
    type_ = f.getType();
  }
  Field(const char* value, const char* field_name, enum DataTypes type)
    : type_(type),
      value_(value),
      field_name_(field_name)
  {
  }
  ~Field() {}

  Field& operator = (const Field& f) {
    value_ = f.value_;
    field_name_ = f.field_name_;
    type_ = f.getType();
    return *this;
  }

  enum DataTypes getType() const { return type_; }
  const std::string getString() const { return value_; }
  std::string getCppString() const { return value_; }
  float getFloat() const { return static_cast<float>(atof(value_.c_str())); }
  bool getBool() const { return atoi(value_.c_str()) > 0; }
  uint8_t getUInt8() const { return static_cast<uint8_t>(atol(value_.c_str())); }
  int8_t getInt8() const { return static_cast<int8_t>(atol(value_.c_str())); }
  uint16_t getUInt16() const { return static_cast<uint16_t>(atol(value_.c_str())); }
  int16_t getInt16() const { return static_cast<int16_t>(atol(value_.c_str())); }
  uint32_t getUInt32() const { return static_cast<uint32_t>(atol(value_.c_str())); }
  int32_t getInt32() const { return static_cast<int32_t>(atol(value_.c_str())); }
  uint64_t getUInt64() const { return static_cast<uint64_t>(atoll(value_.c_str())); }
  int64_t getInt64() const { return static_cast<int64_t>(atoll(value_.c_str())); }

  void setType(enum DataTypes type) { type_ = type; }
  void setValue(const char* value, size_t len) { value_.assign(value, len); }
  void setName(const std::string& str_name) {
    field_name_ = str_name;
    toLowerString(field_name_);
  }
  const std::string& getName() const { return field_name_; }

  bool isNull() { return is_null_; }

 public:
  bool is_null_;

 private:
  std::string value_;
  std::string field_name_;
  enum DataTypes type_;
};


#endif