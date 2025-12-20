#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



//The client mode creates a client socket to communicate and exchange data with the server. It recieves data from
//the server and then prints that data, "pong" and sends the message "ping" to the server. Once the server
//closes the client program also terminates.
void clientMode(char *ip, int port) {
    //client socket for data exchange/communication.
    int clientSocket;
    //Data of the client recieved from the server connection.
    int clientData;
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));           //Sets up client structure.
    client.sin_family = AF_INET;                  //IPV4.
    client.sin_port = htons(port);                //Sets up port for the client.
    inet_pton(AF_INET, ip, &client.sin_addr);     //To convert IP from text to binary.

    //Creates a client socket and proceeds to connect.
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1) {
        printf("Socket creation failed.\n");
        exit(1);
    }
    else    
        printf("Socket created.\n");            
    if(connect(clientSocket, (struct sockaddr *)&client, sizeof(client)) == -1) {
        printf("Connection failed.\n");
        close(clientSocket);
        exit(1);
    }
    else    
        printf("Connection success.\n");
    while(1) {
        char containerClient[100];
        clientData = recv(clientSocket, containerClient, sizeof(containerClient) - 1, 0);
        if(clientData <= 0) {
            printf("Server closed.\n");
            break;
        }
        containerClient[clientData] = '\0';
        printf("Client received: %s\n", containerClient);
        char toSend[] = "ping";
        send(clientSocket, toSend, strlen(toSend), 0);
    }
    close(clientSocket);
}

//The server mode creates a server socket to listen for incoming connections to the server, once it establishes
//a connection it then creates a client socket to communicate and exchange data with the client. It initiates 
//the communication with the client by first sending the message "pong", then it recieves data and prints it,
//which is "ping". Then it proceeds to send "pong" again. Once the client closes, the server program terminates. 
void serverMode() {
    //IP address.
    char IP[] = "127.0.0.1";
    //Port number.
    char PORT[] = "1050";
    //server socket to listen for incoming connections.
    int serverSocket;
    //client socket for data exchange/communication.
    int clientSocket; 
    struct addrinfo hints; 
    struct addrinfo *serviceInformation; 
    struct addrinfo *dataHolder;	
    memset(&hints, 0, sizeof (hints));          //To set up hint structure.
    hints.ai_family = AF_UNSPEC;                //IPV4 or IPV6.
    hints.ai_socktype = SOCK_STREAM;            //TCP connection.
    hints.ai_flags = AI_PASSIVE;                //To use my IP.
    int addressHolder = getaddrinfo(NULL, PORT, &hints, &serviceInformation); //Get available interfaces.
    struct sockaddr_storage theirAddress;       //The client's address information.
    socklen_t sin_size;
    //Data of the server recieved from client connection.
    int serverData;

    //To loop through and bind the server socket.
    for(dataHolder = serviceInformation; dataHolder != NULL; dataHolder = dataHolder->ai_next) {
	    if ((serverSocket = socket(dataHolder->ai_family, dataHolder->ai_socktype, dataHolder->ai_protocol)) == -1) { 	
            continue;
        }
        else
            printf("Socket Created\n");
	    if (bind(serverSocket, dataHolder->ai_addr, dataHolder->ai_addrlen) == -1) { 
            close(serverSocket); 
            continue; 
        }
        else
            printf("Binding Success\n");
	    break;
    }
    //Prints the available address and port to connect to.
    printf("The address is %s and the port is %s\n", IP, PORT);
    //To free the structure once it is finished.
    freeaddrinfo(serviceInformation);
	if (listen(serverSocket, 10) == -1)
        exit(1); 
    else
        printf("Listening....\n");
    char containerServer[100];
    sin_size = sizeof(theirAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&theirAddress, &sin_size);
    char toSend[] = "pong";
    send(clientSocket, toSend, strlen(toSend), 0);
    while(1) {
        serverData = recv(clientSocket, containerServer, sizeof(containerServer) - 1, 0);
        if(serverData <= 0) {
            printf("Client closed.\n");
            break;
        }
        containerServer[serverData] = '\0';
        printf("Server received: %s\n", containerServer);
        send(clientSocket, toSend, strlen(toSend), 0);
    }
    close(clientSocket);
    close(serverSocket);
}

//The beginning of the program.
int main(int argc, char *argv[]) {
    if(argc == 3) {
        clientMode(argv[1], atoi(argv[2]));
    }
    else
        serverMode();
    return 0;
}