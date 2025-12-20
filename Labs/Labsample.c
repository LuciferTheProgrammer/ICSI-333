#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

int pipeFileDescriptor1 [2];
int pipeFileDescriptor2 [2];

void ping();
void pong();
void signalHandler(int signalToCatch);

void ping() {
    int value = 0;
    while(value < 100) {
        printf("ping-%d\n", value);
        value++;
        write(pipeFileDescriptor1[1], &value, sizeof(value));
        read(pipeFileDescriptor2[0], &value, sizeof(value));
    }
}
void pong() {
    int value = 0;
    signal(SIGUSR1, signalHandler);
    while(1) {
        read(pipeFileDescriptor1[0], &value, sizeof(value));
        printf("pong-%d\n", value); 
        value++;
        write(pipeFileDescriptor2[1], &value, sizeof(value));
    }
}
void signalHandler(int signalToCatch) {
    printf("pong quitting\n");
    exit(0);
}

int main() {
    int worked1 = pipe(pipeFileDescriptor1);
    int worked2 = pipe(pipeFileDescriptor2);
    if(worked1 < 0 || worked2 < 0) {
        perror("Pipe createion failed");
        exit(1);
    }
    pid_t part_1 = fork();
    if(part_1 < 0) {
        fprintf(stderr, "One of the fork processes failed.");
        exit(1);
     }
    if(part_1 > 0) {
        ping();
        kill(part_1, SIGUSR1);
        wait(NULL);
    }
    else 
        pong();
    close(pipeFileDescriptor1[0]);
    close(pipeFileDescriptor1[1]);
    close(pipeFileDescriptor2[0]);
    close(pipeFileDescriptor2[1]);
    return 0;
}