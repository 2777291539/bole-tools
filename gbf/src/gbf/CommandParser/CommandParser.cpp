#include "CommandParser.h"
#include "Processor.h"
#include "View.h"

static bool hasLog = false;

Dcr::CommandParser::CommandParser(int c, char **v) : argc(c), argv(v)
{
    optstring = "hvp:l";
}

void Dcr::CommandParser::PrintHelp()
{
    std::cout << "Usage: "
              << "gbf"
              << " [-v] [-p path] [-h]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -v                Print version information" << std::endl;
    std::cout << "  -p <path>         Specify a path" << std::endl;
    std::cout << "  -h                Print this help message" << std::endl;
    std::cout << "  -l                Print log" << std::endl;
}

void Dcr::CommandParser::PrintVersion()
{
    std::cout << "Version: " << DCR_VERSION << std::endl;
}

void Dcr::CommandParser::GenerateBehaviortreeFunction()
{
    // if (path.empty())
    // {
    //     std::cout << "Need input a file!" << std::endl;
    //     return;
    // }
    if (!std::filesystem::exists(path))
    {
        std::cout << "Wrong file path: \"" << path << "\"" << std::endl;
        return;
    }
    Dcr::View viewFile{path, hasLog};
    viewFile.GenerateBehaviorFunctionInfo(); // 生成行为树函数信息
    Dcr::Processor processorFile{path, hasLog};
    processorFile.GenerateBehaviorFunction(viewFile.GetFunctionInfo()); // 生成函数实现
    processorFile.PrintInfo();
}

void Dcr::CommandParser::parseCommands()
{
    if (argc == 1)
    {
        std::cout << "嗯?" << std::endl;
        exit(0);
    }
    if (argc >= 2 && argv[1][0] != '-')
    {
        path = argv[1];
    }
    int opt;
    while ((opt = getopt(argc, argv, optstring.c_str())) != -1)
    {
        switch (opt)
        {
        case 'v':
            PrintVersion();
            break;
        case 'h':
            PrintHelp();
            break;
        case 'p':
            path = optarg;
            break;
        case 'l':
            hasLog = true;
        }
    }
    if (!path.empty())
    {
        GenerateBehaviortreeFunction();
    }
}
