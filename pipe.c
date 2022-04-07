#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>

#define MSGSIZE 100

int main(int argc, char **argv) {
        char inbuf[MSGSIZE]; //buffer
        int pid;
        int fd[2]; //pipe
        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }
        printf("mulai \n");
        switch (pid = fork()) {
        case 0:         /* fork returns 0 ke proses anak */
                printf("Proses anak \n");
                // tulis data ke pipe
                char* msg1  = "ini pesan child \0"; // simbol \0 adalah end of string
                char* msg2  = "pesan kedua ...  \0"; 
                /* tutup bagian input dari pipe */
                close(fd[0]);
                // tulis ke pipe
                write(fd[1], msg1, MSGSIZE);
                write(fd[1], msg2, MSGSIZE);
                break;
        default:        /* fork returns pid ke proses ortu */
                printf("Proses ortu\n");
                /* tutup bagian output dari pipe */
                close(fd[1]);
                // baca yang ditulis child dari pipe
                read(fd[0], inbuf, MSGSIZE); //buffer terisi
                printf("proses child menulis pesan ke-1: %s \n", inbuf);
                read(fd[0], inbuf, MSGSIZE); //buffer terisi
                printf("proses child menulis pesan ke-2: %s \n", inbuf);
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}
