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
CMAKE_SOURCE_DIR = /Users/user/esp/CECS490/led_blinky

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/user/esp/CECS490/led_blinky/build

# Utility rule file for bootloader.

# Include any custom commands dependencies for this target.
include CMakeFiles/bootloader.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/bootloader.dir/progress.make

CMakeFiles/bootloader: CMakeFiles/bootloader-complete

CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-install
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-mkdir
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-download
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-update
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-patch
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-configure
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-build
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'bootloader'"
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/user/esp/CECS490/led_blinky/build/CMakeFiles
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/user/esp/CECS490/led_blinky/build/CMakeFiles/bootloader-complete
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp/bootloader-done

bootloader-prefix/src/bootloader-stamp/bootloader-build: bootloader-prefix/src/bootloader-stamp/bootloader-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'bootloader'"
	cd /Users/user/esp/CECS490/led_blinky/build/bootloader && $(MAKE)

bootloader-prefix/src/bootloader-stamp/bootloader-configure: bootloader-prefix/tmp/bootloader-cfgcmd.txt
bootloader-prefix/src/bootloader-stamp/bootloader-configure: bootloader-prefix/src/bootloader-stamp/bootloader-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'bootloader'"
	cd /Users/user/esp/CECS490/led_blinky/build/bootloader && /Applications/CMake.app/Contents/bin/cmake -DSDKCONFIG=/Users/user/esp/CECS490/led_blinky/sdkconfig -DIDF_PATH=/Users/user/esp/esp-idf -DIDF_TARGET=esp32s2 -DPYTHON_DEPS_CHECKED=1 -DPYTHON=/Users/user/.espressif/python_env/idf5.0_py3.9_env/bin/python -DEXTRA_COMPONENT_DIRS=/Users/user/esp/esp-idf/components/bootloader -DPROJECT_SOURCE_DIR=/Users/user/esp/CECS490/led_blinky "-GUnix Makefiles" /Users/user/esp/esp-idf/components/bootloader/subproject
	cd /Users/user/esp/CECS490/led_blinky/build/bootloader && /Applications/CMake.app/Contents/bin/cmake -E touch /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp/bootloader-configure

bootloader-prefix/src/bootloader-stamp/bootloader-download: bootloader-prefix/src/bootloader-stamp/bootloader-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'bootloader'"
	/Applications/CMake.app/Contents/bin/cmake -E echo_append
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp/bootloader-download

bootloader-prefix/src/bootloader-stamp/bootloader-install: bootloader-prefix/src/bootloader-stamp/bootloader-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'bootloader'"
	cd /Users/user/esp/CECS490/led_blinky/build/bootloader && /Applications/CMake.app/Contents/bin/cmake -E echo_append
	cd /Users/user/esp/CECS490/led_blinky/build/bootloader && /Applications/CMake.app/Contents/bin/cmake -E touch /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp/bootloader-install

bootloader-prefix/src/bootloader-stamp/bootloader-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'bootloader'"
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/user/esp/esp-idf/components/bootloader/subproject
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/user/esp/CECS490/led_blinky/build/bootloader
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/tmp
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp/bootloader-mkdir

bootloader-prefix/src/bootloader-stamp/bootloader-patch: bootloader-prefix/src/bootloader-stamp/bootloader-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'bootloader'"
	/Applications/CMake.app/Contents/bin/cmake -E echo_append
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp/bootloader-patch

bootloader-prefix/src/bootloader-stamp/bootloader-update: bootloader-prefix/src/bootloader-stamp/bootloader-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No update step for 'bootloader'"
	/Applications/CMake.app/Contents/bin/cmake -E echo_append
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/user/esp/CECS490/led_blinky/build/bootloader-prefix/src/bootloader-stamp/bootloader-update

bootloader: CMakeFiles/bootloader
bootloader: CMakeFiles/bootloader-complete
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-build
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-configure
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-download
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-install
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-mkdir
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-patch
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-update
bootloader: CMakeFiles/bootloader.dir/build.make
.PHONY : bootloader

# Rule to build all files generated by this target.
CMakeFiles/bootloader.dir/build: bootloader
.PHONY : CMakeFiles/bootloader.dir/build

CMakeFiles/bootloader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bootloader.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bootloader.dir/clean

CMakeFiles/bootloader.dir/depend:
	cd /Users/user/esp/CECS490/led_blinky/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/user/esp/CECS490/led_blinky /Users/user/esp/CECS490/led_blinky /Users/user/esp/CECS490/led_blinky/build /Users/user/esp/CECS490/led_blinky/build /Users/user/esp/CECS490/led_blinky/build/CMakeFiles/bootloader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bootloader.dir/depend

