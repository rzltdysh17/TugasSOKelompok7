#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */
#include <time.h>	    /* time */

#define INTSIZE 50

int main(int argc, char **argv) {
        int pid;
        int proccesProd;
        int proccesCon;
        int procces;

        printf("mulai \n");
        printf("Masukan Jumlah Proses : ");
        scanf("%d", &procces);
        
        int size = INTSIZE * sizeof(int);
        //buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;
        
        switch (pid = fork()) {
        case 0:         /*  proses anak */
                printf("Proses anak \n");
                //isi data di memory map
                // printf("Masukan Jumlah Proses Producer : ");
                // scanf("%d", &proccesProd);
                
                srand(time(0));
                for (int i = 0; i < procces; i++) {
                    shared[i] = rand() % 50;
                    printf("Producer %d menghasilkan angka : %d\n", i, shared[i]);
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
                        // printf("Masukan Jumlah Proses Consumer : ");
                        // scanf("%d", &proccesCon);
                        
                        for (int i = 0; i<procces; i++) {
                            printf("Consumer %d menerima angka: %d\n", i, shared[i]);
                            totalNumber = totalNumber + shared[i];
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
        exit(0);
}
