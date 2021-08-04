#include "TaskList.h"
#include <string.h>

CTaskList::CTaskList()
  : read_idx_(0),
    write_idx_(0)
{
  memset(task_node_, 0, sizeof(task_node_));
}

CTaskList::~CTaskList()
{
  for (int i = 0; i < MAX_TASK_NUM; ++i) {
    delete task_node_[i];
  }
}

bool CTaskList::push(IMysqlTask* task)
{
  uint16_t next_idx = static_cast<uint16_t>((write_idx_ + 1) % MAX_TASK_NUM);
  if (next_idx == read_idx_) return false;

  task_node_[write_idx_] = task;
  write_idx_ = next_idx;

  return true;
}

IMysqlTask* CTaskList::pop()
{
  if (write_idx_ == read_idx_) return nullptr;

  IMysqlTask* task = task_node_[read_idx_];
  read_idx_ = static_cast<uint16_t>((read_idx_ + 1) % MAX_TASK_NUM);

  return task;
}