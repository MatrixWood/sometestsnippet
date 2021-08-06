#include "MysqlThread.h"
#include <functional>

CMysqlThread::CMysqlThread()
  : is_start_(false),
    is_terminate_(false),
    conn_(nullptr)
{
}

CMysqlThread::~CMysqlThread()
{
}

void CMysqlThread::run()
{
  mainLoop();
  uninit();

  if (thread_ != nullptr) {
    thread_->join();
  }
}

bool CMysqlThread::start(const std::string& host, const std::string& usr,
  const std::string& pwd, const std::string& dbname)
{
  conn_ = new CDatabaseMysql();
  if (conn_ == nullptr) return false;

  if (conn_->initialize(host, usr, pwd, dbname) == false) return false;

  return init();
}

void CMysqlThread::stop()
{
  if (is_terminate_) return;

  is_terminate_ = true;
  thread_->join();
}

bool CMysqlThread::init()
{
  if (is_start_) return true;

  thread_.reset(new std::thread(std::bind(&CMysqlThread::mainLoop, this)));

  {
    std::unique_lock<std::mutex> lock(mutex_);
    while (is_start_ == false) {
      cond_.wait(lock);
    }
  }
  return true;
}

void CMysqlThread::uninit()
{
}

void CMysqlThread::mainLoop()
{
  is_start_ = true;

  {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.notify_all();
  }

  IMysqlTask* task;

  while (!is_terminate_) {
    if ((task = tasks_.pop()) != nullptr) {
      task->execute(conn_);
      reply_tasks_.push(task);
      continue;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}