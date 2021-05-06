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
        //close read dari pipe1
        close(pipe1[0]);
        dup2(pipe1[1], 1);
        //close read dari pipe1
        close(pipe1[1]);

        execlp("/bin/ps", "ps", "aux", NULL);
    }
    // close write dari pipe1
    close(pipe1[1]);
    //do not create this pipe until needed
    pipe(pipe2);

    if (fork() == 0)
    {
        //close read dari pipe2
        close(pipe2[0]);

        dup2(pipe1[0], 0);
        //close read dari pipe1
        close(pipe1[0]);
        dup2(pipe2[1], 1);
        //close write dari pipe2
        close(pipe2[1]);

        execlp("/usr/bin/sort", "sort", "-nrk", "3,3", NULL);
    }
    //close read dari pipe1
    close(pipe1[0]);
    //close write dari pipe2
    close(pipe2[1]);

    if (fork() == 0)
    {
        dup2(pipe2[0], 0);
        //close read dari pipe2
        close(pipe2[0]);

        execlp("/usr/bin/head", "head", "-5", NULL);
    }
    //close read dari pipe2
    close(pipe2[0]);
    return 0;
}