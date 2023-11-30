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
    };

    struct FileFunctionInfo
    {
        std::string fileName;
        std::vector<FunctionInfo> fileFunctionList;
    };

    class File
    {
      public:
        File(std::string path, bool hasLog);
        virtual ~File();
        void GenerateBehaviorFunction();

      protected:
        void _GenerateBehaviorFunction(int index);
        void _GetBehaviorFunctionByOrder();
        int _GetIndexByFile(std::string fileName);
        void _SetTargetFileInfo();

      private:
        void __AddFunction(std::string fileName, FunctionInfo functionInfo);
        std::string __GenerateFunctionTemplate(std::string fileName, const FunctionInfo &info);

      private:
        std::string m_path;
        std::vector<FileFunctionInfo> m_fileFunctionInfo;
        std::ifstream m_fileStream;

        std::string m_targetFilePathHead;
        std::string m_handlerFunctionTemplate;
        std::string m_selectorFunctionFreeTemplate;
        std::string m_selectorFunctionBonusTemplate;

        unsigned int createCount = 0;
        unsigned int changeCount = 0;

        std::pair<int, int> lastInfo;

        bool m_hasLog = false;
    };
} // namespace Dcr
