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
        void GenerateBehaviorFunction(); // 生成行为树函数的实现

      protected:
        void _GetBehaviorFunctionByOrder();        // 分析 View 文件，获取行为树函数信息
        void _GenerateBehaviorFunction(int index); // 根据文件函数信息和 index 生成行为树函数的实现

      private:
        int _GetIndexByFile(std::string fileName); // 获取指定文件名在文件函数信息的 index
        void __AddFunction(std::string fileName, FunctionInfo functionInfo); // 添加函数到文件函数信息中
        std::string __GenerateFunctionTemplate(std::string fileName,
                                               const FunctionInfo &info); // 生成函数实现的代码字符串

      private:
        bool __FilterPath(std::string &path);  // 判断当前路径中是否有 GameTheme
        void __DealLuaPath(std::string &path); // 将路径中的 / 都替换为 . ; 格式统一

        std::vector<FileFunctionInfo> m_fileFunctionInfo; // 文件函数信息

        std::string m_handlerFunctionTemplate;       // handler 函数代码模版
        std::string m_selectorFunctionFreeTemplate;  // selector free 函数代码模版
        std::string m_selectorFunctionBonusTemplate; // selector bonus 函数代码模版

        unsigned int createCount = 0; // 计数: 创建文件个数
        unsigned int changeCount = 0; // 计数: 改变文件个数

        std::pair<int, int> lastInfo;
    };
} // namespace Dcr
