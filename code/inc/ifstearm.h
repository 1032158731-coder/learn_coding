#ifndef __IFSTEARM_H__
#define __IFSTEARM_H__
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm> // 包含 find_if 所需的头文件

class IniFileHandler {
public:
    // 从INI文件中读取键值对
    void readIniFile(const std::string& filename);

    // 将键值对写入INI文件
    void writeIniFile(const std::string& filename) const;

    // 更新特定键的字符串值
    void updateStringKeyValue(const std::string& key, const std::vector<std::string>& newValues);

    // 添加新的字符串键值对
    void addStringKeyValue(const std::string& key, const std::vector<std::string>& values);

    // 删除特定键的所有字符串值
    void deleteStringKey(const std::string& key);

    // 更新特定键的整数值
    void updateIntKeyValue(const std::string& key, const std::vector<int>& newValues);

    // 添加新的整数键值对
    void addIntKeyValue(const std::string& key, const std::vector<int>& values);

    // 删除特定键的所有整数值
    void deleteIntKey(const std::string& key);

    // 检查键是否存在于字符串键值对中
    bool stringKeyExists(const std::string& key) const;

    // 检查键是否存在于整数键值对中
    bool intKeyExists(const std::string& key) const;

    // 获取特定键的字符串值
    std::vector<std::string> getStringValue(const std::string& key) const;

    // 获取特定键的整数值
    std::vector<int> getIntValue(const std::string& key) const;

    // 测试函数
    int main_test();

private:
    std::map<std::string, std::vector<std::string>> stringKeyValuePairs;
    std::map<std::string, std::vector<int>> intKeyValuePairs;

    // 辅助函数：将逗号分隔的字符串拆分为字符串向量
    std::vector<std::string> splitString(const std::string& str, char delimiter);

    // 辅助函数：将逗号分隔的字符串拆分为整数向量
    std::vector<int> splitToIntVector(const std::string& str, char delimiter);
};



#endif

