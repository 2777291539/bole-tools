#include "View.h"
#include <regex>

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
    std::regex handlerPatternMulti("\\s*handler\\s*=\\s*(?!Base)(\\w+)\\.(\\w+)");     // handler 多行显示
    std::regex handlerPatternMultiBase("\\s*handler\\s*=\\s*(?=Base)(\\w+)\\.(\\w+)"); // handler 多行显示
    std::regex selectorPattern("\\{\\s*behavior\\s*=\\s*\"[^\"]+\"\\s*,\\s*selector\\s*=\\s*(?!Base)(\\w+)"
                               "\\.(\\w+)\\s*,\\s*yes\\s*=\\s*\"(\\w+)\"\\s*,\\s*no\\s*=\\s*\"(\\w+)"); // selector
    std::regex selectorPatternBase("\\{\\s*behavior\\s*=\\s*\"[^\"]+\"\\s*,\\s*selector\\s*=\\s*(?=Base)(\\w+)"
                                   "\\.(\\w+)\\s*,\\s*yes\\s*=\\s*\"(\\w+)\"\\s*,\\s*no\\s*=\\s*\"(\\w+)"); // selector
    std::regex selectorPatternMulti("\\s*selector\\s*=\\s*(?!Base)(\\w+)\\.(\\w+)");     // selector 多行显示
    std::regex selectorPatternMultiBase("\\s*selector\\s*=\\s*(?=Base)(\\w+)\\.(\\w+)"); // selector 多行显示
    std::regex selectorPatternMultiYes("\\s*yes\\s*=\\s*\"(\\w+)\"");
    std::regex selectorPatternMultiNo("\\s*no\\s*=\\s*\"(\\w+)\"");
    std::regex behaviorName("\\s*behavior\\s*=\\s*\"(\\w+)");
    std::regex comment("^\\s*--");
    std::regex functionCommentPattern("\\}\\s*,\\s*(--.+)$");
    std::string line;
    std::smatch match;
    // // 添加获取 board name
    // std::regex start("^function View:RegisterBehaviorTree()");
    // std::regex left("\\{");
    // std::regex right("\\}");
    std::regex boardName("^\\s*\\[\"(\\w+)\"\\]");
    // enum class State
    // {
    //     WAIT,
    //     START,
    //     END,
    // };
    // unsigned count = 0;
    // bool isStart = false;
    // State currentState = State::WAIT;
    while (std::getline(ifs, line))
    {
        if (std::regex_search(line, comment))
        {
            continue;
        }
        if (std::regex_search(line, match, boardName))
        {
            currentFile = match[1];
            currentFile[0] = toupper(currentFile[0]);
            currentFile = "Processor" + currentFile;
        }
        // if (currentState == State::WAIT && std::regex_search(line, start))
        // {
        //     currentState = State::START;
        // }
        // if (currentState == State::START)
        // {
        //     if (std::regex_search(line, match, boardName))
        //     {
        //     }
        //     if (std::regex_search(line, match, left))
        //     {
        //         ++count;
        //         isStart = true;
        //     }
        //     if (std::regex_search(line, match, right))
        //     {
        //         --count;
        //     }
        //     if (isStart && count == 0)
        //     {
        //         currentState = State::END;
        //     }
        // }
        if (std::regex_search(line, match, selectorPattern))
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
            if (std::regex_search(line, match, behaviorName))
            {
                functionInfo.behaviorName = match[1];
            }
            __AddFunction(fileName, functionInfo);
        }
        else if (std::regex_search(line, match, selectorPatternBase))
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
            if (std::regex_search(line, match, behaviorName))
            {
                functionInfo.behaviorName = match[1];
            }
            functionInfo.isBase = true;
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
            if (std::regex_search(line, match, functionCommentPattern))
            {
                functionInfo.comment = match[1];
            }
            if (std::regex_search(line, match, behaviorName))
            {
                functionInfo.behaviorName = match[1];
            }
            // 无法识别注释
            __AddFunction(fileName, functionInfo);
        }
        else if (std::regex_search(line, match, handlerPatternMultiBase))
        {
            // multi handler
            std::string fileName = match[1];
            std::string functionName = match[2];
            FunctionInfo functionInfo;
            functionInfo.functionName = functionName;
            functionInfo.functionType = Dcr::FunctionType::HANDLER;
            functionInfo.isBase = true;
            if (std::regex_search(line, match, functionCommentPattern))
            {
                functionInfo.comment = match[1];
            }
            if (std::regex_search(line, match, behaviorName))
            {
                functionInfo.behaviorName = match[1];
            }
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
            if (std::regex_search(line, match, functionCommentPattern))
            {
                functionInfo.comment = match[1];
            }
            if (std::regex_search(line, match, behaviorName))
            {
                functionInfo.behaviorName = match[1];
            }
            // 无法识别注释
            __AddFunction(fileName, functionInfo);
        }
        else if (std::regex_search(line, match, selectorPatternMultiBase))
        {
            std::string fileName = match[1];
            std::string functionName = match[2];
            Dcr::FunctionInfo functionInfo;
            functionInfo.functionName = functionName;
            functionInfo.functionType = Dcr::FunctionType::SELECTOR;
            functionInfo.isBase = true;
            if (std::regex_search(line, match, functionCommentPattern))
            {
                functionInfo.comment = match[1];
            }
            if (std::regex_search(line, match, behaviorName))
            {
                functionInfo.behaviorName = match[1];
            }
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
    std::regex board_name("^Processor(\\w+)");
    std::smatch match;
    if (std::regex_search(fileName, match, board_name))
    {
        std::string boardName = match[1];
        boardName[0] = tolower(boardName[0]);
        fileFuncInfo.boardName = boardName;
    }
    m_fileFunctionInfo.push_back(fileFuncInfo);
    return m_fileFunctionInfo.size() - 1;
}

