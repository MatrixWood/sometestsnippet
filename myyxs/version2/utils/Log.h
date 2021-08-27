#ifndef _YXS_LOG_H_
#define _YXS_LOG_H_

namespace yxs {

class Log {
 public:
  static void Info(const char* msg);
  static void Debug(const char* msg);
  static void Warn(const char* msg);
  static void Error(const char* msg);
  static void Fatal(const char* msg);
};

} // namespace yxs


#endif 