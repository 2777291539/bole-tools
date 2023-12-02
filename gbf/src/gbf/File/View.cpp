#include "View.h"

Dcr::View::View(std::string path, bool hasLog) : Dcr::File(path, hasLog)
{
}

Dcr::View::~View()
{
}

void Dcr::View::GenerateBehaviorFunctionInfo()
{
    _GetBehaviorFunctionByOrder();
}

std::vector<Dcr::FileFunctionInfo> Dcr::View::GetFunctionInfo()
{
    return m_fileFunctionInfo;
}

void Dcr::View::_GetBehaviorFunctionByOrder()
{
    std::ifstream ifs(m_path);
    if (!ifs.is_open())
    {
        std::cerr << "Fail to open file: " + m_path << std::endl;
        exit(0);
    }
    std::regex handlerPatternMulti("\\s*handler\\s*=\\s*(?!Base)(\\w+)\\.(\\w+)"); // handler 多行显示
    std::regex selectorPattern("\\{\\s*behavior\\s*=\\s*\"[^\"]+\"\\s*,\\s*selector\\s*=\\s*(?!Base)(\\w+)"
                               "\\.(\\w+)\\s*,\\s*yes\\s*=\\s*\"(\\w+)\"\\s*,\\s*no\\s*=\\s*\"(\\w+)"); // selector
    std::regex selectorPatternMulti("\\s*selector\\s*=\\s*(?!Base)(\\w+)\\.(\\w+)"); // selector 多行显示
    std::regex selectorPatternMultiYes("\\s*yes\\s*=\\s*\"(\\w+)\"");
    std::regex selectorPatternMultiNo("\\s*no\\s*=\\s*\"(\\w+)\"");
    std::regex comment("^\\s*--");
    std::regex functionCommentPattern("\\}\\s*,\\s*(--.+)$");
    std::string line;
    std::smatch match;
    while (std::getline(ifs, line))
    {
        if (std::regex_search(line, comment))
        {
            continue;
        }
        else if (std::regex_search(line, match, selectorPattern))
        {
            std::string fileName = match[1];
            std::string functionName = match[2];
            Dcr::FunctionInfo functionInfo;
            functionInfo.functionName = functionName;
            functionInfo.functionType = Dcr::FunctionType::SELECTOR;
            functionInfo.yes = match[3];
            functionInfo.no = match[4];
            if (std::regex_search(line, match, functionCommentPattern))
            {
                functionInfo.comment = match[1];
            }
            __AddFunction(fileName, functionInfo);
        }
        else if (std::regex_search(line, match, handlerPatternMulti))
        {
            // multi handler
            std::string fileName = match[1];
            std::string functionName = match[2];
            FunctionInfo functionInfo;
            functionInfo.functionName = functionName;
            functionInfo.functionType = Dcr::FunctionType::HANDLER;
            // 无法识别注释
            __AddFunction(fileName, functionInfo);
        }
        else if (std::regex_search(line, match, selectorPatternMulti))
        {
            std::string fileName = match[1];
            std::string functionName = match[2];
            Dcr::FunctionInfo functionInfo;
            functionInfo.functionName = functionName;
            functionInfo.functionType = Dcr::FunctionType::SELECTOR;
            // 无法识别注释
            __AddFunction(fileName, functionInfo);
        }
        else if (std::regex_search(line, match, selectorPatternMultiYes))
        {
            m_fileFunctionInfo[lastInfo.first].fileFunctionList[lastInfo.second].yes = match[1];
        }
        else if (std::regex_search(line, match, selectorPatternMultiNo))
        {
            m_fileFunctionInfo[lastInfo.first].fileFunctionList[lastInfo.second].no = match[1];
        }
    }
    ifs.close();
}

int Dcr::View::_GetIndexByFile(std::string fileName)
{
    for (int i = 0; i < m_fileFunctionInfo.size(); i++)
    {
        const auto &funcInfo = m_fileFunctionInfo[i];
        if (funcInfo.fileName == fileName)
        {
            return i;
        }
    }
    FileFunctionInfo fileFuncInfo;
    fileFuncInfo.fileName = fileName;
    m_fileFunctionInfo.push_back(fileFuncInfo);
    return m_fileFunctionInfo.size() - 1;
}

void Dcr::View::__AddFunction(std::string fileName, FunctionInfo functionInfo)
{
    auto &list = m_fileFunctionInfo[_GetIndexByFile(fileName)].fileFunctionList;
    if (std::none_of(list.begin(), list.end(), [&functionInfo](FunctionInfo info) -> bool {
            return info.functionName == functionInfo.functionName;
        }))
    {
        if (m_hasLog)
        {
            std::cout << fileName << ": " << functionInfo.functionName << std::endl;
        }
        list.push_back(functionInfo);
        lastInfo = std::make_pair(_GetIndexByFile(fileName), list.size() - 1);
    }
}
