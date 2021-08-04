#ifndef _MYSQLMGR_MYSQLTHREAD_H_
#define _MYSQLMGR_MYSQLTHREAD_H_

#include <condition_variable>
#include <thread>

#include "../mysqlapi/DatabaseMysql.h"
#include "MysqlTask.h"
#include "TaskList.h"

class CMysqlThread {
 public:
  CMysqlThread();
  ~CMysqlThread();

  void run();

  bool start(const std::string& host, const std::string& usr, const std::string& pwd, const std::string& dbname);
  void stop();
  bool addTask(IMysqlTask* task) {
    return tasks_.push(task);
  }

  IMysqlTask* getReplyTask() {
    return reply_tasks_.pop();
  }

 protected:
  bool init();
  void mainLoop();
  void uninit();

 private:
  bool is_terminate_;
  std::unique_ptr<std::thread> thread_;
  bool is_start_;
  CDatabaseMysql* conn_;
  CTaskList tasks_;
  CTaskList reply_tasks_;

  std::mutex mutex_;
  std::condition_variable cond_;
};


#endif