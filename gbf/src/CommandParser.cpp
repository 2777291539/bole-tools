#include "CommandParser.h"
#include "File.h"

void Dcr::CommandParser::parseCommands()
{
    if (!arguments.size())
    {
        std::string str = "用法: gbf [option] [可选参数]\n\n"
                          "选项:\n"
                          "    -v | --version        查看当前版本\n"
                          "    -p | --path <file>    生成行为树函数";
        std::cout << str << std::endl;
        return;
    }
    bool pathFlag = false;
    std::string path;
    for (const std::string &arg : arguments)
    {
        // 主版本号.次版本号.修订号
        if (arg == "-v" || arg == "--version")
        {
            std::cout << "version 1.0.0" << std::endl;
            return;
        }
        else if (arg == "-p" || arg == "--path")
        {
            pathFlag = true;
        }
        else if (pathFlag)
        {
            pathFlag = false;
            path = arg;
        }
        else if (path.empty())
        {
            path = arg;
        }
        else
        {
            std::cout << "嗯?" << std::endl;
            return;
        }
    }
    if (path.empty())
    {
        std::cout << "Need input a file!" << std::endl;
        return;
    }
    if (!std::filesystem::exists(path))
    {
        std::cout << "Wrong file path!" << std::endl;
        return;
    }
    Dcr::File file{path};
    file.GenerateBehaviorFunction();
}
