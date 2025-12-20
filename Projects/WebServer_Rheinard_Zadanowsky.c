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

//Port Number.
#define PORT 8000

//Backlog number.
#define BACKLOG 10

//Totat amount of threads allowed.
#define TOTALTHREADS 50


//Client structure to hold information about client socket and root directory to look 
//for file.
typedef struct clientHolder {
    int clientSocket;
    char rootFilePath[1000];
} clientHolder;

//Prototype Functions to be used with specific return types and parameter types.

void networkCreation(char *rootDirectory);
char *parseHeader(char *String);
void *processClientRequest(void *pass);

//Creates a server socket, binds it with an address and port, and listens for incomming connections to the
//server from the client side. Then it proceeds to accept connections with the client, this also proceeds
//to create a thread per client request which recieves and sends data based on the client request,
//enabling for multithreading, but no more than 50 threads at a time.
void networkCreation(char *rootDirectory) {
    int serverSocket;                   //Server socket to listen for incoming connections.
    struct addrinfo collectiveUnits;    //Collective units structure.
    struct addrinfo *serviceInformation;    
    struct addrinfo *dataHolder;	
    memset(&collectiveUnits, 0, sizeof (collectiveUnits)); //Sets up the collective units structure.
    collectiveUnits.ai_family = AF_UNSPEC;                 //IPV4 or IPV6.
    collectiveUnits.ai_socktype = SOCK_STREAM;             //TCP connection.
    collectiveUnits.ai_flags = AI_PASSIVE;                 //To use my IP.
    char placeHolder[8];                                   //To store a String with length 8.
    sprintf(placeHolder, "%d", PORT);

    //Get available interfaces.
    int addressHolder = getaddrinfo(NULL, placeHolder, &collectiveUnits, &serviceInformation);
    struct sockaddr_storage theirAddress;                   //The client's address information.
    socklen_t sin_size;

    //To loop through and bind the server socket.
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
    //To free the structure once it is finished.
    freeaddrinfo(serviceInformation);
	if (listen(serverSocket, BACKLOG) == -1)
        exit(1); 
    else
        printf("Listening....\n");

    //Creates an array of threads to store up to 50 threads.
    pthread_t threadIDS[TOTALTHREADS];

    //To keep track of the number of threads.
    int threadCounter = 0;
	while(1) {

        //Create client structure to get and store client socket and root directory.
        clientHolder *clientInformation = malloc(sizeof(clientHolder));
		sin_size = sizeof(theirAddress);
		clientInformation->clientSocket = accept(serverSocket, (struct sockaddr *)&theirAddress, &sin_size);
        strcpy(clientInformation->rootFilePath, rootDirectory);

        //Creates a thread to handle a client request.
        pthread_create(&threadIDS[threadCounter], NULL, processClientRequest, (void*)clientInformation);

        //The thread is detached so it can automatically clean up resources once the thread
        //terminates when the process is finished and is indepedent of the other threads.
        pthread_detach(threadIDS[threadCounter]);

        //Checks if threads are up to 50, if it is it resets the thread count to 0. If not the
        //thread is incremented for the creation of a new thread.
        if(threadCounter == TOTALTHREADS)
            threadCounter = 0;
        else
            threadCounter++;
        }
    close(serverSocket);
}

//This function parses the client request, to get rid of "GET", " ", and "HTTP/1.1" and
//return the file path from the client request.
char *parseHeader(char *String) {

    //Pointer to a String.
    char *stringToParse;

    //Pointer to a String.
    char *holder = String;
    stringToParse = strtok_r(holder, "GET ", &holder);
    return(stringToParse);
}

//This function handles the client request, is invoked and processed per thread created. It receives
//a request from the client and sends the proper response. If the file being requested does not exist,
//it sends a "404 Not Found" and if the file exist it sends a "202 OK" with the contents of the file
//being requested.
void *processClientRequest(void *pass) {
    clientHolder *retrieveClientInfo = (clientHolder *)pass;

    //An array of characters of size 1000.
    char homeDirectory[1000];

    //An array of characters of size 500.
    char data [500];

    //To store the recieved data from the request.
    int recievedData;

    //Data that is received to be processed.
    recievedData = recv(retrieveClientInfo->clientSocket, data, sizeof(data) - 1, 0);

    //Handles error case if there is no data received during the thread process.
    if(recievedData <= 0) { 
        close(retrieveClientInfo->clientSocket);
        free(retrieveClientInfo);
        pthread_exit(NULL);
    }
    //Sets the stored data, the file request.
    data[recievedData] = '\0';

    //Gets the home directory of the user.  
    strcpy(homeDirectory, retrieveClientInfo->rootFilePath);

    //To check if home path has an ending of '/', if it does it is removed.
    if(homeDirectory[strlen(homeDirectory) - 1] == '/')
        homeDirectory[strlen(homeDirectory) - 1] = '\0';
    printf("\n%s", data);

    //Gets the parsed file request and skips '/' part of the request.
    char *holder = parseHeader(data);
    if(holder[0] == '/')
        holder++;
    
    //An array of characters of size 1200.
    char assembledPath[1200];

    //Combines the file paths to the absolute file path to be
    //opened and processed.
    sprintf(assembledPath, "%s/%s", homeDirectory, holder);
    int file = open(assembledPath, O_RDONLY);
    if(file == -1) {
        char errorResponse[1000] = ("HTTP/1.1 404 Not Found\r\n"
                                    "\r\n"
                                    "THE FILE DOES NOT EXIST!\r\n");
        //Sends the proper response if file does not exist.
        send(retrieveClientInfo->clientSocket, errorResponse, strlen(errorResponse), 0);
    }
    else {
        //Use of stat to get size of file in bytes.
        struct stat contentLength;
        stat(assembledPath, &contentLength);

        //To store the size of file being sent.
        int sizeOfFile = contentLength.st_size;

        //Create holder to store the content of the file being requested.
        char *contentHolder = malloc(sizeOfFile);

        //Sets up the container.
        memset(contentHolder, '\0', sizeOfFile);

        //Reads data from the file.
        read(file, contentHolder, sizeOfFile);

        //An array of characters of size 1200.
        char success[1200];
        sprintf(success, "HTTP/1.1 200 OK\r\n"
                        "Content-Length: %d\r\n"
                        "\r\n", sizeOfFile);

        //Sends the proper response if the file exist.
        send(retrieveClientInfo->clientSocket, success, strlen(success), 0);
        send(retrieveClientInfo->clientSocket, contentHolder, sizeOfFile, 0);
        free(contentHolder);
        close(file);
    }
    close(retrieveClientInfo->clientSocket);
    free(retrieveClientInfo); 
    pthread_exit(NULL);
}

//The beginning of the program. It checks if there is a root file path passed as a second
//parameter in the command line. If there is no second argument in the command line, then it fails
//and exits.
int main(int argc, char *argv[]) {
    if(argc == 2)
        networkCreation(argv[1]);
    else {
        printf("Failed: No File Path Passed. For example: ./myWebServer ~/myFiles or /home/<User>, the User being the user profile.\n");
        exit(1);
    }   
    return 0;
}
