#! /bin/bash
./build.sh
./cppcompute "1 + 2"
while inotifywait -qq -e modify,create,delete ./src; do 
    clear
    if [ -n "$pid" ] && [ -e /proc/$pid ]; then
        kill "$pid"
    fi
    ./build.sh
    ./cppcompute "1 + 2" 
done
