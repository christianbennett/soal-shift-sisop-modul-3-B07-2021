#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int pipe1[2];
    int pipe2[2];

    pipe(pipe1);

    if (fork() == 0)
    {
        //close read dari pipe A
        close(pipe1[0]);
        dup2(pipe1[1], 1);
        //do not pass A-write twice
        close(pipe1[1]);

        execlp("/bin/ps", "ps", "aux", NULL);
    }

    close(pipe1[1]); // A-write not needed anymore

    pipe(pipe2); //do not create this pipe until needed

    if (fork() == 0)
    {
        //close read dari pipe B
        close(pipe2[0]);

        dup2(pipe1[0], 0);
        //do not pass A-read twice
        close(pipe1[0]);
        dup2(pipe2[1], 1);
        //do not pass B-write twice
        close(pipe2[1]);

        execlp("/usr/bin/sort", "sort", "-nrk", "3,3", NULL);
    }
    // A-read not needed anymore
    close(pipe1[0]);
    // B-write not needed anymore
    close(pipe2[1]);

    if (fork() == 0)
    {
        dup2(pipe2[0], 0);
        // do not pass B-read twice
        close(pipe2[0]);

        execlp("/usr/bin/head", "head", "-5", NULL);
    }
    // B-read not needed anymore
    close(pipe2[0]);
    return 0;
}