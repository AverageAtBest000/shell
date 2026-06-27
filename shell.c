#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
/*
*   Work Flow
*   1. get input entered by the user as a string - use buffer
*   2. tokenize the input
*   3. fork()
*   4. execvp()
*/
void getcmd(char**);
void tokenize(int* argc, char*** argv, char** cmd, char* delim);
void reset(int* argc, char** cmd, char*** argv);

int main(void)
{

    char* cmd = NULL;
    int argc = 0;                   
    char** argv;
    char* delim = " ";

    while (1)
    {
        printf(">>> ");
        getcmd(&cmd);        
        tokenize(&argc, &argv, &cmd, delim);
        
        /*printf("%d ",argc);
        
        for(int i = 0 ; i < argc ; i++)
            printf("%s ",argv[i]);

        printf("\n");   
        */
        
        if(strcmp(cmd, "exit") == 0) return 0;
        
        pid_t pid = fork();
                        

        if(pid == -1 ) perror("Fork function fail");

        if(pid == 0 ){
            //printf("%s\n",argv[0]);
            int val = execvp(argv[0], argv);
            if(val == -1) perror("Command fail");
        }else{  
            wait(NULL);
        }


        reset(&argc, &cmd, &argv);
    
    }
    
    return 0;
}

void reset(int* argc, char** cmd, char*** argv)
{
    *argc = 0;

    free(*cmd);
    *cmd = NULL;
    
    free(*argv);
    *argv = NULL;
}

void tokenize(int* argc, char*** argv, char** cmd, char* delim)
{
    char* token;
    /* +1 for the NULL terminator*/
    char* cmd_cpy = malloc(sizeof(char) * strlen(*cmd) + 1);

    strcpy(cmd_cpy , *cmd );
    
    token = strtok(cmd_cpy, delim); 
    *argc = 0;

    while (token)
    {
        (*argc)++;
        token = strtok(NULL, delim);
    }

    *argv = malloc((*argc + 1) * sizeof(char*));
    token = strtok(*cmd, delim);

    for(int i = 0 ; token ; i++)
    {
        (*argv)[i] = token;
        token = strtok(NULL, delim);
    }

    (*argv)[*argc] = NULL;

    free(cmd_cpy);
    //free(token);
}

void getcmd(char** cmd){
    size_t n = 10;
    ssize_t numchar = getline(cmd, &n, stdin);
    //printf("%c\n",(*cmd)[numchar-1]);
    (*cmd)[numchar-1] = '\0';   

}