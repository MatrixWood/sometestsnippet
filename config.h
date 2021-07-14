#include <map>
#include <string>

class Config {
public:
    Config(const char* filename);
    ~Config();
    char* getConfigName(const char* name);
    int setConfigValue(const char* name, const char* value);

private:
    void loadFile(const char* filename);
    int writeFile(const char* filename = nullptr);
    void parseLine(char* line);
    char* trimSpace(char* name);

    bool m_load_ok;
    std::map<std::string, std::string> m_config_map;
    std::string m_config_file;
};