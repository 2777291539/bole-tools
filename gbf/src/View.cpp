#include "View.h"

Dcr::View::View(std::string path, bool hasLog) : Dcr::File(path, hasLog)
{
    m_handlerFunctionTemplate = "    self.currentGameBoard:Step()\n"
                                "end";
    m_selectorFunctionFreeTemplate = "    if self.ctrl.free_game_data and (self.ctrl.free_game_data.trigger_free_spins "
                                     "or self.ctrl.free_game_data.free_spin_total) then\n"
                                     "        self.currentGameBoard:OnSelectorResult(true)\n"
                                     "    else\n"
                                     "        self.currentGameBoard:OnSelectorResult(false)\n"
                                     "    end\n"
                                     "end";
    m_selectorFunctionBonusTemplate = "    if self.ctrl.bonus_game_data then\n"
                                      "        self.currentGameBoard:OnSelectorResult(true)\n"
                                      "    else\n"
                                      "        self.currentGameBoard:OnSelectorResult(false)\n"
                                      "    end\n"
                                      "end";
}

Dcr::View::~View()
{
    std::cout << "Create " << (createCount == 1 ? "file" : "files") << " count: " << createCount << std::endl;
    std::cout << "Change " << (changeCount == 1 ? "file" : "files") << " count: " << changeCount << std::endl;
}

