# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/user/esp/CECS490/demo_1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/user/esp/CECS490/demo_1/build

# Utility rule file for dfu.

# Include any custom commands dependencies for this target.
include CMakeFiles/dfu.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/dfu.dir/progress.make

CMakeFiles/dfu:
	/Users/user/.espressif/python_env/idf5.0_py3.9_env/bin/python /Users/user/esp/esp-idf/tools/mkdfu.py write -o /Users/user/esp/CECS490/demo_1/build/dfu.bin --json /Users/user/esp/CECS490/demo_1/build/flasher_args.json --pid 2

dfu: CMakeFiles/dfu
dfu: CMakeFiles/dfu.dir/build.make
.PHONY : dfu

# Rule to build all files generated by this target.
CMakeFiles/dfu.dir/build: dfu
.PHONY : CMakeFiles/dfu.dir/build

CMakeFiles/dfu.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dfu.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dfu.dir/clean

CMakeFiles/dfu.dir/depend:
	cd /Users/user/esp/CECS490/demo_1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/user/esp/CECS490/demo_1 /Users/user/esp/CECS490/demo_1 /Users/user/esp/CECS490/demo_1/build /Users/user/esp/CECS490/demo_1/build /Users/user/esp/CECS490/demo_1/build/CMakeFiles/dfu.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dfu.dir/depend

