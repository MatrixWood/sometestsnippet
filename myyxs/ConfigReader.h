#ifndef _YXS_CONFIGREADER_H_
#define _YXS_CONFIGREADER_H_

#include <string>
#include <map>
#include <vector>

namespace yxs {

typedef std::map<std::string, std::map<std::string, std::string>* > STR_MAP;

class ConfigReader {
 public:
  ~ConfigReader();

  std::string getString(const std::string& section, const std::string& key, const std::string& default_value = "");
  std::vector<std::string> getStringList(const std::string& section, const std::string& key);
  unsigned getNumber(const std::string& section, const std::string& key, unsigned default_value = 0);
  bool getBool(const std::string& section, const std::string& key, bool default_value = false);
  float getFloat(const std::string& section, const std::string& key, const float& default_value);

  static bool setPath(const std::string& path);

  static ConfigReader* instance();

 private:
  ConfigReader() {}

  bool isSection(std::string line, std::string& section);
  unsigned parseNumber(const std::string& s);
  std::string trimLeft(const std::string& s);
  std::string trimRight(const std::string& s);
  std::string trim(const std::string& s);
  bool load(const std::string& path);

  static ConfigReader* config;

  STR_MAP map_;
};

} // namespace yxs

#endif