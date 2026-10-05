#pragma once
#include <sys/types.h>

int handle_backspace(ssize_t *num_char);
int handle_tab(ssize_t *num_char, char **cmd);
