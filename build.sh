#/bin/bash

EXE=builds/soundboard
MODE=$1

if [[ $MODE = "debug"  ]]; then
 g++ -g main.cpp -o $EXE --std=c++20 \
   -Wall -Wextra -Wpedantic \
   -lraylib -lm -lpthread -lrt -lX11 && ./$EXE 
elif [[ $MODE = "release" ]]; then
 g++ main.cpp -o $EXE -O2 --std=c++20 \
   -Wall -Wextra -Wpedantic \
   -lraylib -lm -lpthread -lrt -lX11 && ./$EXE
fi
