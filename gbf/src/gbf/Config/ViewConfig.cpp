#include "ViewConfig.h"
#include "View.h"
#include <cctype>
#include <regex>

bool Dcr::ViewConfig::SetConfigFile(std::string path)
{
    try
    {
        if (path.empty())
        {
            throw std::runtime_error("Empty file path");
        }
        if (*begin(path) == '\'' || *begin(path) == '\"')
        {
            path.erase(begin(path));
        }
        if (path.empty())
        {
            throw std::runtime_error("Empty file path");
        }
        if (*(end(path) - 1) == '\'' || *(end(path) - 1) == '\"')
        {
            path.erase(end(path) - 1);
        }
        if (path.empty())
        {
            throw std::runtime_error("Empty file path");
        }
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error("Wrong file path: \"" + path + "\"");
        }
        m_configFile = path;
    }
    catch (std::runtime_error err)
    {
        std::cout << std::endl << err.what() << std::endl;
        return false;
    }
    return true;
}

void Dcr::ViewConfig::ReadConfig()
{
    std::ifstream in(m_configFile);
    if (!in.is_open())
    {
        std::cerr << "Fail to open: " << m_configFile << std::endl;
        exit(0);
    }
    std::string line;
    std::regex comment("^\\s*(--\\s*.+)$");
    std::regex filePattern("^\\s*#\\s*(\\w+)");
    std::regex functionPattern("^\\s*\\*?(\\w+)");
    std::regex selectPattern("^\\s*(\\w+)\\s+(\\w+)\\s+(\\w+)");
    std::regex isBase("^\\s*\\*");
    std::smatch match;
    std::string commentStr;
    while (std::getline(in, line))
    {
        if (std::regex_search(line, match, comment))
        {
            commentStr = match[1];
            continue;
        }
        if (std::regex_search(line, match, filePattern))
        {
            FileFunctionInfo info;
            std::string file_name = match[1];
            file_name[0] = toupper(file_name[0]);
            info.fileName = "Processor" + file_name;
            info.boardName = file_name;
            m_fileFunctionInfo.push_back(info);
        }
        else if (std::regex_search(line, match, functionPattern))
        {
            FunctionInfo info;
            info.behaviorName = match[1];
            std::string funcName;
            bool preFlag = false;
            if (!commentStr.empty())
            {
                info.comment = commentStr;
                commentStr.clear();
            }
            for (const char &c : info.behaviorName)
            {
                if (c == '_')
                {
                    preFlag = true;
                    continue;
                }
                if (preFlag || funcName.empty())
                {
                    funcName += toupper(c);
                    preFlag = false;
                }
                else
                {
                    funcName += c;
                }
            }
            info.functionName = "Behavior" + funcName;
            if (std::regex_search(line, match, selectPattern))
            {
                info.functionType = FunctionType::SELECTOR;
                info.yes = match[2];
                info.no = match[3];
            }
            else
            {
                info.functionType = FunctionType::HANDLER;
            }
            if (std::regex_search(line, match, isBase))
            {
                info.isBase = true;
            }
            else
            {
                info.isBase = false;
            }
            (end(m_fileFunctionInfo) - 1)->fileFunctionList.push_back(info);
        }
    }
    in.close();
}

void Dcr::ViewConfig::PrintLog()
{
    for (const FileFunctionInfo &fileInfo : m_fileFunctionInfo)
    {
        std::cout << std::endl;
        std::cout << "----------" << fileInfo.fileName << "-----------" << std::endl;
        for (const FunctionInfo &funcInfo : fileInfo.fileFunctionList)
        {
            std::cout << std::endl;
            if (!funcInfo.comment.empty())
            {
                std::cout << "\t" << funcInfo.comment << std::endl;
            }
            std::cout << "\tBehavior Name: " << funcInfo.behaviorName << std::endl;
            std::cout << "\tFunction Type: " << ((int)funcInfo.functionType == 0 ? "HANDLER" : "SELECTOR") << std::endl;
            std::cout << "\tFunction Name: " << funcInfo.functionName << std::endl;
            if ((int)funcInfo.functionType)
            {
                std::cout << "\tYes: " << funcInfo.yes << std::endl;
                std::cout << "\tNo: " << funcInfo.no << std::endl;
            }
            std::cout << "\tBase: " << funcInfo.isBase << std::endl;
        }
    }
}

std::vector<Dcr::FileFunctionInfo> Dcr::ViewConfig::GetFunctionInfo()
{
    return m_fileFunctionInfo;
}

void Dcr::ViewConfig::GenerateDefautConfig(const std::vector<FileFunctionInfo> &fileInfo)
{
    std::stringstream ss;
    for (const auto &file : fileInfo)
    {
        if (file.boardName.empty())
        {
            continue;
        }
        ss << R"(#)";
        ss << file.boardName << std::endl;
        for (const auto &func : file.fileFunctionList)
        {
            if (!func.comment.empty())
            {
                ss << "    ";
                ss << func.comment << std::endl;
            }
            ss << "    ";
            if (func.isBase)
            {
                ss << R"(*)";
            }
            ss << func.behaviorName;
            if (func.functionType == FunctionType::SELECTOR)
            {
                ss << " " << func.yes;
                ss << " " << func.no;
            }
            ss << std::endl;
        }
    }
    std::string configPath = Dcr::get_executable_path() + "/Default/Config";
    std::ofstream out{configPath};
    if (out.is_open())
    {
        out << ss.str();
    }
    else
    {
        std::cerr << "路径错误🙅" << std::endl;
        exit(0);
    }
}
