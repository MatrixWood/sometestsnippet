#ifndef _MYSQLAPI_DATABASEMYSQL_H_
#define _MYSQLAPI_DATABASEMYSQL_H_

#include <stdint.h>
#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include "QueryResult.h"

#define MAX_QUERY_LEN 1024

class CDatabaseMysql {
 public:
  struct DatabaseInfo {
    std::string str_host;
    std::string str_usr;
    std::string str_pwd;
    std::string str_dbname;
  };

 public:
  CDatabaseMysql();
  ~CDatabaseMysql();

  bool initialize(const std::string& host, const std::string& usr, const std::string& pwd, const std::string& dbname);
  QueryResult* query(const char* sql);
  QueryResult* query(const std::string& sql) { return query(sql.c_str()); }
  QueryResult* pquery(const char* format, ...);

  bool execute(const char* sql);
  bool execute(const char* sql, uint32_t& affected_count, int& errno_);
  bool pexecute(const char* format, ...);

  uint32_t getInsertID();
  void clearStoreResults();
  int32_t escapeString(char* dst, const char* src, uint32_t size);

 private:
  DatabaseInfo db_info_;
  std::unique_ptr<MYSQL> mysql_;
  bool is_init_;
};



#endif