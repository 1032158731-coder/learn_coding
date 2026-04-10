#include "ifstearm.h"
#include "public.h"

// ��INI�ļ��ж�ȡ��ֵ��
void IniFileHandler::readIniFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "�޷������ļ�: " << filename << std::endl;
        return;
    }

    while (getline(file, line)) {
        // ȥ������β�ո�
        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");

        if (start != std::string::npos && end != std::string::npos) {
            line = line.substr(start, end - start + 1);

            // ����ע����
            if (line[0] == ';' || line[0] == '#') continue;

            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = line.substr(0, equalPos);
                std::string valueStr = line.substr(equalPos + 1);
                
                // ȥ��key��value��Χ�Ŀո�
                key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](unsigned char ch) {return !std::isspace(ch);}));
                key.erase(std::find_if(key.rbegin(), key.rend(), [](unsigned char ch) {return !std::isspace(ch);}).base(), key.end());
                valueStr.erase(valueStr.begin(), std::find_if(valueStr.begin(), valueStr.end(), [](unsigned char ch) {return !std::isspace(ch);}));
                valueStr.erase(std::find_if(valueStr.rbegin(), valueStr.rend(), [](unsigned char ch) {return !std::isspace(ch);}).base(), valueStr.end());

                // ���Խ�valueת��Ϊ��������
                try {
                    std::vector<int> values = splitToIntVector(valueStr, ',');
                    intKeyValuePairs[key] = values;
                } catch (...) {
                    // ����ת��ʧ�ܣ�����Ϊ���ַ�������
                    std::vector<std::string> values = splitString(valueStr, ',');
                    stringKeyValuePairs[key] = values;
                }
            }
        }
    }

    file.close();
}

// ����ֵ��д��INI�ļ�
void IniFileHandler::writeIniFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "�޷������ļ�: " << filename << std::endl;
        return;
    }

    for (const auto& pair : stringKeyValuePairs) {
        std::ostringstream oss;
        for (size_t i = 0; i < pair.second.size(); ++i) {
            oss << pair.second[i];
            if (i < pair.second.size() - 1) {
                oss << ",";
            }
        }
        file << pair.first << "=" << oss.str() << "\n";
    }

    for (const auto& pair : intKeyValuePairs) {
        std::ostringstream oss;
        for (size_t i = 0; i < pair.second.size(); ++i) {
            oss << pair.second[i];
            if (i < pair.second.size() - 1) {
                oss << ",";
            }
        }
        file << pair.first << "=" << oss.str() << "\n";
    }

    file.close();
}

// �����ض������ַ���ֵ
void IniFileHandler::updateStringKeyValue(const std::string& key, const std::vector<std::string>& newValues) {
    if (stringKeyExists(key)) {
        stringKeyValuePairs[key] = newValues;
    } else {
        std::cerr << "�� " << key << " ������" << std::endl;
    }
}

// �����µ��ַ�����ֵ��
void IniFileHandler::addStringKeyValue(const std::string& key, const std::vector<std::string>& values) {
    if (!stringKeyExists(key)) {
        stringKeyValuePairs[key] = values;
    } else {
        std::cerr << "�� " << key << " �Ѵ���" << std::endl;
    }
}

// ɾ���ض����������ַ���ֵ
void IniFileHandler::deleteStringKey(const std::string& key) {
    if (stringKeyExists(key)) {
        stringKeyValuePairs.erase(key);
    } else {
        std::cerr << "�� " << key << " ������" << std::endl;
    }
}

// �����ض���������ֵ
void IniFileHandler::updateIntKeyValue(const std::string& key, const std::vector<int>& newValues) {
    if (intKeyExists(key)) {
        intKeyValuePairs[key] = newValues;
    } else {
        std::cerr << "�� " << key << " ������" << std::endl;
    }
}

// �����µ�������ֵ��
void IniFileHandler::addIntKeyValue(const std::string& key, const std::vector<int>& values) {
    if (!intKeyExists(key)) {
        intKeyValuePairs[key] = values;
    } else {
        std::cerr << "�� " << key << " �Ѵ���" << std::endl;
    }
}

// ɾ���ض�������������ֵ
void IniFileHandler::deleteIntKey(const std::string& key) {
    if (intKeyExists(key)) {
        intKeyValuePairs.erase(key);
    } else {
        std::cerr << "�� " << key << " ������" << std::endl;
    }
}

// �������Ƿ��������ַ�����ֵ����
bool IniFileHandler::stringKeyExists(const std::string& key) const {
    return stringKeyValuePairs.find(key) != stringKeyValuePairs.end();
}

// �������Ƿ�������������ֵ����
bool IniFileHandler::intKeyExists(const std::string& key) const {
    return intKeyValuePairs.find(key) != intKeyValuePairs.end();
}

// ��ȡ�ض������ַ���ֵ
std::vector<std::string> IniFileHandler::getStringValue(const std::string& key) const {
    if (stringKeyExists(key)) {
        return stringKeyValuePairs.at(key);
    } else {
        std::cerr << "�� " << key << " ������" << std::endl;
        return {}; // ���ؿ�����
    }
}

// ��ȡ�ض���������ֵ
std::vector<int> IniFileHandler::getIntValue(const std::string& key) const {
    if (intKeyExists(key)) {
        return intKeyValuePairs.at(key);
    } else {
        std::cerr << "�� " << key << " ������" << std::endl;
        return {}; // ���ؿ�����
    }
}

// ���������������ŷָ����ַ�������Ϊ�ַ�������
std::vector<std::string> IniFileHandler::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (getline(ss, item, delimiter)) {
        // ȥ��item��Χ�Ŀո�
        item.erase(item.begin(), std::find_if(item.begin(), item.end(), [](unsigned char ch) {return !std::isspace(ch);}));
        item.erase(std::find_if(item.rbegin(), item.rend(), [](unsigned char ch) {return !std::isspace(ch);}).base(), item.end());
        result.push_back(item);
    }
    return result;
}

// ���������������ŷָ����ַ�������Ϊ��������
std::vector<int> IniFileHandler::splitToIntVector(const std::string& str, char delimiter) {
    std::vector<int> result;
    std::stringstream ss(str);
    std::string item;
    while (getline(ss, item, delimiter)) {
        // ȥ��item��Χ�Ŀո�
        item.erase(item.begin(), std::find_if(item.begin(), item.end(), [](unsigned char ch) {return !std::isspace(ch);}));
        item.erase(std::find_if(item.rbegin(), item.rend(), [](unsigned char ch) {return !std::isspace(ch);}).base(), item.end());
        result.push_back(std::stoi(item));
    }
    return result;
}

// Ժ
int IniFileHandler::main_test() {
    // һtxtļд"hello world"
    std::ofstream file("hello_world.txt");
    if (file.is_open()) {
        file << "hello world" << std::endl;
        file.close();
        std::cout << "Successfully wrote 'hello world' to hello_world.txt" << std::endl;
    } else {
        std::cerr << "Could not open file for writing." << std::endl;
        return -1;
    }

    return 0;
}
