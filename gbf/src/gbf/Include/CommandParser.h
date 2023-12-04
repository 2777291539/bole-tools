#pragma once
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

#include "Processor.h"
#include "View.h"
#include "ViewConfig.h"

#define DCR_VERSION "2.0.0"

namespace Dcr
{
    std::string get_executable_path();

    class CommandParser
    {
    public:
        CommandParser(int, char **);
        void ParseCommands(); // 解析

    protected:
        void _InterAction(); // 进入交互模式
        void _PrintHelp();   // 查看帮助信息

    private:
        void __PrintVersion(); // 版本号

        bool __InterActionGenerate(); // 实现步骤

        void __DefaultConfig();                              // 生成默认配置
        void __CurrentConfig();                              // 生成当前配置
        void __PrintResult();                                // 打印结果
        void __GenerateBehaviortreeFunction();               // 生成行为树函数
        bool __ImplementBehaviortreeFunction(std::string &); // 行为树函数实现
        void __PrintHelp();                                  // 查看帮助信息

        bool __DefaultGenerate();
        bool __DefaultRegister();
        bool __GenerateConfig();

        bool __CheckPath(std::string &); // 异常处理

    private:
        int argc;
        char **argv;
        std::string optstring;

        std::string path;
    };
} // namespace Dcr
