#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>

#define INTSIZE 10
int main(int argc, char **argv) {
        int pid;
        int fd[2]; //pipe
        int procces;
        
        
        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }
        printf("mulai \n");
        printf("Masukan Jumlah Proses : ");
        scanf("%d", &procces);
        
        for (int i = 0; i < procces; i++) {
            switch (pid = fork()) {
            case 0:         /* fork returns 0 ke proses anak */
                    printf("Proses anak \n");
                    /* tutup bagian input dari pipe */
                    close(fd[0]);
                    int randomNumberProducer[INTSIZE];
                    srand(getpid());
                    for (int i = 0; i < 10; i++) {
                        randomNumberProducer[i] = rand() % 50;
                        write(fd[1], &randomNumberProducer, sizeof(randomNumberProducer));
                        printf("Producer menghasilkan angka : %d\n", randomNumberProducer[i]);
                    }
                    
                    // tulis ke pipe
                    break;
            default:        /* fork returns pid ke proses ortu */
                    printf("Proses ortu\n");
                    /* tutup bagian output dari pipe */
                    close(fd[1]);
                    
                    int totalNumber = 0;
                    int buffer[INTSIZE];
                    
                    for (int i = 0; i < 10; i++) {
                        read(fd[0], &buffer, sizeof(buffer));
                        printf("Consumer Mengambil angka : %d\n", buffer[i]);
                        totalNumber = totalNumber + buffer[i];
                    }
                    
                    printf("Total Number yang diterima Consumer : %d\n", totalNumber);
                    
                    break;
            case -1:        /* error */
                    perror("fork");
                    exit(1);
            }
        }
        
        exit(0);
}