void Dcr::View::GenerateBehaviorFunction()
{
    _GetBehaviorFunctionByOrder();
    _SetTargetFileInfo();
    for (int i = 0; i < m_fileFunctionInfo.size(); i++)
    {
        _GenerateBehaviorFunction(i);
    }
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

void Dcr::View::_GenerateBehaviorFunction(int index)
{
    _GetBehaviorFunctionByOrder();
    if (index < 0 || index >= m_fileFunctionInfo.size())
    {
        std::cout << "Wrong file param!" << std::endl;
        return;
    }
    FileFunctionInfo fileFuncInfo = m_fileFunctionInfo[index];
    std::string fileName = fileFuncInfo.fileName;
    std::string targetFilePath = m_targetFilePathHead + fileName + ".lua";
    std::string requirePath = targetFilePath;
    bool requireFlag = __FilterPath(requirePath);
    if (!std::filesystem::exists(targetFilePath))
    {
        std::ofstream outFileStream{targetFilePath, std::ios::out};
        if (!outFileStream.is_open())
        {
            std::cerr << "Fail to Open: " << targetFilePath << std::endl;
            exit(-1);
        }
        if (requireFlag)
        {
            __DealLuaPath(requirePath);
            outFileStream << "local " + fileName + " = require \"" + requirePath + "\"\n\n";
        }
        for (auto iterator = fileFuncInfo.fileFunctionList.begin(); iterator != fileFuncInfo.fileFunctionList.end();
             iterator++)
        {
            if (iterator == fileFuncInfo.fileFunctionList.begin())
            {
                outFileStream << __GenerateFunctionTemplate(fileName, *iterator) << std::endl;
            }
            else if (iterator == fileFuncInfo.fileFunctionList.end() - 1)
            {
                outFileStream << std::endl << __GenerateFunctionTemplate(fileName, *iterator);
            }
            else
            {
                outFileStream << std::endl << __GenerateFunctionTemplate(fileName, *iterator) << std::endl;
            }
        }
        if (requireFlag)
        {
            outFileStream << std::endl << std::endl << "return " + fileName;
        }
        std::cout << "Create: " << targetFilePath << std::endl;
        createCount++;
        outFileStream.close();
    }
    else
    {
        bool needChange = false;
        std::ifstream preOutFileStream{targetFilePath, std::ios::in};
        if (!preOutFileStream.is_open())
        {
            std::cerr << "Fail to Open: " << targetFilePath << std::endl;
            exit(0);
        }
        enum class State
        {
            NORMAL,
            END
        };
        std::string line;
        std::regex commonPattern("^function " + fileName + "[.|:]" + "(\\w+)");
        std::regex endPattern("^return\\s+" + fileName);
        std::regex endSpacePattern("^\\s*$");
        std::smatch result;
        std::stringstream ss;
        std::stringstream endSpaceSs;
        std::stringstream endSs;
        State currentState = State::NORMAL;
        auto currentIterator = fileFuncInfo.fileFunctionList.begin();
        bool isFirst = true;
        unsigned int count{0};
        while (std::getline(preOutFileStream, line))
        {
            if (currentState == State::NORMAL and std::regex_search(line, result, endPattern))
            {
                currentState = State::END;
            }
            if (currentState == State::NORMAL)
            {
                if (std::regex_search(line, result, commonPattern))
                {
                    auto cmpFunc = [&result](FunctionInfo info) { return info.functionName == result[1]; };
                    auto it = std::find_if(fileFuncInfo.fileFunctionList.begin(), fileFuncInfo.fileFunctionList.end(),
                                           cmpFunc);
                    if (it != fileFuncInfo.fileFunctionList.end())
                    {
                        while (currentIterator != it)
                        {
                            needChange = true;
                            ss << std::endl << std::endl << __GenerateFunctionTemplate(fileName, *currentIterator);
                            currentIterator++;
                        }
                        currentIterator++; // skip this function
                    }
                }
                if (isFirst)
                {
                    ss << line;
                }
                else
                {
                    if (std::regex_search(line, result, endSpacePattern))
                    {
                        endSpaceSs << std::endl << line;
                    }
                    else
                    {
                        ss << endSpaceSs.str();
                        endSpaceSs.str("");
                        ss << std::endl << line;
                    }
                }
            }
            else if (currentState == State::END)
            {
                endSs << endSpaceSs.str();
                endSpaceSs.str("");
                endSs << std::endl << line;
            }
            isFirst = false;
        }
        if (currentIterator != fileFuncInfo.fileFunctionList.end())
        {
            while (currentIterator != fileFuncInfo.fileFunctionList.end())
            {
                needChange = true;
                ss << std::endl << std::endl << __GenerateFunctionTemplate(fileName, *currentIterator);
                currentIterator++;
            }
        }
        if (needChange)
        {
            std::ofstream outFileStream{targetFilePath, std::ios::out};
            if (!outFileStream.is_open())
            {
                std::cerr << "Wrong Open " << targetFilePath << std::endl;
                exit(-1);
            }
            outFileStream << ss.str() << endSs.str();
            std::cout << "Change: " << targetFilePath << std::endl;
            changeCount++;
            outFileStream.close();
        }
        preOutFileStream.close();
    }
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

bool Dcr::View::__FilterPath(std::string &path)
{
    int pos = path.find("GameTheme");
    if (pos != std::string::npos)
    {
        path = path.substr(pos, path.size());
        return true;
    }
    return false;
}

void Dcr::View::__DealLuaPath(std::string &path)
{
    int pos = path.find(".lua");
    if (pos != std::string::npos)
    {
        path = path.substr(0, pos);
    }
    for (auto &v : path)
    {
        if (v == '/')
        {
            v = '.';
        }
    }
}

std::string Dcr::View::__GenerateFunctionTemplate(std::string fileName, const FunctionInfo &info)
{
    std::string common = "function " + fileName + ":" + info.functionName + "()\n";
    if (!info.comment.empty())
    {
        common = info.comment + "\n" + common;
    }
    if (info.functionType == FunctionType::HANDLER)
    {
        return common + m_handlerFunctionTemplate;
    }
    else if (info.functionType == FunctionType::SELECTOR)
    {
        std::string freeStr1 = "free";
        std::string freeStr2 = "Free";
        size_t found1 = info.functionName.find(freeStr1);
        size_t found2 = info.functionName.find(freeStr2);
        if (found1 != std::string::npos or found2 != std::string::npos)
        {
            return common + m_selectorFunctionFreeTemplate;
        }
        else
        {
            return common + m_selectorFunctionBonusTemplate;
        }
    }
    return "";
}
