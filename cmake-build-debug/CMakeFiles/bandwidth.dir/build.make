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
include CMakeFiles/bandwidth.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bandwidth.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bandwidth.dir/flags.make

CMakeFiles/bandwidth.dir/bandwidth.cpp.o: CMakeFiles/bandwidth.dir/flags.make
CMakeFiles/bandwidth.dir/bandwidth.cpp.o: ../bandwidth.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangke/CLionProjects/Optane/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bandwidth.dir/bandwidth.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bandwidth.dir/bandwidth.cpp.o -c /Users/wangke/CLionProjects/Optane/bandwidth.cpp

CMakeFiles/bandwidth.dir/bandwidth.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bandwidth.dir/bandwidth.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wangke/CLionProjects/Optane/bandwidth.cpp > CMakeFiles/bandwidth.dir/bandwidth.cpp.i

CMakeFiles/bandwidth.dir/bandwidth.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bandwidth.dir/bandwidth.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wangke/CLionProjects/Optane/bandwidth.cpp -o CMakeFiles/bandwidth.dir/bandwidth.cpp.s

# Object files for target bandwidth
bandwidth_OBJECTS = \
"CMakeFiles/bandwidth.dir/bandwidth.cpp.o"

# External object files for target bandwidth
bandwidth_EXTERNAL_OBJECTS =

bandwidth: CMakeFiles/bandwidth.dir/bandwidth.cpp.o
bandwidth: CMakeFiles/bandwidth.dir/build.make
bandwidth: CMakeFiles/bandwidth.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wangke/CLionProjects/Optane/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bandwidth"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bandwidth.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bandwidth.dir/build: bandwidth

.PHONY : CMakeFiles/bandwidth.dir/build

CMakeFiles/bandwidth.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bandwidth.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bandwidth.dir/clean

CMakeFiles/bandwidth.dir/depend:
	cd /Users/wangke/CLionProjects/Optane/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wangke/CLionProjects/Optane /Users/wangke/CLionProjects/Optane /Users/wangke/CLionProjects/Optane/cmake-build-debug /Users/wangke/CLionProjects/Optane/cmake-build-debug /Users/wangke/CLionProjects/Optane/cmake-build-debug/CMakeFiles/bandwidth.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bandwidth.dir/depend

