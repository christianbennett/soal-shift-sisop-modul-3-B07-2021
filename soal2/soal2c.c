#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int pipe_A[2];
    int pipe_B[2];
    pid_t pid_A, pid_B, pid_C;

    pipe(pipe_A);

    if (!(pid_A = fork()))
    {
        close(pipe_A[0]); // A-read not needed here

        close(1);
        dup(pipe_A[1]);
        close(pipe_A[1]); //do not pass A-write twice

        execlp("/bin/ps", "ps", "aux", NULL);
    }

    close(pipe_A[1]); // A-write not needed anymore

    pipe(pipe_B); //do not create this pipe until needed

    if (!(pid_B = fork()))
    {
        close(pipe_B[0]); // B-read not needed here

        close(0);
        dup(pipe_A[0]);
        close(pipe_A[0]); //do not pass A-read twice

        close(1);
        dup(pipe_B[1]);
        close(pipe_B[1]); //do not pass B-write twice

        execlp("/usr/bin/sort", "sort", "-nrk", "3,3", NULL);
    }
    close(pipe_A[0]); // A-read not needed anymore
    close(pipe_B[1]); // B-write not needed anymore

    if (!(pid_C = fork()))
    {

        close(0);
        dup(pipe_B[0]);
        close(pipe_B[0]); // do not pass B-read twice

        execlp("/usr/bin/head", "head", "-5", NULL);
    }
    close(pipe_B[0]); // B-read not needed anymore
    return 0;
}