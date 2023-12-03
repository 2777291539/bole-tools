#include "CommandParser.h"
#include "ViewConfig.h"
#include <fstream>

#ifdef __LINUX__
#include <limits.h>
#include <unistd.h>

std::string get_executable_path()
{
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len != -1)
    {
        buf[len] = '\0';
        return std::filesystem::path(buf).parent_path().parent_path().string();
        // return std::string(buf);
    }
    else
    {
        return "";
    }
    // std::filesystem::path executablePath = std::filesystem::path("/proc/self/exe");
    // return executablePath.parent_path().string();
}
#endif

#ifdef __MACOS__
#include <mach-o/dyld.h>

std::string get_executable_path()
{
    char buf[1024];
    uint32_t size = sizeof(buf);
    if (_NSGetExecutablePath(buf, &size) == 0)
    {
        return std::filesystem::path(buf).parent_path().parent_path().string();
        // return std::string(buf);
    }
    else
    {
        return "";
    }
}
#endif

static bool hasLog = false;

Dcr::CommandParser::CommandParser(int c, char **v) : argc(c), argv(v)
{
    optstring = "hvp:li";
}

void Dcr::CommandParser::ParseCommands()
{
    if (argc == 1)
    {
        __DefaultGenerate();
        return;
        // std::cout << "嗯?" << std::endl;
        // exit(0);
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
                break;
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
            case '2':
            case 'c':
                break;
            case '3':
            case 'r':
            {
                ViewConfig viewConfig;
                std::string configPath;
#ifdef __LINUX__
                configPath = get_executable_path() + "/Default/Config";
#endif
#ifdef __MACOS
#endif
                viewConfig.SetConfigFile(configPath);
                viewConfig.ReadConfig();
                viewConfig.PrintLog();
                break;
            }
            case '4':
            case 'g':
                if (__InterActionGenerate())
                {
                    // return;
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
    std::cout << "1: config\t2: path\t" << std::endl;
    std::cout << "What now> ";
    char in;
    std::cin >> in;
    std::string path;
    switch (in)
    {
        case '1':
        case 'c':
            __DefaultGenerate();
            break;
        case '2':
        case 'p':
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
            break;
    }
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

bool Dcr::CommandParser::__DefaultGenerate()
{
    ViewConfig viewConfig;
    std::string configPath;
    std::string targetPath;
    configPath = get_executable_path() + "/Default/Config";
    targetPath = get_executable_path() + "/Default/Path";
    viewConfig.SetConfigFile(configPath); // 读取配置文件
    viewConfig.ReadConfig();
    std::ifstream pathFile{targetPath};
    if (!pathFile.is_open())
    {
        std::cerr << "can not open Path file" << std::endl;
        return false;
    }
    pathFile >> targetPath;
    Dcr::Processor processorFile{targetPath, hasLog};
    processorFile.ImplementBehaviorFunction(viewConfig.GetFunctionInfo()); // 生成函数实现
    processorFile.PrintInfo();
    pathFile.close();
    return true;
}
