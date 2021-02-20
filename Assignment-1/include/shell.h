#ifndef SHELL_H
#define SHELL_H

#define COMMAND_LENGTH 512
#define DELIMITER " \t\n\a\r"
#define NO_OF_ARGUMENTS 16

void launch_shell();

char * get_command();

char ** get_arguments(char * command);

char ** join_spaces(char ** arguments);

int determine_command_type(char ** arguments);

int change_directory(char ** arguments);

int current_working_directory(char ** arguments);

int exit_shell();

int execute(char ** arguments);

int parse_command(char ** arguments);

int cat(char ** arguments);

int grep(char ** arguments);

int list_directory(char ** arguments);

int make_directory(char ** arguments);

int change_mode(char ** arguments);

int remove_f(char ** arguments);

void remove_filetree(const char * filepath);

int copy(char ** arguments);

int move(char ** arguments);

#endif