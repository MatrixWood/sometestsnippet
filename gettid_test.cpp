#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

pid_t gettid() {
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

void* threadFunc(void*) {
  pid_t id = gettid();
  std::cout << "gettid: " << id << "\n";
}

int main() {
  pthread_t t1, t2;
  pthread_create(&t1, nullptr, threadFunc, nullptr);
  std::cout << t1 << "\n";
  pthread_join(t1, nullptr);

  pthread_create(&t2, nullptr, threadFunc, nullptr);
  std::cout << t2 << "\n";
  pthread_join(t2, nullptr);

  pid_t id = getpid();
  std::cout << id << "\n";
  
  return 0;
}