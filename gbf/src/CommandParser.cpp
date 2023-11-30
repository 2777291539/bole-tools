#include "CommandParser.h"
#include "File.h"

Dcr::CommandParser::CommandParser(int c, char **v) : argc(c), argv(v)
{
    optstring = "hvp:";
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
    Dcr::File file{path};
    file.GenerateBehaviorFunction();
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
        }
    }
    if (!path.empty())
    {
        GenerateBehaviortreeFunction();
    }
}
