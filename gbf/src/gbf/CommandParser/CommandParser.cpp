#include "CommandParser.h"
#include "View.h"
#include "ViewConfig.h"

static bool hasLog = false;

#ifdef __LINUX__
#include <limits.h>
#include <unistd.h>

std::string Dcr::get_executable_path()
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

std::string Dcr::get_executable_path()
{
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
    {
        // Resolve symbolic links to get the real path
        char *real_path = realpath(path, NULL);
        if (real_path != NULL)
        {
            std::string ret = std::filesystem::path(real_path).parent_path().parent_path().string();
            free(real_path);
            return ret;
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }
}
#endif

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
        std::cout << "1: config\t2: register\t3: dry-run" << std::endl;
        std::cout << "4: generate\t5: quit\t\t6: help\t" << std::endl;
        std::cout << "What now> ";
        char in;
        std::cin >> in;
        switch (in)
        {
            case '1':
            case 'c':
                if (__GenerateConfig())
                {
                }
                break;
            case '2':
            case 'r':
                if (__DefaultRegister())
                {
                }
                break;
            case '3':
            case 'd':
            {
                ViewConfig viewConfig;
                std::string configPath;
                configPath = Dcr::get_executable_path() + "/Default/Config";
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
    std::cout << "1. config:\t 生成对应配置文件" << std::endl;
    std::cout << "2. register:\t 注册行为树函数" << std::endl;
    std::cout << "3. dry-run:\t 查看结果" << std::endl;
    std::cout << "4. generate:\t 实现行为树函数" << std::endl;
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
    // std::string configPath;
    std::string targetPath;
    // configPath = Dcr::get_executable_path() + "/Default/Config";
    targetPath = Dcr::get_executable_path() + "/Default/Path";
    // viewConfig.SetConfigFile(configPath); // 读取配置文件
    // viewConfig.ReadConfig();
    std::ifstream pathFile{targetPath};
    if (!pathFile.is_open())
    {
        std::cerr << "can not open Path file" << std::endl;
        return false;
    }
    pathFile >> targetPath;
    pathFile.close();
    return __ImplementBehaviortreeFunction(targetPath);
    // Dcr::Processor processorFile{targetPath, hasLog};
    // processorFile.ImplementBehaviorFunction(viewConfig.GetFunctionInfo()); // 生成函数实现
    // processorFile.PrintInfo();
    // return true;
}

bool Dcr::CommandParser::__DefaultRegister()
{
    ViewConfig viewConfig;
    std::string configPath;
    std::string targetPath;
    configPath = Dcr::get_executable_path() + "/Default/Config";
    targetPath = Dcr::get_executable_path() + "/Default/Path";
    viewConfig.SetConfigFile(configPath); // 读取配置文件
    viewConfig.ReadConfig();
    std::ifstream pathFile{targetPath};
    if (!pathFile.is_open())
    {
        std::cerr << "can not open Path file" << std::endl;
        return false;
    }
    pathFile >> targetPath;
    pathFile.close();
    Dcr::View viewFile{targetPath, hasLog};
    viewFile.GenerateRegisterBehavior(viewConfig.GetFunctionInfo()); // 生成函数实现
    return true;
}

bool Dcr::CommandParser::__GenerateConfig()
{
    std::string targetPath = Dcr::get_executable_path() + "/Default/Path";
    std::ifstream pathFile{targetPath};
    if (!pathFile.is_open())
    {
        std::cerr << "can not open Path file" << std::endl;
        return false;
    }
    pathFile >> targetPath;
    pathFile.close();
    Dcr::View viewFile{targetPath, hasLog};
    viewFile.GenerateBehaviorFunctionInfo();
    viewFile.GetFunctionInfo();
    Dcr::ViewConfig viewConfig;
    viewConfig.GenerateDefautConfig(viewFile.GetFunctionInfo());
    return true;
}
