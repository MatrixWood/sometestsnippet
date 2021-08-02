#include "DatabaseMysql.h"
#include <fstream>
#include <stdarg.h>
#include <string.h>

CDatabaseMysql::CDatabaseMysql()
  : mysql_(nullptr),
    is_init_(false)
{
}

CDatabaseMysql::~CDatabaseMysql()
{
  if (mysql_ != nullptr) {
    if (is_init_) {
      mysql_close(mysql_.release());
    }
  }
}

bool CDatabaseMysql::initialize(const std::string& host, const std::string& usr,
  const std::string& pwd, const std::string& dbname)
{
  if (is_init_) {
    mysql_close(mysql_.release());
  }

  mysql_.reset(mysql_init(mysql_.get()));
  mysql_.reset(mysql_real_connect(mysql_.get(), 
                                  host.c_str(), usr.c_str(),
                                  pwd.c_str(), dbname.c_str(), 0, nullptr, 0));
  
  db_info_.str_dbname = dbname;
  db_info_.str_host = host;
  db_info_.str_usr = usr;
  db_info_.str_pwd = pwd;

  if (mysql_) {
    mysql_query(mysql_.get(), "set names utf8");
    is_init_ = true;
    return true;
  } else {
    mysql_close(mysql_.release());
    return false;
  }
  return false;
}

QueryResult* CDatabaseMysql::query(const char* sql)
{
  if (!mysql_) {
    if (initialize(db_info_.str_host, db_info_.str_usr, db_info_.str_pwd, db_info_.str_dbname) == false) {
      return nullptr;
    }
  }

  if (!mysql_) return nullptr;

  MYSQL_RES* result = 0;
  uint64_t row_count = 0;
  uint32_t field_count = 0;

  {
    int temp_ret = mysql_real_query(mysql_.get(), sql, strlen(sql));
    if (temp_ret) {
      unsigned int errno_ = mysql_errno(mysql_.get());
      if (errno_ == CR_SERVER_GONE_ERROR) {
        if (initialize(db_info_.str_host, db_info_.str_usr, db_info_.str_pwd, db_info_.str_dbname) == false) {
          return nullptr;
        }
        temp_ret = mysql_real_query(mysql_.get(), sql, strlen(sql));
        if (temp_ret) {
          // log
        }
      } else {
        return nullptr;
      }
    }

    result = mysql_store_result(mysql_.get());
    row_count = mysql_affected_rows(mysql_.get());
    field_count = mysql_field_count(mysql_.get());
  }

  if (!result) return nullptr;

  QueryResult* query_result = new QueryResult(result, row_count, field_count);
  query_result->nextRow();
  return query_result;
}

QueryResult* CDatabaseMysql::pquery(const char* format, ...)
{
  if (!format) return nullptr;

  va_list ap;
  char str_query[MAX_QUERY_LEN];
  va_start(ap, format);
  int res = vsnprintf(str_query, MAX_QUERY_LEN, format, ap);
  va_end(ap);

  if (res == -1) return nullptr;

  return query(str_query);
}

bool CDatabaseMysql::execute(const char* sql)
{
  if (!mysql_) return false;

  {
    int temp_ret = mysql_query(mysql_.get(), sql);
    if (temp_ret) {
      unsigned int errno_ = mysql_errno(mysql_.get());
      if (errno_ == CR_SERVER_GONE_ERROR) {
        if (initialize(db_info_.str_host, db_info_.str_usr, db_info_.str_pwd, db_info_.str_dbname) == false) {
          return false;
        }
        temp_ret = mysql_real_query(mysql_.get(), sql, strlen(sql));
        if (temp_ret) {
          // log
        }
      } else {
        // log
      }
      return false;
    }
  }
  return true;
}

bool CDatabaseMysql::execute(const char* sql, uint32_t& affected_count, int& errno_)
{
  if (!mysql_) return false;

  {
    int temp_ret = mysql_query(mysql_.get(), sql);
    if (temp_ret) {
      unsigned int errno_ = mysql_errno(mysql_.get());
      if (errno_ == CR_SERVER_GONE_ERROR) {
        if (initialize(db_info_.str_host, db_info_.str_usr, db_info_.str_pwd, db_info_.str_dbname) == false) {
          return false;
        }
        temp_ret = mysql_query(mysql_.get(), sql);
        errno_ = temp_ret;
        if (temp_ret) {
          // log
        }
      } else {
        // log
      }
      return false;
    }
    affected_count = static_cast<uint32_t>(mysql_affected_rows(mysql_.get()));
  }
  return true;
}

bool CDatabaseMysql::pexecute(const char* format, ...)
{
  if (!format) return false;

  va_list ap;
  char str_query[MAX_QUERY_LEN];
  va_start(ap, format);
  int res = vsnprintf(str_query, MAX_QUERY_LEN, format, ap);
  va_end(ap);

  if (res == -1) return false;

  if (!mysql_) return false;

  {
    int temp_ret = mysql_query(mysql_.get(), str_query);
    if (temp_ret) {
      unsigned int errno_ = mysql_errno(mysql_.get());
      if (errno_ == CR_SERVER_GONE_ERROR) {
        if (initialize(db_info_.str_host, db_info_.str_usr, db_info_.str_pwd, db_info_.str_dbname) == false) {
          return false;
        }
        temp_ret = mysql_query(mysql_.get(), str_query);
        if (temp_ret) {
          // log
        }
      } else {
        // log
      }
      return false;
    }
  }
  return true;
}

void CDatabaseMysql::clearStoreResults()
{
  if (!mysql_) return;

  MYSQL_RES* result = nullptr;
  while (!mysql_next_result(mysql_.get())) {
    if ((result = mysql_store_result(mysql_.get())) != nullptr) {
      mysql_free_result(result);
    }
  }
}

uint32_t CDatabaseMysql::getInsertID()
{
  return static_cast<uint32_t>(mysql_insert_id(mysql_.get()));
}

int32_t CDatabaseMysql::escapeString(char* dst, const char* src, uint32_t size)
{
  if (mysql_.get() == nullptr) return 0;
  if (dst == nullptr || src == nullptr) return 0;

  return mysql_real_escape_string(mysql_.get(), dst, src, size);
}