#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>
#include <time.h>

#define INTSIZE 50
int main(int argc, char **argv) {
        int pid;
        int fd[2]; //pipe
        int procces;
        int buffer[INTSIZE];
        
        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }
        printf("mulai \n");
        printf("Masukan Jumlah Proses : ");
        scanf("%d", &procces);
        
        switch (pid = fork()) {
        case 0:         /* fork returns 0 ke proses anak */
                printf("Proses anak \n");
                /* tutup bagian input dari pipe */
                close(fd[0]);
                
                srand(time(0);
                int randomNumberProducer[sizeof(procces)];
                for (int i = 0; i < procces; i++) {
                    randomNumberProducer[i] = rand() % 50;
                    printf("Producer menghasilkan angka : %d\n", randomNumberProducer[i]);
                }
                
                // tulis ke pipe
                write(fd[1], randomNumberProducer, INTSIZE);
                break;
        default:        /* fork returns pid ke proses ortu */
                printf("Proses ortu\n");
                /* tutup bagian output dari pipe */
                close(fd[1]);
                read(fd[0], buffer, INTSIZE);
                
                int totalNumber = 0;
                for (int i = 0; i < procces; i++) {
                    printf("Consumer Mengambil angka : %d\n", buffer[i]);
                    totalNumber = totalNumber + buffer[i];
                }
                
                printf("Total Number yang diterima Consumer : %d\n", totalNumber);
                
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}
