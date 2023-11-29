#include <iostream>
#include <string>
#include <vector>

namespace Dcr
{
    class CommandParser
    {
      public:
        CommandParser(int argc, char *argv[]) : arguments(argv + 1, argv + argc)
        {
        }

        void parseCommands();

      private:
        std::vector<std::string> arguments;
    };
} // namespace Dcr
