#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

struct arg_struct
{
    int arg1;
    int arg2;
};

int fact(int n)
{
    printf("%d ", n);
    if (n <= 1)
    {
        return 1;
    }
    return n * fact(n - 1);
}

void printFact(int n, int m)
{
    int lim = n - m;
    int count = 0;
    // printf("lim = %d\n", lim);
    printf("[");
    if (n == 0 || m == 0)
    {
        printf("0");
    }
    else
    {
        for (int i = n; i > 0; i--, n--)
        {
            if (i == lim)
            {
                break;
            }
            else
            {
                printf("%d", n);
            }
            if (count != m - 1)
            {
                printf("*");
                count++;
            }
        }
        // printf("\n");
    }
    printf("] ");
}

void *funct(void *arguments)
{
    struct arg_struct *args = (struct arg_struct *)arguments;

    printFact(args->arg1, args->arg2);
}

void main()
{
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int) * 4 * 6, IPC_CREAT | 0666);

    int *arr = (int *)shmat(shmid, NULL, 0);

    int matrix[4][6];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            matrix[i][j] = arr[i * 6 + j];
        }
    }

    printf("Matrix: \n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            printf("[%d]\t", matrix[i][j]);
        }
        printf("\n");
    }

    int newMatrix[4][6];
    printf("Input new matrix: (Matrix[4][6])\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            scanf("%d", &newMatrix[i][j]);
        }
    }

    // int hasil[4][6];
    int n, m, index = 0;
    pthread_t tid[24];
    struct arg_struct args;

    printf("Output matrix: \n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (matrix[i][j] >= newMatrix[i][j])
            {
                // printf("lebih-");
                n = matrix[i][j];
                m = newMatrix[i][j];
                // printf("%d", fact(n - m));
                // hasil[i][j] = fact(n) / fact(n - m);
                // printFact(n, m);
                args.arg1 = n;
                args.arg2 = m;
                pthread_create(&tid[index], NULL, &funct, (void *)&args);
                index++;
            }
            else if (newMatrix[i][j] > matrix[i][j])
            {
                // printf("kurang-");
                n = matrix[i][j];
                // printf("%d", fact(n));
                // hasil[i][j] = fact(n);
                // printFact(n, n);
                args.arg1 = n;
                args.arg2 = n;
                pthread_create(&tid[index], NULL, &funct, (void *)&args);
                index++;
            }
            else if (matrix[i][j] == 0)
            {
                // hasil[i][j] = 0;
                // printFact(0, 0);
                args.arg1 = 0;
                args.arg2 = 0;
                pthread_create(&tid[index], NULL, &funct, (void *)&args);
                index++;
                // printf("0");
            }
            // printf("\t");
            // printf("[%d]\t", hasil[i][j]);
            // printf("[%d]-[%d]\t", matrix[i][j], newMatrix[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < index; i++)
    {
        pthread_join(tid[i], NULL);
    }

    shmdt(arr);
    shmctl(shmid, IPC_RMID, NULL);
}