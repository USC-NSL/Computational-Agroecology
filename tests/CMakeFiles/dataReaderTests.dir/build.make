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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests

# Include any dependencies generated for this target.
include CMakeFiles/dataReaderTests.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dataReaderTests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dataReaderTests.dir/flags.make

CMakeFiles/dataReaderTests.dir/dataReader.cpp.o: CMakeFiles/dataReaderTests.dir/flags.make
CMakeFiles/dataReaderTests.dir/dataReader.cpp.o: dataReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dataReaderTests.dir/dataReader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dataReaderTests.dir/dataReader.cpp.o -c /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests/dataReader.cpp

CMakeFiles/dataReaderTests.dir/dataReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dataReaderTests.dir/dataReader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests/dataReader.cpp > CMakeFiles/dataReaderTests.dir/dataReader.cpp.i

CMakeFiles/dataReaderTests.dir/dataReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dataReaderTests.dir/dataReader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests/dataReader.cpp -o CMakeFiles/dataReaderTests.dir/dataReader.cpp.s

# Object files for target dataReaderTests
dataReaderTests_OBJECTS = \
"CMakeFiles/dataReaderTests.dir/dataReader.cpp.o"

# External object files for target dataReaderTests
dataReaderTests_EXTERNAL_OBJECTS =

dataReaderTests: CMakeFiles/dataReaderTests.dir/dataReader.cpp.o
dataReaderTests: CMakeFiles/dataReaderTests.dir/build.make
dataReaderTests: /usr/local/lib/libgtest.a
dataReaderTests: CMakeFiles/dataReaderTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable dataReaderTests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dataReaderTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dataReaderTests.dir/build: dataReaderTests

.PHONY : CMakeFiles/dataReaderTests.dir/build

CMakeFiles/dataReaderTests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dataReaderTests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dataReaderTests.dir/clean

CMakeFiles/dataReaderTests.dir/depend:
	cd /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests /Users/hangjiezheng/Desktop/GitHub/Computational-Agroecology/tests/CMakeFiles/dataReaderTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dataReaderTests.dir/depend

