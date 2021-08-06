#include "MysqlThreadMgr.h"

CMysqlThreadMgr::CMysqlThreadMgr()
{
}

CMysqlThreadMgr::~CMysqlThreadMgr()
{
}

bool CMysqlThreadMgr::addTask(uint32_t hash_id, IMysqlTask* task)
{
  uint32_t idx = getTableHashID(hash_id);

  if (idx >= thread_count_) return false;

  return mysql_threads_[idx].addTask(task);
}

bool CMysqlThreadMgr::init(const std::string& host, const std::string& usr,
  const std::string& pwd, const std::string& dbname)
{
  for (uint32_t i = 0; i < thread_count_ + 1; ++i) {
    if (mysql_threads_[i].start(host, usr, pwd, dbname) == false)
      return false;
  }
  return true;
}

bool CMysqlThreadMgr::processReplyTask(int32_t count)
{
  bool is_result = false;

  for (uint32_t i = 0; i < thread_count_ + 1; ++i) {
    IMysqlTask* task = mysql_threads_[i].getReplyTask();
    int32_t process_nbr = 0;
    while (((count == -1) || (process_nbr < count)) && (task != nullptr)) {
      task->reply();
      task->release();
      task = mysql_threads_[i].getReplyTask();
      ++process_nbr;
    }

    if (process_nbr == count) {
      is_result = true;
    }
  }
  return is_result;
}