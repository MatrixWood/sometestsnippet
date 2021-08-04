#ifndef _MYSQLMGR_TASKLIST_H_
#define _MYSQLMGR_TASKLIST_H_

#include "MysqlTask.h"
#include <stdint.h>
#include <memory>

#define MAX_TASK_NUM 1000

class CTaskList {
 public:
  CTaskList();
  ~CTaskList();

  bool push(IMysqlTask* task);
  IMysqlTask* pop();

 private:
  uint16_t read_idx_;
  uint16_t write_idx_;
  IMysqlTask* task_node_[MAX_TASK_NUM];
};

#endif