#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>


#define PORT 8000
#define BACKLOG 50

pthread_mutex_t mutex;

typedef struct clientHolder {
    int clientSocket;
    char rootFilePath[1000];
} clientHolder;

void networkCreation(char *rootDirectory);
char *parseHeader(char *String);
void *processClientRequest(void *pass);


void networkCreation(char *rootDirectory) {
    int serverSocket;
    struct addrinfo hints; 
    struct addrinfo *serviceInformation; 
    struct addrinfo *dataHolder;	
    memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    char placeHolder[8];
    sprintf(placeHolder, "%d", PORT);
    int addressHolder = getaddrinfo(NULL, placeHolder, &hints, &serviceInformation);
    struct sockaddr_storage theirAddress; 
    socklen_t sin_size;
    int recievedData;
    pthread_mutex_init(&mutex, NULL);
    for(dataHolder = serviceInformation; dataHolder != NULL; dataHolder = dataHolder->ai_next) {
	    if ((serverSocket = socket(dataHolder->ai_family, dataHolder->ai_socktype, dataHolder->ai_protocol)) == -1) { 	
            printf("Socket Failed\n");		
            continue;
        }
        else
            printf("Socket Created\n");
	    if (bind(serverSocket, dataHolder->ai_addr, dataHolder->ai_addrlen) == -1) { 
            printf("Binding Failed\n");
            close(serverSocket); 
            continue; 
        }
        else
            printf("Binding Success\n");
	    break;
    }
    freeaddrinfo(serviceInformation);
	if (listen(serverSocket, BACKLOG) == -1)
        exit(1); 
    else
        printf("Listening....\n");

    pthread_t threadIDS[BACKLOG];
    int threadCounter = 0;
	while(1) {
        clientHolder *clientInformation = malloc(sizeof(clientHolder));
		sin_size = sizeof(theirAddress);
		clientInformation->clientSocket = accept(serverSocket, (struct sockaddr *)&theirAddress, &sin_size);
        strcpy(clientInformation->rootFilePath, rootDirectory);
        pthread_create(&threadIDS[threadCounter], NULL, processClientRequest, (void*)clientInformation);
        threadCounter++;
        if(threadCounter == BACKLOG)
            threadCounter = 0;
    }
    int i = 0;
    while(i < threadCounter) {
        pthread_join(threadIDS[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&mutex);
    close(serverSocket);
}
char *parseHeader(char *String) {
    char *stringToParse;
    char *holder = String;
    stringToParse = strtok_r(holder, "GET ", &holder);
    return(stringToParse);
}

void *processClientRequest(void *pass) {
    clientHolder *retreiveClientInfo = (clientHolder *)pass;
    char homeDirectory[1000];
    char data [500];
    int recievedData;
    pthread_mutex_lock(&mutex);
    recievedData = recv(retreiveClientInfo->clientSocket, data, sizeof(data) - 1, 0);
    if(recievedData <= 0) { 
        pthread_mutex_unlock(&mutex);
        close(retreiveClientInfo->clientSocket);
        free(retreiveClientInfo);
        pthread_exit(NULL);
        return NULL;
    }
    data[recievedData] = '\0';
    if(retreiveClientInfo->rootFilePath[0] == '~')
        strcpy(homeDirectory, getenv("HOME"));
    else    
        strcpy(homeDirectory, retreiveClientInfo->rootFilePath);
    printf("\n%s", data);
    char *holder = parseHeader(data);
    if(holder[0] == '/')
        holder++;
    char assembledPath[1200];
    sprintf(assembledPath, "%s/%s", homeDirectory, holder);
    int file = open(assembledPath, O_RDONLY);
    if(file == -1) {
        char errorResponse[1000] = ("HTTP/1.1 404 Not Found\r\n"
                                    "\r\n"
                                    "THE FILE DOES NOT EXIST!\r\n");
        send(retreiveClientInfo->clientSocket, errorResponse, strlen(errorResponse), 0);
    }
    else {
        struct stat contentLength;
        stat(assembledPath, &contentLength);
        int sizeOfFile = contentLength.st_size;
        char *contentHolder = malloc(sizeOfFile);
        memset(contentHolder, '\0', sizeOfFile);
        read(file, contentHolder, sizeOfFile);
        char success[1200];
        sprintf(success, "HTTP/1.1 200 OK\r\n"
                        "Content-Length: %d\r\n"
                        "\r\n", sizeOfFile);
        send(retreiveClientInfo->clientSocket, success, strlen(success), 0);
        send(retreiveClientInfo->clientSocket, contentHolder, sizeOfFile, 0);
        free(contentHolder);
        close(file);
    }
    pthread_mutex_unlock(&mutex); 
    close(retreiveClientInfo->clientSocket);
    free(retreiveClientInfo); 
    pthread_exit(NULL);
    return NULL;
}
int main(int argc, char *argv[]) {
    if(argc == 2)
        networkCreation(argv[1]);
    else {
        printf("Failed: No File Path Passed.\n");
        exit(1);
    }   
    return 0;
}
