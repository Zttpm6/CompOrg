# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion 2018.3.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion 2018.3.3\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/main2.c.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main2.c.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main2.c.dir/flags.make

CMakeFiles/main2.c.dir/Assembler.c.obj: CMakeFiles/main2.c.dir/flags.make
CMakeFiles/main2.c.dir/Assembler.c.obj: ../Assembler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main2.c.dir/Assembler.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\main2.c.dir\Assembler.c.obj   -c "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\Assembler.c"

CMakeFiles/main2.c.dir/Assembler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main2.c.dir/Assembler.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\Assembler.c" > CMakeFiles\main2.c.dir\Assembler.c.i

CMakeFiles/main2.c.dir/Assembler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main2.c.dir/Assembler.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\Assembler.c" -o CMakeFiles\main2.c.dir\Assembler.c.s

# Object files for target main2.c
main2_c_OBJECTS = \
"CMakeFiles/main2.c.dir/Assembler.c.obj"

# External object files for target main2.c
main2_c_EXTERNAL_OBJECTS =

main2.c.exe: CMakeFiles/main2.c.dir/Assembler.c.obj
main2.c.exe: CMakeFiles/main2.c.dir/build.make
main2.c.exe: CMakeFiles/main2.c.dir/linklibs.rsp
main2.c.exe: CMakeFiles/main2.c.dir/objects1.rsp
main2.c.exe: CMakeFiles/main2.c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable main2.c.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\main2.c.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main2.c.dir/build: main2.c.exe

.PHONY : CMakeFiles/main2.c.dir/build

CMakeFiles/main2.c.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\main2.c.dir\cmake_clean.cmake
.PHONY : CMakeFiles/main2.c.dir/clean

CMakeFiles/main2.c.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2" "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2" "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\cmake-build-debug" "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\cmake-build-debug" "C:\Users\Zach Taylor\Desktop\CompOrg\Lab2\cmake-build-debug\CMakeFiles\main2.c.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/main2.c.dir/depend

