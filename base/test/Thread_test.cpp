#include "../Thread.h"

#include <string>
#include <boost/bind.hpp>
#include <stdio.h>

#include <iostream>

void threadFunc() {
  std::cout << "tid = " << base::CurrentThread::tid() << "\n";
}

void threadFunc2(int x) {
  std::cout << "tid = " << base::CurrentThread::tid() << ", x = " << x << "\n";
}

void threadFunc3() {
  std::cout << "tid = " << base::CurrentThread::tid() << "\n";
  sleep(1);
}



int main() {
  std::cout << "pid = " << ::getpid() << ", tid = " << base::CurrentThread::tid() << "\n";

  base::Thread t1(threadFunc);
  t1.start();
  t1.join();

  base::Thread t2(boost::bind(threadFunc2, 42), "thread for free function with argument");
  t2.start();
  t2.join();
}