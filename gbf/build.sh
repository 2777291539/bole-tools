#!/bin/bash

os_type=$(uname)

if [ -n "$(find bin -type f)" ]; then
    make clean
fi

command_make()
{
    if [ "$os_type" == "Darwin" ]; then
        # git pull --depth=1 --autostash
        bear -- make
    elif [ "$os_type" == "Linux" ]; then
        bear make
    fi
}

show_progress()
{
    local delay=0.1
    local frames=("▁" "▂" "▃" "▄" "▅" "▆" "▇" "█")
    local current_frame=0
    local current_work=$1

    if [ "$os_type" == "Linux" ]; then
        echo -n -e '\e[?25l'
    fi
    printf "%s    " "$current_work..."

    command_make &

    local pid=$!
    trap "kill $pid 2> /dev/null" EXIT

    while kill -0 $pid >/dev/null 2>&1; do
        printf "\b\b\b[%s]" "${frames[current_frame]}"
        # echo -ne "\b\b\b[${frames[current_frame]}]"

        let current_frame=$current_frame+1
        if [ $current_frame -eq ${#frames[@]} ]
        then
            let current_frame=0
        fi

        sleep $delay
    done

    # echo -e ""
    if [ "$os_type" == "Linux" ]; then
        echo -n -e '\e[?25h'
    fi
    printf "\n"
    wait $pid
    trap - EXIT
}
# show_progress()
# {
#     local delay=0.5
#     local spin='-\|/'
#     local current_work=$1
#
#     echo -n "$current_work... "
#
#     command_make &
#
#     local pid=$!
#     trap "kill $pid 2> /dev/null" EXIT
#
#     while kill -0 $pid >/dev/null 2>&1; do
#         for i in $(seq 0 3); do
#             echo -ne "\b${spin:$i:1}"
#             sleep $delay
#         done
#     done
#     echo -e ""
#
#     wait $pid
#     trap - EXIT
# }

show_progress "构建中"

executable="./bin/gbf"

if [ -f "$executable" ]; then
    if [ -L /usr/local/bin/gbf ]; then
        rm /usr/local/bin/gbf
        ln -s "$(pwd)/bin/gbf" /usr/local/bin
    fi
    echo "构建成功!"
else
    echo "构建失败!"
fi
