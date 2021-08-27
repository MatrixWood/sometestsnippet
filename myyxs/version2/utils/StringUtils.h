#ifndef _YXS_STRINGUTIL_H_
#define _YXS_STRINGUTIL_H_

#include <string>
#include <vector>

namespace yxs_utils {
  void toUpperString(std::string& str);

  void toLowerString(std::string& str);

  void strSplit(const std::string& str, char split, std::vector<std::string>& out);

  void strSplit(const std::string& str, std::string& split, std::vector<std::string>& out);

  std::string getFileNameByPath(const std::string& path);

  void findFiles(std::string path, std::string format, std::vector<std::string>& fileFullNames);

  std::string getTime();
  std::string getDate();

  int access(const char* filename, int accessMode);

  int mkDir(const char* path);

  bool createPath(const std::string& path);

  std::string getUUID();

  uint64_t atoi64(const char* val);
}

#endif