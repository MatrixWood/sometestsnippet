#include "config.h"
#include <stdio.h>
#include <string.h>

Config::Config(const char* filename) {
    loadFile(filename);
}

Config::~Config() {

}

char* Config::getConfigName(const char* name) {
    if (!m_load_ok) return nullptr;

    char* value = nullptr;
    std::map<std::string, std::string>::iterator it = m_config_map.find(name);
    if (it != m_config_map.end()) {
        value = (char*)it->second.c_str();
    }
    return value;
}

int Config::setConfigValue(const char* name, const char* value) {
    if (!m_load_ok) return -1;

    // std::map<std::string, std::string>::iterator it = m_config_map.find(name);
    // if (it != m_config_map.end()) {
    //     it->second = value;
    // }
    // else {
    //     m_config_map.insert(std::make_pair(name, value));
    // }

    auto [iter, inserted] = m_config_map.try_emplace(name);
    if (inserted)
        iter->second = value;
    else
        m_config_map.insert(std::make_pair(name, value));

    return writeFile();
}

void Config::loadFile(const char* filename) {
    m_config_file.clear();
    m_config_file.append(filename);
    FILE* fp = fopen(filename, "r");
    if (!fp) return;

    char buf[256];
    for (;;) {
        char* p = fgets(buf, 256, fp);
        if (!p) break;
        size_t len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len - 1] = 0;
        
        char* ch = strchr(buf, '#');
        if (ch) *ch = 0;
        if (strlen(buf) == 0) continue;
    }
}