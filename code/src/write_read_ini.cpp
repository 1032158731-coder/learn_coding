#include "write_read_ini.h"
#include "scopedLock.h"

WtReadIni *WtReadIni::m_pInstance = NULL;
MultiThreadMutex WtReadIni::m_mutexOfInstance;


WtReadIni *WtReadIni::GetInstance()
{
    if(NULL == m_pInstance)
    {
        const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
        if(NULL == m_pInstance)
        {
            m_pInstance = new WtReadIni();
            if(DW_OK != m_pInstance->Init())
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    }
    
    return m_pInstance;



}


void WtReadIni::Destroy()
{
    const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }

}

int WtReadIni::Init()
{
	return DW_OK;
}


// 定义一个函数来写入ini文件
int WtReadIni::write_ini(const char *filename, const char *section, const char *key, const char *value)
{
    // 打开文件，如果不存在则创建
    FILE *file = fopen(filename, "a+"); // 使用"a+"模式以追加方式打开文件
    if (file == NULL) {
        perror("无法打开文件");
        return -1;
    }

    // 移动到文件开始位置
    fseek(file, 0, SEEK_SET);

    // 检查文件是否已经包含该节
    int found_section = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '[' && strstr(line, section)) {
            found_section = 1;
            break;
        }
    }

    // 如果没有找到该节，则写入新的节头
    if (!found_section) {
        fprintf(file, "[%s]\n", section);
    }

    // 移动到文件末尾准备写入新的键值对
    fseek(file, 0, SEEK_END);
    fprintf(file, "%s=%s\n", key, value);

    // 关闭文件
    fclose(file);

    return 0;
}

// 实现写入默认配置的函数
int WtReadIni::writeDefaultConfig() {
    // 写入一些数字配置
    if (this->write_ini("config.ini", "Settings", "Number1", "123") == 0) {
        log_info("Successfully wrote Number1 to config.ini\n");
    } else {
        log_error("Failed to write Number1 to config.ini\n");
    }
    
    if (this->write_ini("config.ini", "Settings", "Number2", "456") == 0) {
        log_info("Successfully wrote Number2 to config.ini\n");
    } else {
        log_error("Failed to write Number2 to config.ini\n");
    }
    
    if (this->write_ini("config.ini", "Numbers", "Value1", "789") == 0) {
        log_info("Successfully wrote Value1 to config.ini\n");
    } else {
        log_error("Failed to write Value1 to config.ini\n");
    }
    
    if (this->write_ini("config.ini", "Numbers", "Value2", "101112") == 0) {
        log_info("Successfully wrote Value2 to config.ini\n");
    } else {
        log_error("Failed to write Value2 to config.ini\n");
    }
    
    return 0;
}