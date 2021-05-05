#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void main()
{
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int matrix[4][6], mat1[4][3], mat2[3][6];

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}