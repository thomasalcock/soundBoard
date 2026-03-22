EXE=builds/soundboard
SRC=main.cpp loadsoundicon.h libs/tinyfiledialogs/tinyfiledialogs.c 
INCLUDE_PATHS=-I./libs/tinyfiledialogs -I./libs/raylib/src
LIBRARY_SEARCH_PATHS=-L./libs/raylib/src
LINKER_FLAGS=-l:libraylib.a -lm -lpthread -lrt -lX11 
ARGS=-Wall -Wextra -Wpedantic -Werror
CC=g++

#-l:staticlib.a tells the linker to statically link with this lib
# make PLATFORM=PLATFORM_DESKTOP -> static version
# PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED -> dynamic version

install_raylib:
	git clone --depth 1 https://github.com/raysan5/raylib.git raylib && \
	mv raylib libs && cd libs/raylib/src/ && \
	make PLATFORM=PLATFORM_DESKTOP 

debug:
	xxd -i assets/loadsoundicon.png > loadsoundicon.h
	$(CC) -g --std=c++20 $(SRC) -o $(EXE)  $(ARGS) \
	 $(INCLUDE_PATHS) $(LIBRARY_SEARCH_PATHS)	$(LINKER_FLAGS)

release:
	$(CC) --std=c++20 $(SRC) -o $(EXE)_release  $(ARGS) -O2 \
		$(INCLUDE_PATHS) $(LIBRARY_SEARCH_PATHS) $(LINKER_FLAGS) 
    
run_debug:
	./$(EXE)

run_release:
	./$(EXE)_release
