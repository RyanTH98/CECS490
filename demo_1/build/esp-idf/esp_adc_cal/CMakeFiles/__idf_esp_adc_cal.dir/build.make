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

# Include any dependencies generated for this target.
include esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/compiler_depend.make

# Include the progress variables for this target.
include esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/progress.make

# Include the compile flags for this target's objects.
include esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/flags.make

esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj: esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/flags.make
esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj: /Users/user/esp/esp-idf/components/esp_adc_cal/esp_adc_cal_common.c
esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj: esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj"
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && /Users/user/.espressif/tools/xtensa-esp32s2-elf/esp-2021r2-8.4.0/xtensa-esp32s2-elf/bin/xtensa-esp32s2-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj -MF CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj.d -o CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj -c /Users/user/esp/esp-idf/components/esp_adc_cal/esp_adc_cal_common.c

esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.i"
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && /Users/user/.espressif/tools/xtensa-esp32s2-elf/esp-2021r2-8.4.0/xtensa-esp32s2-elf/bin/xtensa-esp32s2-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/user/esp/esp-idf/components/esp_adc_cal/esp_adc_cal_common.c > CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.i

esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.s"
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && /Users/user/.espressif/tools/xtensa-esp32s2-elf/esp-2021r2-8.4.0/xtensa-esp32s2-elf/bin/xtensa-esp32s2-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/user/esp/esp-idf/components/esp_adc_cal/esp_adc_cal_common.c -o CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.s

esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj: esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/flags.make
esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj: /Users/user/esp/esp-idf/components/esp_adc_cal/esp_adc_cal_esp32s2.c
esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj: esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj"
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && /Users/user/.espressif/tools/xtensa-esp32s2-elf/esp-2021r2-8.4.0/xtensa-esp32s2-elf/bin/xtensa-esp32s2-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj -MF CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj.d -o CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj -c /Users/user/esp/esp-idf/components/esp_adc_cal/esp_adc_cal_esp32s2.c

esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.i"
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && /Users/user/.espressif/tools/xtensa-esp32s2-elf/esp-2021r2-8.4.0/xtensa-esp32s2-elf/bin/xtensa-esp32s2-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/user/esp/esp-idf/components/esp_adc_cal/esp_adc_cal_esp32s2.c > CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.i

esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.s"
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && /Users/user/.espressif/tools/xtensa-esp32s2-elf/esp-2021r2-8.4.0/xtensa-esp32s2-elf/bin/xtensa-esp32s2-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/user/esp/esp-idf/components/esp_adc_cal/esp_adc_cal_esp32s2.c -o CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.s

# Object files for target __idf_esp_adc_cal
__idf_esp_adc_cal_OBJECTS = \
"CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj" \
"CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj"

# External object files for target __idf_esp_adc_cal
__idf_esp_adc_cal_EXTERNAL_OBJECTS =

esp-idf/esp_adc_cal/libesp_adc_cal.a: esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_common.c.obj
esp-idf/esp_adc_cal/libesp_adc_cal.a: esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/esp_adc_cal_esp32s2.c.obj
esp-idf/esp_adc_cal/libesp_adc_cal.a: esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/build.make
esp-idf/esp_adc_cal/libesp_adc_cal.a: esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/user/esp/CECS490/led_blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libesp_adc_cal.a"
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && $(CMAKE_COMMAND) -P CMakeFiles/__idf_esp_adc_cal.dir/cmake_clean_target.cmake
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/__idf_esp_adc_cal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/build: esp-idf/esp_adc_cal/libesp_adc_cal.a
.PHONY : esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/build

esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/clean:
	cd /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal && $(CMAKE_COMMAND) -P CMakeFiles/__idf_esp_adc_cal.dir/cmake_clean.cmake
.PHONY : esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/clean

esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/depend:
	cd /Users/user/esp/CECS490/led_blinky/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/user/esp/CECS490/led_blinky /Users/user/esp/esp-idf/components/esp_adc_cal /Users/user/esp/CECS490/led_blinky/build /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal /Users/user/esp/CECS490/led_blinky/build/esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/esp_adc_cal/CMakeFiles/__idf_esp_adc_cal.dir/depend

