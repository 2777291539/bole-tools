#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace Dcr
{
    class File
    {
      public:
        File(std::string path, bool hasLog);
        virtual ~File();

      protected:
        std::string m_path;               // 文件路径
        bool m_hasLog = false;            // 是否打印 Log
        std::string m_targetFilePathHead; // 生成文件的目录
    };
} // namespace Dcr
