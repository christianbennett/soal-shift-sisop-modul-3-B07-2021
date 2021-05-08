#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>

pthread_t thread[1000];
int many =1;
void *myFile (void *judulFiles) {
    char *ext;
    char judulBaru[2000];
    char *judulFiles1 = (char*) judulFiles;
    snprintf(judulBaru, sizeof judulBaru, "%s", judulFiles1);
    char *judul = judulFiles;
    ext = strtok(judul, ".");
    if(strcmp(ext, judulBaru)==0){
        ext = "unknown";
    }else{
        ext = strtok(NULL, "");
    }
     //buat folder berdasarkan ekstensinya
     //dan masukkan file dengan ekstensi yang sama ke dalam sebuah folder yang namanya sama
    char lokasiPindah[10000];
    const char ch = '/';
    char *ret;
    char *namaFile;
    ret = strrchr(judulBaru, ch);
    //printf ("%s",ret);
    //namaFile = strtok (ret, "/");
    //printf ("%s",namaFile);
    //printf ("%s\n",ret);
    if (ret != NULL) {
        namaFile = strtok (ret,"/");
        printf ("%s\n",namaFile);
    
    }
    if (namaFile[0]=='.'){
      ext="hidden";
    }
    snprintf(lokasiPindah,sizeof lokasiPindah,"%s/%s",ext,namaFile);
    mkdir (ext,0777);

    int hasil; 
    hasil = rename (judulBaru,lokasiPindah);
     if(hasil == 0) 
   {
      printf("File %d : Berhasil Dikategorikan\n",many);
   }
   else 
   {
      printf("File %d : Sad,gagal :(\n",many);
   }
   many++;

     //printf ("%s",ret);
   // printf("%s\n", ext);

}

//fungsi rekursif
void recursive(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            printf("%s\n", dp->d_name);

            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            recursive(path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
  
  /*3a
  Perintah -f */

  //letak argumen pada char 1 akan dibandingkan dengan -f, jika sama maka akan diproses
  if (strcmp(argv[1],"-f")==0){  
    int jumlah = 2;
    int jalankan = 2;
    while (jumlah < argc) {
      pthread_create(&(thread[jumlah]), NULL, myFile, (char*)argv[jumlah]);
      // myFile(argv[jumlah]);
      jumlah++;
    }
    while (jalankan < argc) {
      pthread_join(thread[jalankan],NULL);
      // myFile(argv[jumlah]);
      jalankan++;
    }
  }
  
  /*3b
  Perintah -d */
  else if (strcmp(argv[1],"-d") == 0 ) { 
        
  }
  
  return 0;
}
