EXE=builds/soundboard
SRC=main.cpp libs/tinyfiledialogs/tinyfiledialogs.c
INCLUDE_PATHS=-I libs/tinyfiledialogs
LIBS=-lraylib -lm -lpthread -lrt -lX11
ARGS=-Wall -Wextra -Wpedantic -Werror
CC=g++

# TODO: figure out how to link raylib statically
# make PLATFORM=PLATFORM_DESKTOP -> static version
# PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED -> dynamic version

install_raylib:
	git clone --depth 1 https://github.com/raysan5/raylib.git raylib && \
	mv raylib libs && cd libs/raylib/src/ && \
	make PLATFORM=PLATFORM_DESKTOP && \
	make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

debug:
	$(CC) -g --std=c++20 $(SRC) -o $(EXE)  $(ARGS) $(LIBS) $(INCLUDE_PATHS)

release:
	$(CC) --std=c++20 $(SRC) -o $(EXE)_release  $(ARGS) -O2 $(LIBS) $(INCLUDE_PATHS)
    