EXE=builds/soundboard
SRC=main.cpp libs/tinyfiledialogs/tinyfiledialogs.c
INCLUDE_PATHS=-I libs/tinyfiledialogs
LIBS=-lraylib -lm -lpthread -lrt -lX11
ARGS=-Wall -Wextra -Wpedantic -Werror
CC=g++

debug:
	$(CC) -g --std=c++20 $(SRC) -o $(EXE)  $(ARGS) $(LIBS) $(INCLUDE_PATHS)

release:
	$(CC) --std=c++20 $(SRC) -o $(EXE)_release  $(ARGS) -O2 $(LIBS) $(INCLUDE_PATHS)
    