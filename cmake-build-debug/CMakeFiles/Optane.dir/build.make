# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/wangke/CLionProjects/Optane

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/wangke/CLionProjects/Optane/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Optane.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Optane.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Optane.dir/flags.make

CMakeFiles/Optane.dir/main.cpp.o: CMakeFiles/Optane.dir/flags.make
CMakeFiles/Optane.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangke/CLionProjects/Optane/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Optane.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Optane.dir/main.cpp.o -c /Users/wangke/CLionProjects/Optane/main.cpp

CMakeFiles/Optane.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Optane.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wangke/CLionProjects/Optane/main.cpp > CMakeFiles/Optane.dir/main.cpp.i

CMakeFiles/Optane.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Optane.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wangke/CLionProjects/Optane/main.cpp -o CMakeFiles/Optane.dir/main.cpp.s

CMakeFiles/Optane.dir/reader.cpp.o: CMakeFiles/Optane.dir/flags.make
CMakeFiles/Optane.dir/reader.cpp.o: ../reader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangke/CLionProjects/Optane/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Optane.dir/reader.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Optane.dir/reader.cpp.o -c /Users/wangke/CLionProjects/Optane/reader.cpp

CMakeFiles/Optane.dir/reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Optane.dir/reader.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wangke/CLionProjects/Optane/reader.cpp > CMakeFiles/Optane.dir/reader.cpp.i

CMakeFiles/Optane.dir/reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Optane.dir/reader.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wangke/CLionProjects/Optane/reader.cpp -o CMakeFiles/Optane.dir/reader.cpp.s

# Object files for target Optane
Optane_OBJECTS = \
"CMakeFiles/Optane.dir/main.cpp.o" \
"CMakeFiles/Optane.dir/reader.cpp.o"

# External object files for target Optane
Optane_EXTERNAL_OBJECTS =

Optane: CMakeFiles/Optane.dir/main.cpp.o
Optane: CMakeFiles/Optane.dir/reader.cpp.o
Optane: CMakeFiles/Optane.dir/build.make
Optane: CMakeFiles/Optane.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wangke/CLionProjects/Optane/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Optane"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Optane.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Optane.dir/build: Optane

.PHONY : CMakeFiles/Optane.dir/build

CMakeFiles/Optane.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Optane.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Optane.dir/clean

CMakeFiles/Optane.dir/depend:
	cd /Users/wangke/CLionProjects/Optane/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wangke/CLionProjects/Optane /Users/wangke/CLionProjects/Optane /Users/wangke/CLionProjects/Optane/cmake-build-debug /Users/wangke/CLionProjects/Optane/cmake-build-debug /Users/wangke/CLionProjects/Optane/cmake-build-debug/CMakeFiles/Optane.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Optane.dir/depend
