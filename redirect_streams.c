#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

void redir(const char *inp, const char *cmd, const char *out) {
    pid_t pid;
    char *args[5], *cmd_copy = strdup(cmd), *token = strtok(cmd_copy, " ");
    int fd_in, fd_out, i=0;
    fd_in = open(inp, O_RDONLY);
    fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC);
    if (strcmp(inp, "-") == 0) {
        fd_in = STDIN_FILENO;
    }
    if (strcmp(out, "-") == 0) {
        fd_out = STDOUT_FILENO;
    }
    pid = fork();
    if (pid == 0) {
        dup2(fd_in, STDIN_FILENO);
        dup2(fd_out, STDOUT_FILENO);
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        execvp(args[0], args);
    } else {
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    redir(argv[1], argv[2], argv[3]);
    return 0;
}