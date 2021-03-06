#include "EventLoop.h"

#include "Channel.h"
#include "Poller.h"

#include <assert.h>
#include <iostream>

using namespace net;

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    threadId_(base::CurrentThread::tid()),
    poller_(new Poller(this))
{
  std::cout << "EventLoop created " << this << " in thread " << threadId_ << "\n";
  if (t_loopInThisThread) {
    std::cout << "Another EventLoop " << t_loopInThisThread
      << " exists in this thread " << threadId_ << "\n";
  } else {
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
  quit_ = false;

  while (!quit_) {
    activeChannels_.clear();
    poller_->poll(kPollTimeMs, &activeChannels_);
    for (ChannelList::iterator it = activeChannels_.begin(); 
         it != activeChannels_.end(); ++it) {
      (*it)->handleEvent();
    }
  }

  std::cout << "EventLoop " << this << " stop looping" << "\n";
  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
}

void EventLoop::updateChannel(Channel* channel) {
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(channel);
}
