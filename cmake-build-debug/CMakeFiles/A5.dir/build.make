# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/A5.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/A5.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/A5.dir/flags.make

CMakeFiles/A5.dir/main.cpp.o: CMakeFiles/A5.dir/flags.make
CMakeFiles/A5.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/A5.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/A5.dir/main.cpp.o -c "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/main.cpp"

CMakeFiles/A5.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/A5.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/main.cpp" > CMakeFiles/A5.dir/main.cpp.i

CMakeFiles/A5.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/A5.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/main.cpp" -o CMakeFiles/A5.dir/main.cpp.s

CMakeFiles/A5.dir/A5Engine.cpp.o: CMakeFiles/A5.dir/flags.make
CMakeFiles/A5.dir/A5Engine.cpp.o: ../A5Engine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/A5.dir/A5Engine.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/A5.dir/A5Engine.cpp.o -c "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/A5Engine.cpp"

CMakeFiles/A5.dir/A5Engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/A5.dir/A5Engine.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/A5Engine.cpp" > CMakeFiles/A5.dir/A5Engine.cpp.i

CMakeFiles/A5.dir/A5Engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/A5.dir/A5Engine.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/A5Engine.cpp" -o CMakeFiles/A5.dir/A5Engine.cpp.s

CMakeFiles/A5.dir/Player.cpp.o: CMakeFiles/A5.dir/flags.make
CMakeFiles/A5.dir/Player.cpp.o: ../Player.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/A5.dir/Player.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/A5.dir/Player.cpp.o -c "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/Player.cpp"

CMakeFiles/A5.dir/Player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/A5.dir/Player.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/Player.cpp" > CMakeFiles/A5.dir/Player.cpp.i

CMakeFiles/A5.dir/Player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/A5.dir/Player.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/Player.cpp" -o CMakeFiles/A5.dir/Player.cpp.s

CMakeFiles/A5.dir/types/StaticObject.cpp.o: CMakeFiles/A5.dir/flags.make
CMakeFiles/A5.dir/types/StaticObject.cpp.o: ../types/StaticObject.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/A5.dir/types/StaticObject.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/A5.dir/types/StaticObject.cpp.o -c "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/types/StaticObject.cpp"

CMakeFiles/A5.dir/types/StaticObject.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/A5.dir/types/StaticObject.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/types/StaticObject.cpp" > CMakeFiles/A5.dir/types/StaticObject.cpp.i

CMakeFiles/A5.dir/types/StaticObject.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/A5.dir/types/StaticObject.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/types/StaticObject.cpp" -o CMakeFiles/A5.dir/types/StaticObject.cpp.s

CMakeFiles/A5.dir/types/Enemy.cpp.o: CMakeFiles/A5.dir/flags.make
CMakeFiles/A5.dir/types/Enemy.cpp.o: ../types/Enemy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/A5.dir/types/Enemy.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/A5.dir/types/Enemy.cpp.o -c "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/types/Enemy.cpp"

CMakeFiles/A5.dir/types/Enemy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/A5.dir/types/Enemy.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/types/Enemy.cpp" > CMakeFiles/A5.dir/types/Enemy.cpp.i

CMakeFiles/A5.dir/types/Enemy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/A5.dir/types/Enemy.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/types/Enemy.cpp" -o CMakeFiles/A5.dir/types/Enemy.cpp.s

# Object files for target A5
A5_OBJECTS = \
"CMakeFiles/A5.dir/main.cpp.o" \
"CMakeFiles/A5.dir/A5Engine.cpp.o" \
"CMakeFiles/A5.dir/Player.cpp.o" \
"CMakeFiles/A5.dir/types/StaticObject.cpp.o" \
"CMakeFiles/A5.dir/types/Enemy.cpp.o"

# External object files for target A5
A5_EXTERNAL_OBJECTS =

A5: CMakeFiles/A5.dir/main.cpp.o
A5: CMakeFiles/A5.dir/A5Engine.cpp.o
A5: CMakeFiles/A5.dir/Player.cpp.o
A5: CMakeFiles/A5.dir/types/StaticObject.cpp.o
A5: CMakeFiles/A5.dir/types/Enemy.cpp.o
A5: CMakeFiles/A5.dir/build.make
A5: CMakeFiles/A5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable A5"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/A5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/A5.dir/build: A5
.PHONY : CMakeFiles/A5.dir/build

CMakeFiles/A5.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/A5.dir/cmake_clean.cmake
.PHONY : CMakeFiles/A5.dir/clean

CMakeFiles/A5.dir/depend:
	cd "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src" "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src" "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug" "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug" "/Users/benkeefe/Downloads/SaulGoodman_A3 2/src/cmake-build-debug/CMakeFiles/A5.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/A5.dir/depend

