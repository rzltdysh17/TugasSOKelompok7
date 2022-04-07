#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */

#define INTSIZE 50

int main(int argc, char **argv) {
        int pid;
        int proccesProd;
        int proccesCon;

        printf("mulai \n");
        // printf("Masukan Jumlah Proses : ");
        // scanf("%d", &procces);
        
        int size = INTSIZE * sizeof(int);
        //buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;
        switch (pid = fork()) {
        case 0:         /*  proses anak */
                printf("Proses anak \n");
                //isi data di memory map
                printf("Masukan Jumlah Proses Producer : ");
                scanf("%d", &proccesProd);
                
                for (int i = 0; i < proccesProd; i++) {
                    shared[i] = rand() % 50;
                    printf("Producer menghasilkan angka : %d\n", shared[i]);
                }
                break;
        default:        /*  ortu */
                printf("Proses ortu\n");
                int pidWait,status;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        //cetak isi memory map
                        printf("Masukan Jumlah Proses Consumer : ");
                        scanf("%d", &proccesCon);
                        int totalNumber = 0;
                        
                        for (int i = 0; i<proccesCon; i++) {
                            printf("Consumer menerima angka: %d\n", shared[i]);
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
