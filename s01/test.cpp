#include "Channel.h"
#include "EventLoop.h"

#include <iostream>
#include <sys/timerfd.h>

net::EventLoop* g_loop;

void timeout() { 
  std::cout << "Timeout!\n";
  g_loop->quit();
}

int main() { 
  net::EventLoop loop;
  g_loop = &loop;

  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  net::Channel channel(&loop, timerfd);
  channel.setReadCallback(timeout);
  channel.enableReading();

  struct itimerspec howlong;
  bzero(&howlong, sizeof(howlong));
  howlong.it_value.tv_sec = 5;
  ::timerfd_settime(timerfd, 0, &howlong, nullptr);

  loop.loop();

  ::close(timerfd);
}