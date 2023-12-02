#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#define DCR_VERSION "1.1.0"

namespace Dcr
{
    class CommandParser
    {
      public:
        CommandParser(int, char **);
        void parseCommands();

      private:
        void PrintHelp();
        void PrintVersion();
        void GenerateBehaviortreeFunction();

      private:
        int argc;
        char **argv;
        std::string optstring;

        std::string path;
    };
} // namespace Dcr
