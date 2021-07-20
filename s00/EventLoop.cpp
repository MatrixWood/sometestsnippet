#include "EventLoop.h"

#include <assert.h>
#include <poll.h>
#include <iostream>

using namespace net;

__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop()
  : looping_(false),
    threadId_(base::CurrentThread::tid())
{
  if (t_loopInThisThread) {
    std::cout << "ERROR: Another EventLoop: " << t_loopInThisThread
      << " exists in this thread: " << threadId_ << "\n";
  }
  else {
    t_loopInThisThread = this;
  }
}

EventLoop::~EventLoop() {
  assert(!looping_);
  t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;

  ::poll(nullptr, 0, 5*1000);
  std::cout << "EventLoop: " << this << " stop looping";
  looping_ = false;
}

void EventLoop::abortNotInLoopThread() {
  std::cout << "ERROR: "
            << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  base::CurrentThread::tid();
}