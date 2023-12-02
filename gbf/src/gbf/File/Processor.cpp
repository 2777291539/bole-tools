#include "Processor.h"

Dcr::Processor::Processor(std::string path, bool hasLog) : Dcr::File(path, hasLog)
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

void Dcr::Processor::PrintInfo()
{
    std::cout << "Create " << (createCount == 1 ? "file" : "files") << " count: " << createCount << std::endl;
    std::cout << "Change " << (changeCount == 1 ? "file" : "files") << " count: " << changeCount << std::endl;
}

void Dcr::Processor::ImplementBehaviorFunction(std::vector<FileFunctionInfo> fileFunctionInfo)
{
    for (int i = 0; i < fileFunctionInfo.size(); i++)
    {
        __ImplementBehaviorFunction(fileFunctionInfo, i);
    }
}

void Dcr::Processor::__ImplementBehaviorFunction(std::vector<FileFunctionInfo> fileFunctionInfo, int index)
{
    // _GetBehaviorFunctionByOrder();
    if (index < 0 || index >= fileFunctionInfo.size())
    {
        std::cout << "Wrong file param!" << std::endl;
        return;
    }
    FileFunctionInfo fileFuncInfo = fileFunctionInfo[index];
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
                outFileStream << __ImplementFunctionTemplate(fileName, *iterator) << std::endl;
            }
            else if (iterator == fileFuncInfo.fileFunctionList.end() - 1)
            {
                outFileStream << std::endl << __ImplementFunctionTemplate(fileName, *iterator);
            }
            else
            {
                outFileStream << std::endl << __ImplementFunctionTemplate(fileName, *iterator) << std::endl;
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
                            ss << std::endl << std::endl << __ImplementFunctionTemplate(fileName, *currentIterator);
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
                ss << std::endl << std::endl << __ImplementFunctionTemplate(fileName, *currentIterator);
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

std::string Dcr::Processor::__ImplementFunctionTemplate(std::string fileName, const FunctionInfo &info)
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

bool Dcr::Processor::__FilterPath(std::string &path)
{
    int pos = path.find("GameTheme");
    if (pos != std::string::npos)
    {
        path = path.substr(pos, path.size());
        return true;
    }
    return false;
}

void Dcr::Processor::__DealLuaPath(std::string &path)
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
