#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */

#define MSGSIZE 10

int main(int argc, char **argv) {
        int pid;
        int procces;
        printf("mulai \n");

        int size = MSGSIZE * sizeof(int);
        //buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;
        int *idx = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *idx = 0;
        
        printf("Masukan Jumlah Proses : ");
        scanf("%d", &procces);
        
        for (int i = 0; i < procces; i++) {
            switch (pid = fork()) {
            case 0:         /*  proses anak */
                    printf("Proses anak \n");
                    //isi data di memory map
                    srand(getpid());
                    for (int i = 0; i < 10; i++)
                    {
                        shared[*idx] = rand() % 50;                 // tulis data angka random ke pipe 
                        printf("produser menghasilkan angka: %d\n", shared[*idx]);
                        *idx+=1;
                    }
                    break;
            default:        /*  ortu */
                    printf("Proses ortu\n");
                    int pidWait,status;
                    int totalNumber = 0;
                    /* wait sampai child selesai */
                    while (pidWait = wait(&status)) {   
                        if (pidWait == pid)  /* child sukses selesai*/
                            //cetak isi memory map
                            for (int i = 0; i<MSGSIZE; i++) {
                                printf("Consumer menerima angka: %d\n", shared[*idx-1]);
                                totalNumber = totalNumber + shared[*idx-1];
                                *idx-=1;
                            }
                            printf("Total Number yang diterima Consumer : %d\n", totalNumber);
                            break; //keluar dari loop wait
                        if ((pidWait == -1) && (errno != EINTR)) {
                            /* ada error*/
                            perror("waitpid");
                            exit(1);
                        }
                    }
                    break;
            case -1:        /* error */
                    perror("fork");
                    exit(1);
            }
        }
        
        exit(0);
}