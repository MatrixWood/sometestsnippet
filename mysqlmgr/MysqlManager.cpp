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
    std::string name = row[0].getString();
    if (str_dbname_ == name) {
      result->endQuery();
      return true;
    }

    if (result->nextRow() == false) {
      break;
    }

    row = result->fetch();
  }

  result->endQuery();

  delete result;
  return false;
}

bool CMysqlManager::createDB()
{
  if (conn_ == nullptr) return false;

  uint32_t affected_count = 0;
  int errno_ = 0;

  std::stringstream ss;
  ss << "create database " << str_dbname_;

  if (conn_->execute(ss.str().c_str(), affected_count, errno_)) {
    if (affected_count == 1)
      return true;
  } else {
    return false;
  }
  return false;
}

bool CMysqlManager::checkTable(const STableInfo& table)
{
  if (conn_ == nullptr) return false;

  if (table.str_name_.find_first_not_of("\t\r\n") == std::string::npos) return true;

  std::stringstream ss;
  ss << "desc " << table.str_name_;
  QueryResult* result = conn_->query(ss.str());
  if (result == nullptr) {
    if (createTable(table))
      return true;
    return false;
  } else { // check field match
    std::map<std::string, std::string> map_oldtable;
    Field* row = result->fetch();
    while (row != nullptr) {
      std::string name = row[0].getString();
      std::string type = row[1].getString();
      map_oldtable[name] = type;

      if (result->nextRow() == false) {
        break;
      }
      row = result->fetch();
    }

    result->endQuery();
    delete result;

    for (auto it = table.map_field_.begin(); it != table.map_field_.end(); ++it) {
      STableField field = it->second;
      if (map_oldtable.find(field.str_name_) == map_oldtable.end()) {
        std::stringstream ss;
        ss << "alter table " << table.str_name_ << " add column "
           << field.str_name_ << " " << field.str_type_;
        
        std::string sql = ss.str();
        if (conn_->execute(sql.c_str())) {
          continue;
        } else {
          return false;
        }
      }
    }
  }

  return true;
}

bool createTable(const STableInfo& table) {
  if (table.map_field_.size() == 0) {
    return false;
  }

  std::stringstream ss;
  ss << "CREATE TABLE IF NOT EXISTS " << table.str_name_ << "(";

  for (auto it = table.map_field_.begin(); it != table.map_field_.end(); ++it) {
    if (it != table.map_field_.begin()) {
      ss << ", ";
    }

    STableField field = it->second;
    ss << field.str_name_ << " " << field.str_type_;
  }

  if (table.str_keystring_ != "") {
    ss << ", " << table.str_keystring_;
  }

  ss << ") default charset = utf8, Engine = InnoDB;";
  if (conn_->execute(ss.str().c_str())) {
    return true;
  }

  return false;
}