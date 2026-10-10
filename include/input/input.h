#pragma once
#include "ds_string.h"
#include <sys/types.h>

ssize_t get_cmd_cannonical(char **cmd);
ssize_t get_cmd_noncannonical(ds_string *cmd);
int delete_from_userin(int num_chars);
void reset(int *argc, ds_string *cmd, char ***argv);
