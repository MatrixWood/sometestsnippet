#include "EventLoop.h"

#include "Channel.h"
#include "Poller.h"
#include "TimerQueue.h"

#include <iostream>
#include <assert.h>

using namespace net;

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    threadId_(CurrentThread::tid()),
    poller_(new Poller(this)),
    timerQueue_(new TimerQueue(this))
{
  std::cout << "EventLoop created " << this << " in thread " << threadId_ << std::endl;
  if (t_loopInThisThread) {
    std::cout << "Another EventLoop " << t_loopInThisThread
      << " exists in this thread " << threadId_ << std::endl;
  } else {
    t_loopInThisThread = this;
  }
}

EventLoop::~EventLoop()
{
  assert(!looping_);
  t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;
  quit_ = false;

  while (!quit_) {
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    for (auto it = activeChannels_.begin();
         it != activeChannels_.end(); ++it)
    {
      (*it)->handleEvent();
    }
  }
  std::cout << "EventLoop " << this << " stop looping" << std::endl;
  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb) {
  return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb) {
  Timestamp time(addTime(Timestamp::now(), delay));
  return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb) {
  Timestamp time(addTime(Timestamp::now(), interval));
  return timerQueue_->addTimer(cb, time, interval);
}

void EventLoop::updateChannel(Channel* channel) {
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(channel);
}

void EventLoop::abortNotInLoopThread() {
  std::cout << "EventLoop::abortNotInLoopThread - EventLoop " << this
    << " was created in threadId_ = " << threadId_
    << ", current thread id = " << CurrentThread::tid();
}