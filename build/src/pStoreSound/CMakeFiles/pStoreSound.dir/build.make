# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ual/moos-ivp-logan

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ual/moos-ivp-logan/build

# Include any dependencies generated for this target.
include src/pStoreSound/CMakeFiles/pStoreSound.dir/depend.make

# Include the progress variables for this target.
include src/pStoreSound/CMakeFiles/pStoreSound.dir/progress.make

# Include the compile flags for this target's objects.
include src/pStoreSound/CMakeFiles/pStoreSound.dir/flags.make

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o: src/pStoreSound/CMakeFiles/pStoreSound.dir/flags.make
src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o: ../src/pStoreSound/StoreSound.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ual/moos-ivp-logan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pStoreSound.dir/StoreSound.cpp.o -c /home/ual/moos-ivp-logan/src/pStoreSound/StoreSound.cpp

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pStoreSound.dir/StoreSound.cpp.i"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ual/moos-ivp-logan/src/pStoreSound/StoreSound.cpp > CMakeFiles/pStoreSound.dir/StoreSound.cpp.i

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pStoreSound.dir/StoreSound.cpp.s"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ual/moos-ivp-logan/src/pStoreSound/StoreSound.cpp -o CMakeFiles/pStoreSound.dir/StoreSound.cpp.s

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o.requires:

.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o.requires

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o.provides: src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o.requires
	$(MAKE) -f src/pStoreSound/CMakeFiles/pStoreSound.dir/build.make src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o.provides.build
.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o.provides

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o.provides.build: src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o


src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o: src/pStoreSound/CMakeFiles/pStoreSound.dir/flags.make
src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o: ../src/pStoreSound/StoreSound_Info.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ual/moos-ivp-logan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o -c /home/ual/moos-ivp-logan/src/pStoreSound/StoreSound_Info.cpp

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.i"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ual/moos-ivp-logan/src/pStoreSound/StoreSound_Info.cpp > CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.i

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.s"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ual/moos-ivp-logan/src/pStoreSound/StoreSound_Info.cpp -o CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.s

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o.requires:

.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o.requires

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o.provides: src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o.requires
	$(MAKE) -f src/pStoreSound/CMakeFiles/pStoreSound.dir/build.make src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o.provides.build
.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o.provides

src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o.provides.build: src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o


src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o: src/pStoreSound/CMakeFiles/pStoreSound.dir/flags.make
src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o: ../src/pStoreSound/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ual/moos-ivp-logan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pStoreSound.dir/main.cpp.o -c /home/ual/moos-ivp-logan/src/pStoreSound/main.cpp

src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pStoreSound.dir/main.cpp.i"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ual/moos-ivp-logan/src/pStoreSound/main.cpp > CMakeFiles/pStoreSound.dir/main.cpp.i

src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pStoreSound.dir/main.cpp.s"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ual/moos-ivp-logan/src/pStoreSound/main.cpp -o CMakeFiles/pStoreSound.dir/main.cpp.s

src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o.requires:

.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o.requires

src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o.provides: src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o.requires
	$(MAKE) -f src/pStoreSound/CMakeFiles/pStoreSound.dir/build.make src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o.provides.build
.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o.provides

src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o.provides.build: src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o


# Object files for target pStoreSound
pStoreSound_OBJECTS = \
"CMakeFiles/pStoreSound.dir/StoreSound.cpp.o" \
"CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o" \
"CMakeFiles/pStoreSound.dir/main.cpp.o"

# External object files for target pStoreSound
pStoreSound_EXTERNAL_OBJECTS =

../bin/pStoreSound: src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o
../bin/pStoreSound: src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o
../bin/pStoreSound: src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o
../bin/pStoreSound: src/pStoreSound/CMakeFiles/pStoreSound.dir/build.make
../bin/pStoreSound: /home/ual/moos-ivp/build/MOOS/MOOSCore/lib/libMOOS.a
../bin/pStoreSound: src/pStoreSound/CMakeFiles/pStoreSound.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ual/moos-ivp-logan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../../bin/pStoreSound"
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pStoreSound.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/pStoreSound/CMakeFiles/pStoreSound.dir/build: ../bin/pStoreSound

.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/build

src/pStoreSound/CMakeFiles/pStoreSound.dir/requires: src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound.cpp.o.requires
src/pStoreSound/CMakeFiles/pStoreSound.dir/requires: src/pStoreSound/CMakeFiles/pStoreSound.dir/StoreSound_Info.cpp.o.requires
src/pStoreSound/CMakeFiles/pStoreSound.dir/requires: src/pStoreSound/CMakeFiles/pStoreSound.dir/main.cpp.o.requires

.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/requires

src/pStoreSound/CMakeFiles/pStoreSound.dir/clean:
	cd /home/ual/moos-ivp-logan/build/src/pStoreSound && $(CMAKE_COMMAND) -P CMakeFiles/pStoreSound.dir/cmake_clean.cmake
.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/clean

src/pStoreSound/CMakeFiles/pStoreSound.dir/depend:
	cd /home/ual/moos-ivp-logan/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ual/moos-ivp-logan /home/ual/moos-ivp-logan/src/pStoreSound /home/ual/moos-ivp-logan/build /home/ual/moos-ivp-logan/build/src/pStoreSound /home/ual/moos-ivp-logan/build/src/pStoreSound/CMakeFiles/pStoreSound.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/pStoreSound/CMakeFiles/pStoreSound.dir/depend

