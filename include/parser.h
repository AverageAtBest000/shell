#pragma once
#include "ds_string.h"
struct ds_string;

int tokenize(int *argc, char ***argv, ds_string *cmd, char delim);
