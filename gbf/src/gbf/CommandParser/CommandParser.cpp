#include "CommandParser.h"
#include "Processor.h"
#include "View.h"

static bool hasLog = false;

Dcr::CommandParser::CommandParser(int c, char **v) : argc(c), argv(v)
{
    optstring = "hvp:li";
}

void Dcr::CommandParser::ParseCommands()
{
    if (argc == 1)
    {
        std::cout << "嗯?" << std::endl;
        exit(0);
    }
    if (argc == 2 && argv[1][0] != '-')
    {
        path = argv[1];
    }
    else if (!strcmp(argv[1], "-i"))
    {
        _InterAction(); // enter inter action
        return;
    }
    int opt;
    while ((opt = getopt(argc, argv, optstring.c_str())) != -1)
    {
        switch (opt)
        {
            case 'v':
                __PrintVersion();
                break;
            case 'h':
                _PrintHelp();
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
        __ImplementBehaviortreeFunction(path);
    }
}

void Dcr::CommandParser::_PrintHelp()
{
    std::cout << "Usage: "
              << "gbf"
              << " [option] [file]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -v                Print version information" << std::endl;
    std::cout << "  -p <path>         Specify a path" << std::endl;
    std::cout << "  -h                Print this help message" << std::endl;
    std::cout << "  -l                Print log" << std::endl;
    std::cout << "  -i                interaction" << std::endl;
}

void Dcr::CommandParser::__PrintVersion()
{
    std::cout << "Version: " << DCR_VERSION << std::endl;
}

bool Dcr::CommandParser::__ImplementBehaviortreeFunction(std::string &path)
{
    if (!__CheckPath(path))
    {
        return false;
    }
    std::cout << std::endl;
    Dcr::View viewFile{path, hasLog};
    viewFile.GenerateBehaviorFunctionInfo(); // 生成行为树函数信息
    Dcr::Processor processorFile{path, hasLog};
    processorFile.ImplementBehaviorFunction(viewFile.GetFunctionInfo()); // 生成函数实现
    processorFile.PrintInfo();
    return true;
}

void Dcr::CommandParser::_InterAction()
{
    do
    {
        std::cout << std::endl;
        std::cout << "*** Commands ***" << std::endl;
        std::cout << "1: default\t2: config\t3: result" << std::endl;
        std::cout << "4: generate\t5: quit\t\t6: help\t" << std::endl;
        std::cout << "What now> ";
        char in;
        std::cin >> in;
        switch (in)
        {
            case '1':
            case 'd':
                break;
            case 2:
            case 'c':
                break;
            case 3:
            case 'r':
                break;
            case '4':
            case 'g':
                if (__InterActionGenerate())
                {
                    return;
                }
                break;
            case '5':
            case 'q':
                std::cout << "Bye." << std::endl;
                exit(0);
            case '6':
            case 'h':
                __PrintHelp();
                break;
        }
    } while (true);
}

bool Dcr::CommandParser::__InterActionGenerate()
{
    std::cout << std::endl;
    std::cout << "*** 4.generate(q:quit) ***" << std::endl;
    std::string path;
    do
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        path.clear();
        std::cout << "path> ";
        std::cin >> path;
        if (path.size() == 1 and *begin(path) == 'q')
        {
            return false;
        }
    } while (!__ImplementBehaviortreeFunction(path));
    return true;
}

void Dcr::CommandParser::__PrintHelp()
{
    std::cout << std::endl;
    std::cout << "1. default:\t 生成默认配置文件" << std::endl;
    std::cout << "2. config:\t 生成对应配置文件" << std::endl;
    std::cout << "3. result:\t 查看结果" << std::endl;
    std::cout << "4. generate:\t 生成或实现行为树函数" << std::endl;
    std::cout << "5. quit:\t 退出" << std::endl;
    std::cout << std::endl;
}

bool Dcr::CommandParser::__CheckPath(std::string &path)
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
    }
    catch (std::runtime_error err)
    {
        std::cout << std::endl << err.what() << std::endl;
        return false;
    }
    return true;
}
