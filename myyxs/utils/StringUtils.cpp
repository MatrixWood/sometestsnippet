#include "StringUtils.h"
#include <string.h>
#include <algorithm>
#include <sys/timeb.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <uuid/uuid.h>

namespace yxs_utils {
  void toUpperString(std::string& str) {
    transform(str.begin(), str.end(), str.begin(), (int (*)(int))toupper);
  }

  void toLowerString(std::string& str) {
    transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);
  }

  void strSplit(const std::string& str, char spliter, std::vector<std::string>& out) {
    std::size_t posBegin(0);
    std::size_t posFind = str.find(spliter, posBegin);
    while (posFind != std::string::npos) {
      std::string tmp = str.substr(posBegin, posFind - posBegin);
      out.push_back(tmp);
      posBegin = posFind + 1;
      posFind = str.find(spliter, posBegin);
    }

    std::string tmp = str.substr(posBegin, str.length() - posBegin);
    if (tmp != "") {
      out.push_back(tmp);
    }
  }

  void strSplit(const std::string& str, std::string& split, std::vector<std::string>& out) {
    out.clear();
    size_t pos_s = 0;
    size_t pos_e = str.find(split.at(0), pos_s);
    while (pos_e != std::string::npos) {
      out.push_back(str.substr(pos_s, pos_e - pos_s));
      pos_s = pos_e + split.size();
      pos_e = str.find(split.at(0), pos_s);
    }
    out.push_back(str.substr(pos_s, str.size() - pos_s));
  }

  std::string getFileNameByPath(const std::string& path) {
    std::string::size_type off = path.rfind('\\');
    if (off == std::string::npos)
      return "";
    return path.substr(off + 1, path.length() - off - 1);
  }

  void findFiles(std::string path, std::string format, std::vector<std::string>& fileFullNames) {
    DIR *pDir;
    struct dirent *ptr;
    if (!(pDir = opendir(path.c_str())))
      return;
    while ((ptr = readdir(pDir)) != 0) {
      if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
        std::string file = path + "/" + ptr->d_name;
        if (opendir(file.c_str())) {
          findFiles(file, format, fileFullNames);
        } else {
          if (format == file.substr(file.size() - format.size())) {
            fileFullNames.push_back(file);
          }
        }
      }
    }
    closedir(pDir);
  }

  std::string getTime() {
    char ret[1024] = {0};
    struct timeb tb = {0,0};
    ftime(&tb);
    tm *tout = localtime(&tb.time);
    sprintf(ret,"%d-%d-%d %d:%d:%d.%03d", 
            tout->tm_year + 1900, tout->tm_mon + 1, tout->tm_mday, tout->tm_hour,
            tout->tm_min, tout->tm_sec,tb.millitm);
    return std::string(ret);
  }

	std::string getDate() {
		char ret[1024] = {0};
		struct timeb tb = {0,0};
		ftime(&tb);
		tm *tout = localtime(&tb.time);
		sprintf(ret,"%d%d%d%d%d%d", 
            tout->tm_year + 1900, tout->tm_mon + 1, tout->tm_mday, tout->tm_hour,
			      tout->tm_min, tout->tm_sec);
		return std::string(ret);
	}

  int access(const char* filename, int accessMode) {
    return ::access(filename,accessMode);
  }

  int mkDir(const char* path) {
    return ::mkdir(path, 0777);
  }

  bool CreatePath(const std::string& path) {
    if (access(path.c_str(),0) > -1) {
      return true;
    }
    std::string subpath ;
    size_t pos1 = path.find_last_of("\\");
    size_t pos2 = path.find_last_of("/");
    if (pos1 == std::string::npos && pos2 == std::string::npos) {
      return false;
    }
    if (pos2 == std::string::npos || pos1 > pos2) {
      subpath = path.substr(0, path.find_last_of("\\"));
    }
    if (pos1 == std::string::npos || pos1 < pos2) {
      subpath = path.substr(0, path.find_last_of("/"));
    }

    if (!createPath(subpath)) {
      return false;
    }
    if (mkDir(path.c_str()) < 0) {
      return false;
    }
    return true;
  }



  std::string GetUUID() {
    char buf[64] = { 0 };
    uuid_t uu;
    uuid_generate(uu);

    for (int32_t i = 0; i < 16; i++) {
      sprintf(buf + i*2, "%02X", uu[i]);
    }
    return std::move(std::string(buf));
  }


  uint64_t atoi64(const char* val) {
    return strtoll(val, NULL, 10);
  }

}