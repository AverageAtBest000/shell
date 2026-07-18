#ifndef UTILS_H
#define UTILS_H

void reset(int* argc, char** cmd, char*** argv);
void tokenize(int* argc, char*** argv, char** cmd, char* delim);
void getcmd(char** cmd);

#endif