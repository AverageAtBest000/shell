#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute(char ***argv) {
  pid_t pid = fork();

  if (pid == -1) {
    perror("Fork function fail");
    return;
  }

  if (pid == 0) {
    int val = execvp((*argv)[0], *argv);
    printf("exec reachhed");
    fflush(stdout);
    if (val == -1) {

      // Remove for debug perror("Command fail");
      fprintf(stderr, "Command fail: Command is the following : %s: %s\n",
              (*argv)[0], strerror(errno));
      _exit(1);
      return;
    }
  } else {
    waitpid(pid, NULL, 0);
  }
}
