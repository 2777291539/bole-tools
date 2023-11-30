#!/bin/bash

# 检查是否存在 bin 目录，如果不存在则创建
if [ ! -d "bin" ]; then
    mkdir bin
fi

# 执行 make 命令清理
make clean

# 执行 make 命令编译
echo "开始构建..."
make

# 设置可执行程序的路径
executable="./bin/gbf"

# 判断可执行程序是否存在
if [ -f "$executable" ]; then
    # 软链接可执行程序
    if [ ! -L /usr/local/bin/gbf ]; then
        ln -s "$(pwd)/bin/gbf" /usr/local/bin
    fi
    echo "构建成功!"
else
    echo "构建失败!"
fi
