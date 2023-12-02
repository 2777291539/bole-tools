#pragma once
#include "File.h"

namespace Dcr
{
    enum class FunctionType
    {
        HANDLER,
        SELECTOR
    };

    struct FunctionInfo
    {
        std::string functionName;
        std::vector<std::string> params;
        FunctionType functionType;
        std::string yes;
        std::string no;
        std::string comment;
        std::string behaviorName;
    };

    struct FileFunctionInfo
    {
        std::string fileName;
        std::vector<FunctionInfo> fileFunctionList;
    };

    class View : public File
    {
      public:
        View(std::string path, bool hasLog);
        virtual ~View();
        void GenerateBehaviorFunctionInfo(); // 生成行为树函数的信息
        std::vector<FileFunctionInfo> GetFunctionInfo();

      protected:
        void _GetBehaviorFunctionByOrder(); // 分析 View 文件，获取行为树函数信息

      private:
        int _GetIndexByFile(std::string fileName); // 获取指定文件名在文件函数信息的 index
        void __AddFunction(std::string fileName, FunctionInfo functionInfo); // 添加函数到文件函数信息中

      private:
        std::vector<FileFunctionInfo> m_fileFunctionInfo; // 文件函数信息
        std::pair<int, int> lastInfo;
    };
} // namespace Dcr
