#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

//Declaration of two global pipes.
int pipeFileDescriptor1 [2];
int pipeFileDescriptor2 [2];

//Declaration of functions to be used with some specific return and parameter types.
void ping();
void pong();
void signalHandler(int signalToCatch);

//The function prints the ping value, then increments it. It then writes the value to a pipe and reads
//the value back from the other pipe until the value is greater than or equal to 100. Then it proceeds to exit.
void ping() {

    //Delcaration of a variable of the integer type and is set to 0.
    int value = 0;
    while(value < 100) {
        printf("ping-%d\n", value);
        value++;
        write(pipeFileDescriptor1[1], &value, sizeof(value));
        read(pipeFileDescriptor2[0], &value, sizeof(value));
    }
    exit(0);
}
//The pong function to call a signal handler when it recieves a signal. But before getting the signal
//it loops forever to read from a pipe and print the pong value. Then it proceeds to increment the value
//and write the value back to the other pipe.
void pong() {

    //Delcaration of a variable of the integer type and is set to 0.
    int value = 0;
    while(1) {
        read(pipeFileDescriptor1[0], &value, sizeof(value));
        printf("pong-%d\n", value); 
        value++;
        write(pipeFileDescriptor2[1], &value, sizeof(value));
        if(value == 100)
            signalHandler(SIGUSR1);
    }
}
//The function for the signal handler and prints the message "pong quitting" when it is
//called by the pong function and then proceeds to exit.
void signalHandler(int signalToCatch) {
    printf("pong quitting\n");
    exit(0);
}

//The beginning of the program.
int main() {

    //The first pipe.
    int worked1 = pipe(pipeFileDescriptor1);

    //The second pipe.
    int worked2 = pipe(pipeFileDescriptor2);

    //To check if one of the pipes failed.
    if(worked1 < 0 || worked2 < 0) {
        printf("Pipe creation failed.\n");
        exit(1);
    }

    //Declaration of a variable to contain the proccess ID returned from the fork(), which creates
    //two processes.
    pid_t part_1 = fork();

    //To check if the procces failed.
    if(part_1 < 0) {
        printf("Fork process failed.\n");
        exit(1);
    }
    //The parent process.
    if(part_1 > 0) {
        ping();
        wait(NULL);
        kill(part_1, SIGUSR1);
    }
    //The child process.
    else 
        pong();
    //Closes both two ends of the two global pipes.
    close(pipeFileDescriptor1[0]);
    close(pipeFileDescriptor1[1]);
    close(pipeFileDescriptor2[0]);
    close(pipeFileDescriptor2[1]);
    return 0;
}