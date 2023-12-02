#!/bin/bash

os_type=$(uname)

if [ ! -d "bin" ]; then
    mkdir bin
fi

make clean

echo "开始构建..."
if [ "$os_type" == "Darwin" ]; then
    git pull --depth=1 --autostash
    bear -- make
elif [ "$os_type" == "Linux" ]; then
    bear make
fi

executable="./bin/gbf"

if [ -f "$executable" ]; then
    if [ ! -L /usr/local/bin/gbf ]; then
        ln -s "$(pwd)/bin/gbf" /usr/local/bin
    fi
    echo "构建成功!"
else
    echo "构建失败!"
fi
