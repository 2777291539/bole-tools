#pragma once

#include "View.h"

namespace Dcr
{
    class ViewConfig
    {
      public:
        void GenerateDefautConfig();                                       // 生成默认的配置文件
        void GenerateDefautConfig(std::vector<FileFunctionInfo> fileInfo); // 生成配置文件
        void ReadConfig();                                                 // 读取配置文件
        bool SetConfigFile(std::string);                                   // 设置配置文件路径
        std::vector<Dcr::FileFunctionInfo> GetFunctionInfo();              // 获取 info

        void PrintLog();

      private:
        std::string m_configFile;
        std::vector<FileFunctionInfo> m_fileFunctionInfo; // 文件函数信息
    };
}; // namespace Dcr
