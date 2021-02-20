
# Assignment1

## Compiling  

Open a terminal in the src directory, create an empty folder named obj and run 'make'

`cd src`    
`mkdir obj`    
`make`   

## Running compiled binary

Open a terminal in the src directory and run './shell'

`cd src`    
`./shell`

## Cleaning 

Open a terminal in the src directory and run 'make clean'

`cd src`    
`make clean`

## Description of folders

### include

Contains the header file shell.h

### src

Contains all .c files, the Makefile and the compiled binary named shell.

### src/obj

Contains all object files.


## Description of files

### include/shell.h

Header file I created.

### src/shell

Compiled binary of the shell

### src/shell.c

Main C file, with the main() function.

### src/cat.c

Implementation of cat as a function.

### src/change_directory.c

Implementation of cd as a function.

### src/change_mode.c

Implementation of chmod as a function.

### src/copy.c

Implementation of cp as a function.

### src/current_working _directory.c

Implementation of pwd as a function.

### src/determine_command_type.c

Implements a function to decide which command to execute.

### src/execute.c

Implements a function to run in-built binaries

### src/exit_shell.c

Implements a function to exit the shell.

### src/get_arguments.c

Implements a function to split the input command read into arguments.

### src/get_command.c

Implements a function to read the command entered by the user.

### src/grep.c

Implementation of grep as a function.

### src/join_spaces.c

Implements a function used to combine arguments ending with \ and the next argument with a space in between.

### src/list_directory.c

Implementation of ls as a function.

### src/make_directory.c

Implementation of mkdir as a function.

### src/Makefile

The Makefile used for compiling.

### src/move.c

Implementation of mv as a function.

### src/parse_command.c

Implements a function to parse the arguments to check for & (background process) and to execute the command in the background if needed.

### src/remove_filetree.c

Implements a recursive function that deletes a directory. This function is used by the implementation of rm.

### src/remove.c

Implementation of rm as a function.
