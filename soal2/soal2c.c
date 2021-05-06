#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int pid;
int pipe1[2];
int pipe2[2];

void exec1()
{
    // input from stdin (already done)
    // output to pipe1
    dup2(pipe1[1], 1);
    // close fds
    close(pipe1[0]);
    close(pipe1[1]);
    // exec
    // char *argv[] = {"ps", "-aux", NULL};
    // execv("/bin/ps", argv);
    char *argv[] = {"ls", NULL};
    execv("/bin/ls", argv);
}

void exec2()
{
    // input from pipe1
    dup2(pipe1[0], 0);
    // output to pipe2
    dup2(pipe2[1], 1);
    // close fds
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    // exec
    char *argv[] = {"sort", "-nrk", "3,3", NULL};
    execv("/bin/sort", argv);
}

void exec3()
{
    // input from pipe2
    dup2(pipe2[0], 0);
    // output to stdout (already done)
    // close fds
    close(pipe2[0]);
    close(pipe2[1]);
    // exec
    char *argv[] = {"head", "-5", NULL};
    execv("/bin/head", argv);
}

int main()
{
    if (pipe(pipe1) == -1)
    {
        perror("bad pipe1");
        exit(1);
    }

    if (pipe(pipe2) == -1)
    {
        perror("bad pipe2");
        exit(1);
    }

    if (pid = fork() == 0)
    {
        exec1();
    }

    if (pid = fork() == 0)
    {
        exec2();
    }

    if (pid = fork() == 0)
    {
        exec3();
    }
}
