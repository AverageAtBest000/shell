#ifndef PARSER_H
#define PARSER_H

void tokenize(int* argc, char*** argv, char** cmd, char* delim);
int count_tokens(int* argc, char* cmd);

#endif