#pragma once
#include "ds_string.h"
#include <sys/types.h>

struct ds_string;

int handle_backspace(ds_string cmd);
int handle_tab(ds_string *cmd);
