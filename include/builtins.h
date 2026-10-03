#pragma once

#include <stdbool.h>

bool handleBuiltins(char **argv, int argc);
void cd(char **argv, int argc);
bool is_builtin(char *cmd);
