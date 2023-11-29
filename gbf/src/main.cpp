#include "CommandParser.h"

int main(int argc, char **argv)
{
    Dcr::CommandParser commandParser(argc, argv);
    commandParser.parseCommands();
}
