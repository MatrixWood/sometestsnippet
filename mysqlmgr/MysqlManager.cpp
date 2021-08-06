#include "MysqlManager.h"
#include <sstream>
#include "MysqlThreadMgr.h"

CMysqlManager::CMysqlManager()
{
  str_characterset_ = "utf8";

  {
    STableInfo info;
    info.str_name_ = "t_user";
    info.map_field_["f_id"] = { "f_id", "bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增ID'", "bigint(20)" };
    info.map_field_["f_user_id"] = { "f_user_id", "bigint(20) NOT NULL COMMENT '用户ID'", "bigint(20)" };
    
    info.str_keystring_ = "PRIMARY KEY (f_user_id), INDEX f_user_id (f_user_id), KEY f_id (f_id)";
    vec_tableinfo_.push_back(info);
  }
}

CMysqlManager::~CMysqlManager()
{

}

bool CMysqlManager::init(const char* host, const char* usr,
  const char* pwd, const char* dbname)
{
  str_host_ = host;
  str_usr_ = usr;
  if (pwd != nullptr)
    str_pwd_ = pwd;
  str_dbname_ = dbname;

  conn_.reset(new CDatabaseMysql());
  if (!conn_->initialize(str_host_, str_usr_, str_pwd_, ""))
    return false;

  // 0. check database existence
  if (!isDBExist()) {
    if (!createDB()) {
      return false;
    }
  }

  // 1. check mysql connection again
  conn_.reset(new CDatabaseMysql());
  if (!conn_->initialize(str_host_, str_usr_, str_pwd_, str_dbname_)) {
    return false;
  }

  // 2. check correctness of table
  for (size_t i = 0; i < vec_tableinfo_.size(); ++i) {
    STableInfo table = vec_tableinfo_[i];
    if (!checkTable(table)) {
      return false;
    }
  }

  conn_.reset();
  return true;
}

bool CMysqlManager::isDBExist()
{
  if (conn_ == nullptr) return false;

  QueryResult* result = conn_->query("show databases");
  if (result == nullptr) return false;

  Field* row = result->fetch();
  while (row != nullptr) {
    
  }
}