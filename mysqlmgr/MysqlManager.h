#ifndef _MYSQLMGR_MYSQLMANAGER_H_
#define _MYSQLMGR_MYSQLMANAGER_H_

#include <memory>
#include <map>
#include <vector>

#include "../mysqlapi/DatabaseMysql.h"

struct STableField {
  STableField() {}
  STableField(std::string str_name, std::string str_type, std::string str_idx)
    : str_name_(str_name),
      str_type_(str_type),
      str_desc_(str_idx)
  {
  }

  std::string str_name_;
  std::string str_type_;
  std::string str_desc_;
};

struct STableInfo {
  STableInfo() {}
  STableInfo(std::string str_name)
    : str_name_(str_name)
  {
  }

  std::string str_name_;
  std::map<std::string, STableField> map_field_;
  std::string str_keystring_;
};

class CMysqlManager {
 public:
  CMysqlManager();
  virtual ~CMysqlManager();

  bool init(const char* host, const char* user, const char* pwd, const char* dbname);

  std::string getHost() { return str_host_; }
  std::string getUser() { return str_usr_; }
  std::string getPwd() { return str_pwd_; }
  std::string getDBName() { return str_dbname_; }
  std::string getCharSet() { return str_characterset_; }

 private:
  bool isDBExist();
  bool createDB();
  bool checkTable(const STableInfo& table);
  bool createTable(const STableInfo& table);
  bool updateTable(const STableInfo& table);

 protected:
  std::shared_ptr<CDatabaseMysql> conn_;
  std::string str_host_;
  std::string str_usr_;
  std::string str_pwd_;
  std::string str_dbname_;
  std::string str_characterset_;

  std::vector<STableInfo> vec_tableinfo_;
};



#endif