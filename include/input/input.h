#pragma once
#include <sys/types.h>

ssize_t get_cmd_cannonical(char **cmd);
ssize_t get_cmd_noncannonical(char **cmd);
int delete_from_userin(int num_chars);
void reset(int *argc, char **cmd, char ***argv);
