# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.2.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.2.4\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\InformaticaGraficaSesiones\Sesiones\P3_

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\InformaticaGraficaSesiones\Sesiones\P3_\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/P3_.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/P3_.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/P3_.dir/flags.make

CMakeFiles/P3_.dir/main.cpp.obj: CMakeFiles/P3_.dir/flags.make
CMakeFiles/P3_.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\InformaticaGraficaSesiones\Sesiones\P3_\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/P3_.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\P3_.dir\main.cpp.obj -c C:\InformaticaGraficaSesiones\Sesiones\P3_\main.cpp

CMakeFiles/P3_.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/P3_.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\InformaticaGraficaSesiones\Sesiones\P3_\main.cpp > CMakeFiles\P3_.dir\main.cpp.i

CMakeFiles/P3_.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/P3_.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\InformaticaGraficaSesiones\Sesiones\P3_\main.cpp -o CMakeFiles\P3_.dir\main.cpp.s

# Object files for target P3_
P3__OBJECTS = \
"CMakeFiles/P3_.dir/main.cpp.obj"

# External object files for target P3_
P3__EXTERNAL_OBJECTS =

P3_.exe: CMakeFiles/P3_.dir/main.cpp.obj
P3_.exe: CMakeFiles/P3_.dir/build.make
P3_.exe: CMakeFiles/P3_.dir/linklibs.rsp
P3_.exe: CMakeFiles/P3_.dir/objects1.rsp
P3_.exe: CMakeFiles/P3_.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\InformaticaGraficaSesiones\Sesiones\P3_\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable P3_.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\P3_.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/P3_.dir/build: P3_.exe

.PHONY : CMakeFiles/P3_.dir/build

CMakeFiles/P3_.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\P3_.dir\cmake_clean.cmake
.PHONY : CMakeFiles/P3_.dir/clean

CMakeFiles/P3_.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\InformaticaGraficaSesiones\Sesiones\P3_ C:\InformaticaGraficaSesiones\Sesiones\P3_ C:\InformaticaGraficaSesiones\Sesiones\P3_\cmake-build-debug C:\InformaticaGraficaSesiones\Sesiones\P3_\cmake-build-debug C:\InformaticaGraficaSesiones\Sesiones\P3_\cmake-build-debug\CMakeFiles\P3_.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/P3_.dir/depend
