# soal-shift-sisop-modul-3-B07-2021
## Anggota Kelompok
* Christian Bennett Robin 05111940000078
* Zelda Elma Sibuea 05111940000038
* Hanifa Fauziah 05111940000024
##Soal 1

##Soal 2
Crypto (kamu) adalah teman Loba. Suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya. Detil dari tugas tersebut adalah:
###2A###
Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

Pertama-tama inisialisasi key dan array `*arr` agar dapat digunakan dalam shared memory pada program kedua.

```c
 key_t key = 1234;
    int shmid = shmget(key, sizeof(int) * 4 * 6, IPC_CREAT | 0666);

    int *arr = (int *)shmat(shmid, NULL, 0);
```

Lalu sesuai permintaan soal yaitu membuat 2 matrix dengan input dari user untuk perkalian biasa menggunakan for loop. Matrix pertama memiliki besar 4x3, matrix kedua memiliki besar 3x6, dan hasil perkalian matrix memiliki besar 4x6.

```c
printf("This is a program to calculate matrix mutlipication.\nEnter the element of the first matrix: (Matrix[4][3])\n");

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            scanf("%d", &mat1[i][j]);
        }
    }

    printf("Enter the element of the second matrix: (Matrix[3][6])\n");

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            scanf("%d", &mat2[i][j]);
        }
    }

    int sum = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                sum += (mat1[i][k] * mat2[k][j]);
            }
            matrix[i][j] = sum;
            sum = 0;
        }
    }
```
 
Selanjutnya memasukkan value dari matrix yang telah dikali ke array yang akan digunakan dalam shared memory yaitu matrix `arr`.

```c
for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            arr[i * 6 + j] = matrix[i][j];
            // printf("arr[%d][%d] = %d\n", i, j, arr[i * 6 + j]);
            // sleep(1);
        }
    }
```

Terakhir, jalankan program 2B.
```c
printf("Run program 2.\n");
    for (int i = 30; i > 0; i--)
    {
        printf("%d...\n", i);
        sleep(1);
    }
```

![soal2a](Screenshots/2a.jpg)

###2B###
Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (Catatan!: gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil) (Catatan!: gunakan thread untuk perhitungan di setiap cel).

Pertama-tama menginisialisasi key dan arr agar dapat digunakan shared memorynya dari program pertama, lalu assign valuenya pada matrix lokal pada program ke 2.
```c
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
```

Selanjutnya menginputkan matrix untuk melakukan perhitungan angka faktorial sesuai besar matrix hasil perkalian yaitu 4x6.
```c
printf("Input new matrix: (Matrix[4][6])\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            scanf("%d", &newMatrix[i][j]);
        }
    }    
```

Lalu untuk setiap kotak matrix inputan, dilakukan perbandingan terhadap matrix hasil perkalian dari program 1 dan dilakukan perhitungan angka faktorialnya sesuai permintaan soal. Ini dilakukan menggunakan thread.
```c
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
            }
            else if (matrix[i][j] == 0)
            {
                // hasil[i][j] = 0;
                // printFact(0, 0);
                args.arg1 = 0;
                args.arg2 = 0;

                // printf("0");
            }
            pthread_create(&tid[index], NULL, &funct, (void *)&args);
            pthread_join(tid[index], NULL);
            index++;
            // printf("\t");
            // printf("[%d]\t", hasil[i][j]);
            // printf("[%d]-[%d]\t", matrix[i][j], newMatrix[i][j]);
        }
        printf("\n");
    }
```

Untuk fungsi yang dipakai yaitu pertama `c void printFact(int n, int m)`, yaitu untuk melakukan print deret faktorial, n sebagai batas atasnya dan n-m sebagai batas bawahnya. Contoh: jika n = 5, m = 6, maka hasilnya adalah 5\*4\*3\*2\*1, jika n = 5, m = 0, maka hasilnya 0, dan jika n = 5, m = 2, maka hasilnya 5\*4.

```c
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
```

Lalu untuk fungsi yang memanggil fungsi `c void printFact(int, int)` tadi dalam bentuk thread merupakan fungsi `c void *funct(void arguments)`, yaitu: 

```c
void *funct(void *arguments)
{
    struct arg_struct *args = (struct arg_struct *)arguments;

    printFact(args->arg1, args->arg2);
}
```

Didalam fungsi ini menggunakan struct sebagai argumen karena pada parameter fungsi menggunakan lebih dari 1 variabel, yaitu n dan m. Struct didefinisikan di awal program, yaitu:

```c
struct arg_struct
{
    int arg1;
    int arg2;
};
```

![soal2b](Screenshots/2b.jpg)

###2C###
Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “`c ps aux | sort -nrk 3,3 | head -5`” (Catatan!: Harus menggunakan IPC Pipes)

Pertama-tama menginisialisasi kedua pipe yang akan digunakan menggunakan: 

```c
int pipe1[2];
int pipe2[2];
```
 
Lalu melakukan pipe dan fork untuk proses pertama.
 
```c
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
```
 
`c close(pipe1[0])` untuk menutup bagian read dari pipe1, lalu `c dup2(pipe1[1], 1)` untuk meredirect output proses dari standart output ke bagian write pipe1, dan `c close(pipe1[1])` untuk mengclose bagian read dari pipe1. `c execlp("/bin/ps", "ps", "aux", NULL)` untuk melakukan proses `ps aux`, dan terakhir bagian write dari pipe1 diclose meggunakan `c close(pipe1[1])`.
 
 Lalu lanjut membuat pipe dan fork untuk proses kedua.
 
 ```c
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
```
 
Mirip dengan bagian sebelumnya, bedanya disini `c dup2(pipe1[0], 0)` dilakukan untuk meredirect standart input ke bagian read pipe1, dan `c dup2(pipe2[1], 1)` untuk meredirect standard output pipe2 ke bagian write pipe2. Lalu `c execlp("/usr/bin/sort", "sort", "-nrk", "3,3", NULL)` untuk melakukan proses `sort -nrk 3,3`, terakhir mengclose bagian read dari pipe1 dan write dari pipe2 agar tidak terjadi memory leak.

Dan dilanjuti dengan pipe dan fork proses terakhir.

```c
if (fork() == 0)
    {
        dup2(pipe2[0], 0);
        //close read dari pipe2
        close(pipe2[0]);

        execlp("/usr/bin/head", "head", "-5", NULL);
    }
    //close read dari pipe2
    close(pipe2[0]);
```

`c dup2(pipe2[0], 0)` dilakukan untuk meredirect standard input ke bagian read dari pipe2. Lalu dilanjuti dengan pemanggilan proses menggunakan `c  execlp("/usr/bin/head", "head", "-5", NULL)` untuk melakukanp proses `head -5`, dan mengclose bagian read dari pipe2.

![soal2c](Screenshots/2c.jpg)

##Soal 3
