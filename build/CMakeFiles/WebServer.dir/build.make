# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/ss/code/WebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ss/code/WebServer/build

# Include any dependencies generated for this target.
include CMakeFiles/WebServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/WebServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/WebServer.dir/flags.make

CMakeFiles/WebServer.dir/config.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/config.cpp.o: ../config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ss/code/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/WebServer.dir/config.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WebServer.dir/config.cpp.o -c /home/ss/code/WebServer/config.cpp

CMakeFiles/WebServer.dir/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/config.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ss/code/WebServer/config.cpp > CMakeFiles/WebServer.dir/config.cpp.i

CMakeFiles/WebServer.dir/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/config.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ss/code/WebServer/config.cpp -o CMakeFiles/WebServer.dir/config.cpp.s

CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.o: ../httpconn/httpconn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ss/code/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.o -c /home/ss/code/WebServer/httpconn/httpconn.cpp

CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ss/code/WebServer/httpconn/httpconn.cpp > CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.i

CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ss/code/WebServer/httpconn/httpconn.cpp -o CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.s

CMakeFiles/WebServer.dir/timer/timer.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/timer/timer.cpp.o: ../timer/timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ss/code/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/WebServer.dir/timer/timer.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WebServer.dir/timer/timer.cpp.o -c /home/ss/code/WebServer/timer/timer.cpp

CMakeFiles/WebServer.dir/timer/timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/timer/timer.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ss/code/WebServer/timer/timer.cpp > CMakeFiles/WebServer.dir/timer/timer.cpp.i

CMakeFiles/WebServer.dir/timer/timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/timer/timer.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ss/code/WebServer/timer/timer.cpp -o CMakeFiles/WebServer.dir/timer/timer.cpp.s

CMakeFiles/WebServer.dir/timer/timequeue.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/timer/timequeue.cpp.o: ../timer/timequeue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ss/code/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/WebServer.dir/timer/timequeue.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WebServer.dir/timer/timequeue.cpp.o -c /home/ss/code/WebServer/timer/timequeue.cpp

CMakeFiles/WebServer.dir/timer/timequeue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/timer/timequeue.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ss/code/WebServer/timer/timequeue.cpp > CMakeFiles/WebServer.dir/timer/timequeue.cpp.i

CMakeFiles/WebServer.dir/timer/timequeue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/timer/timequeue.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ss/code/WebServer/timer/timequeue.cpp -o CMakeFiles/WebServer.dir/timer/timequeue.cpp.s

CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.o: ../threadpool/threadpool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ss/code/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.o -c /home/ss/code/WebServer/threadpool/threadpool.cpp

CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ss/code/WebServer/threadpool/threadpool.cpp > CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.i

CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ss/code/WebServer/threadpool/threadpool.cpp -o CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.s

CMakeFiles/WebServer.dir/server.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/server.cpp.o: ../server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ss/code/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/WebServer.dir/server.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WebServer.dir/server.cpp.o -c /home/ss/code/WebServer/server.cpp

CMakeFiles/WebServer.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/server.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ss/code/WebServer/server.cpp > CMakeFiles/WebServer.dir/server.cpp.i

CMakeFiles/WebServer.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/server.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ss/code/WebServer/server.cpp -o CMakeFiles/WebServer.dir/server.cpp.s

CMakeFiles/WebServer.dir/main.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ss/code/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/WebServer.dir/main.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WebServer.dir/main.cpp.o -c /home/ss/code/WebServer/main.cpp

CMakeFiles/WebServer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/main.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ss/code/WebServer/main.cpp > CMakeFiles/WebServer.dir/main.cpp.i

CMakeFiles/WebServer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/main.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ss/code/WebServer/main.cpp -o CMakeFiles/WebServer.dir/main.cpp.s

# Object files for target WebServer
WebServer_OBJECTS = \
"CMakeFiles/WebServer.dir/config.cpp.o" \
"CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.o" \
"CMakeFiles/WebServer.dir/timer/timer.cpp.o" \
"CMakeFiles/WebServer.dir/timer/timequeue.cpp.o" \
"CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.o" \
"CMakeFiles/WebServer.dir/server.cpp.o" \
"CMakeFiles/WebServer.dir/main.cpp.o"

# External object files for target WebServer
WebServer_EXTERNAL_OBJECTS =

WebServer: CMakeFiles/WebServer.dir/config.cpp.o
WebServer: CMakeFiles/WebServer.dir/httpconn/httpconn.cpp.o
WebServer: CMakeFiles/WebServer.dir/timer/timer.cpp.o
WebServer: CMakeFiles/WebServer.dir/timer/timequeue.cpp.o
WebServer: CMakeFiles/WebServer.dir/threadpool/threadpool.cpp.o
WebServer: CMakeFiles/WebServer.dir/server.cpp.o
WebServer: CMakeFiles/WebServer.dir/main.cpp.o
WebServer: CMakeFiles/WebServer.dir/build.make
WebServer: CMakeFiles/WebServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ss/code/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable WebServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/WebServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/WebServer.dir/build: WebServer

.PHONY : CMakeFiles/WebServer.dir/build

CMakeFiles/WebServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/WebServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/WebServer.dir/clean

CMakeFiles/WebServer.dir/depend:
	cd /home/ss/code/WebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ss/code/WebServer /home/ss/code/WebServer /home/ss/code/WebServer/build /home/ss/code/WebServer/build /home/ss/code/WebServer/build/CMakeFiles/WebServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/WebServer.dir/depend