void Dcr::View::__AddFunction(std::string fileName, FunctionInfo functionInfo)
{
    if (fileName == "Base")
    {
        // if (m_fileFunctionInfo.size() && (m_fileFunctionInfo.end() - 1)->fileName == currentFile)
        // {
        //     (m_fileFunctionInfo.end() - 1)->fileFunctionList.push_back(functionInfo);
        //     return;
        // }
        fileName = currentFile;
    }
    auto &list = m_fileFunctionInfo[_GetIndexByFile(fileName)].fileFunctionList;
    if (std::none_of(list.begin(), list.end(),
                     [&functionInfo](FunctionInfo info) -> bool
                     { return info.functionName == functionInfo.functionName; }))
    {
        if (m_hasLog)
        {
            std::cout << fileName << ": " << functionInfo.functionName << std::endl;
        }
        list.push_back(functionInfo);
        lastInfo = std::make_pair(_GetIndexByFile(fileName), list.size() - 1);
    }
}

void Dcr::View::GenerateRegisterBehavior(const std::vector<FileFunctionInfo> &info)
{
    std::ifstream in{m_path};
    if (!in.is_open())
    {
        std::cerr << "failed to open: " << m_path << std::endl;
        return;
    }
    std::string line;
    std::regex start("^function View:RegisterBehaviorTree()");
    std::regex left("\\{");
    std::regex right("\\}");
    enum class State
    {
        WAIT,
        START,
        END,
    };
    State currentState = State::WAIT;
    std::smatch match;
    unsigned count = 0;
    bool isStart = false;
    std::stringstream ss;
    bool isComplete = false;
    while (std::getline(in, line))
    {
        if (currentState == State::WAIT && std::regex_search(line, match, start))
        {
            currentState = State::START;
        }
        if (currentState == State::START)
        {
            if (std::regex_search(line, match, left))
            {
                ++count;
                isStart = true;
            }
            if (std::regex_search(line, match, right))
            {
                --count;
            }
            if (isStart && count == 1 && !isComplete)
            {
                ss << line << std::endl;
                for (const auto &file : info)
                {
                    ss << "\t\t";
                    std::string board_name = file.boardName;
                    board_name[0] = tolower(board_name[0]);
                    ss << R"([")" << board_name << R"("] = {)" << std::endl;
                    for (const auto &func : file.fileFunctionList)
                    {
                        ss << "\t\t\t";
                        ss << R"({behavior = ")";
                        ss << std::left << std::setfill(' ') << std::setw(50);
                        ss << func.behaviorName + R"(",)";
                        // ss << "\t\t\t\t\t";
                        // if (!func.comment.empty())
                        // {
                        //     ss << std::setw(30);
                        // }
                        // else
                        // {
                        //     ss << std::setw(0);
                        // }
                        if (func.functionType == FunctionType::HANDLER)
                        {
                            ss << R"(handler = )";
                            ss << (func.isBase ? "Base." : file.fileName + ".") << func.functionName << R"(},)";
                        }
                        else if (func.functionType == FunctionType::SELECTOR)
                        {
                            ss << R"(selector = )";
                            ss << (func.isBase ? "Base." : file.fileName + ".") << func.functionName << R"(, )";
                            ss << R"(yes = ")" << func.yes << R"(", )";
                            ss << R"(no = ")" << func.no << R"("},)";
                        }
                        if (!func.comment.empty())
                        {
                            ss << " " << func.comment;
                        }
                        ss << std::setw(0);
                        ss << std::endl;
                    }
                    ss << "\t\t";
                    ss << R"(},)" << std::endl;
                }
                isComplete = true;
            }
            if (isStart && count == 0)
            {
                currentState = State::END;
            }
            if (!isComplete)
            {
                ss << line << std::endl;
            }
        }
        if (currentState != State::START)
        {
            ss << line << std::endl;
        }
    }
    std::ofstream out{m_path};
    if (out.is_open())
    {
        out << ss.str();
    }
    in.close();
    out.close();
}
