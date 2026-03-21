#/bin/bash
set -ex
EXE=builds/soundboard
MODE=$1
VALGRIND=$2
SRC="main.cpp tinyfiledialogs/tinyfiledialogs.c"
INCLUDE_PATHS="-I tinyfiledialogs" 
LIBS="-lraylib -lm -lpthread -lrt -lX11"
ARGS="-Wall -Wextra -Wpedantic -Werror"

if [[ $MODE = "debug"  ]]; then
  if [[ $MODE = "yes" ]]; then
    valgrind --leak-check=full log-file=memleak.txt --show-leak-kinds=all ./$EXE
  else
    ./$EXE
  fi
elif [[ $MODE = "release" ]]; then
  EXE="${EXE}_release"
  ARGS="${ARGS} -O2"
fi

g++ -g $SRC -o $EXE --std=c++20 $ARGS $LIBS $INCLUDE_PATHS
