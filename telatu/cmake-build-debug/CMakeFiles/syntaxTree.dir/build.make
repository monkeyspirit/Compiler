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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.1.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.1.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Claudio\Projects\C\CLionProjects\Compilatore

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/syntaxTree.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/syntaxTree.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/syntaxTree.dir/flags.make

CMakeFiles/syntaxTree.dir/lexer.c.obj: CMakeFiles/syntaxTree.dir/flags.make
CMakeFiles/syntaxTree.dir/lexer.c.obj: ../lexer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/syntaxTree.dir/lexer.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\syntaxTree.dir\lexer.c.obj   -c C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\lexer.c

CMakeFiles/syntaxTree.dir/lexer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/syntaxTree.dir/lexer.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\lexer.c > CMakeFiles\syntaxTree.dir\lexer.c.i

CMakeFiles/syntaxTree.dir/lexer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/syntaxTree.dir/lexer.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\lexer.c -o CMakeFiles\syntaxTree.dir\lexer.c.s

CMakeFiles/syntaxTree.dir/parser.c.obj: CMakeFiles/syntaxTree.dir/flags.make
CMakeFiles/syntaxTree.dir/parser.c.obj: ../parser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/syntaxTree.dir/parser.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\syntaxTree.dir\parser.c.obj   -c C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\parser.c

CMakeFiles/syntaxTree.dir/parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/syntaxTree.dir/parser.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\parser.c > CMakeFiles\syntaxTree.dir\parser.c.i

CMakeFiles/syntaxTree.dir/parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/syntaxTree.dir/parser.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\parser.c -o CMakeFiles\syntaxTree.dir\parser.c.s

CMakeFiles/syntaxTree.dir/syntaxtree.c.obj: CMakeFiles/syntaxTree.dir/flags.make
CMakeFiles/syntaxTree.dir/syntaxtree.c.obj: ../syntaxtree.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/syntaxTree.dir/syntaxtree.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\syntaxTree.dir\syntaxtree.c.obj   -c C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\syntaxtree.c

CMakeFiles/syntaxTree.dir/syntaxtree.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/syntaxTree.dir/syntaxtree.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\syntaxtree.c > CMakeFiles\syntaxTree.dir\syntaxtree.c.i

CMakeFiles/syntaxTree.dir/syntaxtree.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/syntaxTree.dir/syntaxtree.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\syntaxtree.c -o CMakeFiles\syntaxTree.dir\syntaxtree.c.s

CMakeFiles/syntaxTree.dir/symboltable.c.obj: CMakeFiles/syntaxTree.dir/flags.make
CMakeFiles/syntaxTree.dir/symboltable.c.obj: ../symboltable.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/syntaxTree.dir/symboltable.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\syntaxTree.dir\symboltable.c.obj   -c C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\symboltable.c

CMakeFiles/syntaxTree.dir/symboltable.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/syntaxTree.dir/symboltable.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\symboltable.c > CMakeFiles\syntaxTree.dir\symboltable.c.i

CMakeFiles/syntaxTree.dir/symboltable.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/syntaxTree.dir/symboltable.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\symboltable.c -o CMakeFiles\syntaxTree.dir\symboltable.c.s

CMakeFiles/syntaxTree.dir/main.c.obj: CMakeFiles/syntaxTree.dir/flags.make
CMakeFiles/syntaxTree.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/syntaxTree.dir/main.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\syntaxTree.dir\main.c.obj   -c C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\main.c

CMakeFiles/syntaxTree.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/syntaxTree.dir/main.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\main.c > CMakeFiles\syntaxTree.dir\main.c.i

CMakeFiles/syntaxTree.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/syntaxTree.dir/main.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\main.c -o CMakeFiles\syntaxTree.dir\main.c.s

# Object files for target syntaxTree
syntaxTree_OBJECTS = \
"CMakeFiles/syntaxTree.dir/lexer.c.obj" \
"CMakeFiles/syntaxTree.dir/parser.c.obj" \
"CMakeFiles/syntaxTree.dir/syntaxtree.c.obj" \
"CMakeFiles/syntaxTree.dir/symboltable.c.obj" \
"CMakeFiles/syntaxTree.dir/main.c.obj"

# External object files for target syntaxTree
syntaxTree_EXTERNAL_OBJECTS =

syntaxTree.exe: CMakeFiles/syntaxTree.dir/lexer.c.obj
syntaxTree.exe: CMakeFiles/syntaxTree.dir/parser.c.obj
syntaxTree.exe: CMakeFiles/syntaxTree.dir/syntaxtree.c.obj
syntaxTree.exe: CMakeFiles/syntaxTree.dir/symboltable.c.obj
syntaxTree.exe: CMakeFiles/syntaxTree.dir/main.c.obj
syntaxTree.exe: CMakeFiles/syntaxTree.dir/build.make
syntaxTree.exe: CMakeFiles/syntaxTree.dir/linklibs.rsp
syntaxTree.exe: CMakeFiles/syntaxTree.dir/objects1.rsp
syntaxTree.exe: CMakeFiles/syntaxTree.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable syntaxTree.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\syntaxTree.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/syntaxTree.dir/build: syntaxTree.exe

.PHONY : CMakeFiles/syntaxTree.dir/build

CMakeFiles/syntaxTree.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\syntaxTree.dir\cmake_clean.cmake
.PHONY : CMakeFiles/syntaxTree.dir/clean

CMakeFiles/syntaxTree.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Claudio\Projects\C\CLionProjects\Compilatore C:\Users\Claudio\Projects\C\CLionProjects\Compilatore C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug C:\Users\Claudio\Projects\C\CLionProjects\Compilatore\cmake-build-debug\CMakeFiles\syntaxTree.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/syntaxTree.dir/depend
