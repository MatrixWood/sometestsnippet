#include "Log.h"

#include <string.h>
#include <stdarg.h>
#include <sstream>

namespace yxs {

void Log::Info(const char* msg) {
  std::stringstream ss;
  ss << "[INFO]" << msg;
  printf("%s\n", ss.str().c_str());
}

void Log::Debug(const char* msg) {
  std::stringstream ss;
  ss << "[DEBUG]" << msg;
  printf("%s\n", ss.str().c_str());
}
void Log::Warn(const char* msg) {
  std::stringstream ss;
  ss << "[WARN]" << msg;
  printf("%s\n", ss.str().c_str());
}

void Log::Error(const char* msg) {
  std::stringstream ss;
  ss << "[ERROR]" << msg;
  perror(ss.str().c_str());
}

void Log::Fatal(const char* msg) {
  std::stringstream ss;
  ss << "[FATAL]" << msg;
  perror(ss.str().c_str());
  Crash();
}

void Log::Crash() {
  char* p = nullptr;
  *p = 0;
}

}