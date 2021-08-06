#ifndef _MYSQLMGR_MYSQLTHREADMGR_H_
#define _MYSQLMGR_MYSQLTHREADMGR_H_

#include "MysqlTask.h"
#include "MysqlThread.h"

class CMysqlThreadMgr {
 public:
  CMysqlThreadMgr();
  virtual ~CMysqlThreadMgr();

  bool init(const std::string& host, const std::string& usr, const std::string& pwd, const std::string& dbname);
  bool addTask(uint32_t hash_id, IMysqlTask* task);
  bool addTask(IMysqlTask* task) {
    return mysql_threads_[thread_count_].addTask(task);
  }
  inline uint32_t getTableHashID(uint32_t hash_id) const {
    return hash_id % thread_count_;
  }
  bool processReplyTask(int32_t count);
  static uint32_t getThreadsCount() {
    return thread_count_;
  }

 protected:
  static const uint32_t thread_count_ = 9;
  CMysqlThread mysql_threads_[thread_count_];
};

#endif