#pragma once
#include "File.h"
#include "View.h"

namespace Dcr
{
    class Processor : File
    {
      public:
        Processor(std::string path, bool hasLog);
        void ImplementBehaviorFunction(
            std::vector<FileFunctionInfo> m_fileFunctionInfo); // 根据文件函数信息和 index 生成行为树函数的实现
        void PrintInfo();

      private:
        void __ImplementBehaviorFunction(std::vector<FileFunctionInfo> m_fileFunctionInfo,
                                         int index); // 根据文件函数信息和 index 生成行为树函数的实现
        std::string __ImplementFunctionTemplate(std::string fileName,
                                                const FunctionInfo &info); // 生成函数实现的代码字符串
        bool __FilterPath(std::string &path);                              // 判断当前路径中是否有 GameTheme
        void __DealLuaPath(std::string &path); // 将路径中的 / 都替换为 . ; 格式统一

        unsigned int createCount = 0;                // 计数: 创建文件个数
        unsigned int changeCount = 0;                // 计数: 改变文件个数
        std::string m_handlerFunctionTemplate;       // handler 函数代码模版
        std::string m_selectorFunctionFreeTemplate;  // selector free 函数代码模版
        std::string m_selectorFunctionBonusTemplate; // selector bonus 函数代码模版
    };
} // namespace Dcr
