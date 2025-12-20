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
#include <time.h>
#include <signal.h>

//Port Number.
#define PORT 8000

//Backlog number.
#define BACKLOG 10

//Totat amount of threads allowed.
#define TOTALTHREADS 50

//A structure to create a node of the wordListNode type to store a given word that is read
//from a file and points to the next node of the Linked List.
typedef struct wordListNode {
    char stringForWord[30];
    struct wordListNode *next;
} wordListNode;

//A structure to create a node of the gameListNode type to store a given word that can be made up
//from the master word and points to the next node of the Linked List.
typedef struct gameListNode {
    char stringForGame[30];
    bool hasBeenFound;
    struct gameListNode *next; 
} gameListNode;

//Client structure to hold information about client socket, root directory to look 
//for file, number of words from the master dictionary, and the user guess.
typedef struct clientHolder {
    int clientSocket;
    char rootFilePath[1000];
    int numberOfWords;
    int guessValidOrNot;
} clientHolder;

//A pointer to the wordListNode type Linked List that contains words that is read from a file.
wordListNode *firstWordNode = NULL;

//A pointer to the gameListNode type Linked List that contains words that can be made up from the master word.
gameListNode *firstGameNode = NULL;

//The array of characters to hold the user input of length 100.
//char input[100];

//Declaration of functions to be used in the program with specific return types and parameter types.

int initialization();
void gameLoop(char *masterWord);
bool isDone();
void tearDown();
void displayWorld(char *masterWord, int descriptor, int validateGuess);
int acceptInput(char *putIn);
int *getLetterDistribution(char *string, int *numberOfInstances);
bool compareCounts(int *dictionary, int *userData);
char *getRandomWord(int pass);
void findWords(char *taken);
void addWordListNode(char *dataForWordList);
void addGameListNode(char *dataForGameList);
void cleanupWordListNodes();
void cleanupGameListNodes();
void reverseOrder();
char *sortString(char *stringHolder);
void swapFunction(char *stringHolder, int currentPosition, int minimum);
char *printUnderScores(char *template, char *holdBlankLines);
bool compareStrings(char *userInput, char *wordToFind);
void capitalizeWords(char *word);
void networkCreation(char *rootDirectory);
void *processClientRequest(void *pass);
char *newParseFunction(char *String);
void signalToKill(int signal);

//Signal function to handle and free the master dictionary and current game session when the program
//is terminated prematurely.
void signalToKill(int signal) {
    if(firstWordNode != NULL)
        cleanupWordListNodes();
    if(firstGameNode != NULL)
        cleanupGameListNodes();
    if(firstWordNode == NULL)
        printf("\nMaster dictionary successfully freed.\n");
    else
        printf("\nNo successful free for the master dictionary.\n");
    if(firstGameNode == NULL)
        printf("\nGame puzzle successfully freed.\n");
    else
        printf("\nNo successful free for the game puzzle.\n");
    printf("\nServer shut down along with Words Without Friends Game.\n");
    exit(0);
}

//The function loads the dictionary into memory. This opens and reads data from a file, which adds the words 
//into the wordListNode type Linked List and then it closes the file and returns the total number of words.
int initialization() {
    srand(time(NULL));

    //Declaration of a pointer to a file.
    FILE *file;

    //Declaration of an array of characters of length 30.
    char container[30];

    //Declaration of a variable of the integer type and is set to 0.
    int counter = 0;

    file = fopen("2of12.txt", "r");

    //To check if the file being referenced exist or not.
    if(file == NULL) {
        printf("The file DOES NOT EXIST!\n");
        exit(1);
    }
    while(fgets(container, sizeof(container), file) != NULL) { 
        container[strcspn(container, "\r\n")] = 0;

        //To detect and filter out the last empty line from the file.
        if(strlen(container) == 0)
            break;
        capitalizeWords(container);
        addWordListNode(container);
        counter++;
        memset(container, '\0', sizeof(container));
    }
    fclose(file);
    return counter;
}
//This returns true when all the possible words that can be formed within a given master word in the
//gameListNodes has been found by the user and false otherwise.
bool isDone() {

   //Declaration of a pointer of the gameListNode type Linked List.
    gameListNode *temporary = firstGameNode;
    while(temporary != NULL) {
        if(!(temporary->hasBeenFound))
            return false;
        temporary = temporary->next;
    }
    return true;
}
//Prints the message "All Done" in the screen, indicating the game is finished.
void tearDown() {
    printf("All Done!\n");
}
//This will send the display of the puzzle game to the web browser, by first showing the alphabetically sorted master word, 
//the words to find based on the master word which are in hyphens, and the words that are correctly found
//by the user. 
void displayWorld(char *masterWord, int descriptor, int validateGuess) {

    //Declaration of a pointer to a String and is set to the template of an HTML set up.
    char *template1 = "<html><body style=\"background-color: black; color: white;\"><h1 style=\"border: 4px solid yellow; color: orange; font-size: 28px; text-align: center;\"> Words Without Friends Game</h1>"
                    "<form action=\"words\" method=\"GET\"><div style=\"color: brown; font-size: 18px;\"> Enter a guess: " 
                    "<input type=\"text\" name=\"move\" autofocus>"
                    "</input></div></form>";

    //Declaration of a pointer to a String and is set to the template of an HTML set up.
    char *template2 = "</body></html>";

    //Declatrion of an array of characters of length 50000.
    char buffer[50000];
    memset(buffer, '\0', sizeof(buffer));

    //To check if user input is invalid, if it is it prompts the user that it's not correct.
   if(validateGuess == -1) {

        //Declaration of a pointer to a String and is set to hold the invalid message prompt.
        char *invalid = "<p style=\"text-align: center; color: red; font-size: 24px; border: 2px solid red;\">"
                        "INVALID GUESS</p>";
        sprintf(buffer + strlen(buffer), "%s", invalid);
    }

    //Declaration of an array of characters of length 50.
    char masterWordHolder[50];
    strcpy(masterWordHolder, masterWord);

    //Declaration of a pointer to a String and is set to point to the sorted master word.
    char *placement = sortString(masterWordHolder);
    sprintf(buffer + strlen(buffer), "<p style=\"border: 4px solid gray; color: blue; font-size: 24px; font-style: italic; text-align: center;\">");
    for(int i = 0; i < strlen(placement); i++)
        sprintf(buffer + strlen(buffer), "%c ", placement[i]);
    sprintf(buffer + strlen(buffer), "</p>");
    sprintf(buffer + strlen(buffer), "<hr style=\"background-color: orange;\"></hr>");

    //Declaration of a pointer and is assigned to point to the gameListNode type Linked List.
    gameListNode *took = firstGameNode;
    while(took != NULL) {
        if(took->hasBeenFound)
            sprintf(buffer + strlen(buffer), "<p style=\"background-color: green; color: white; font-size: 20px;\">FOUND: %s</p>", took->stringForGame);
        else {

            //Declaration of an array of characters of length 1000.
            char toContain[1000];
            memset(toContain, '\0', sizeof(toContain));

            //Declaration of a pointer to a String and is set to the blank lines for the missing words.
            char *stringPlace = printUnderScores(took->stringForGame, toContain);
            sprintf(buffer + strlen(buffer), "<p style=\"color: red; font-size: 20px;\">%s</p>", stringPlace);
        }
        took = took->next;
    }

    //Declaration of a pointer to a String and is set to hold the HTTP header '200 OK'.
    char *responder = "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8 \r\n\r\n";

    //Declaration of an array of characters of length 75000.
    char gameWorld[75000];

    //Combines and creates the HTML version of the game puzzle to be sent to the web browser.
    sprintf(gameWorld, "%s%s%s%s", responder, template1, buffer, template2);

    //Sends the game puzzle to the web browser.
    send(descriptor, gameWorld, strlen(gameWorld), 0);
}

//This function accepts the URL request from the client, parses the the request to get the
//the guess of the user. If there is no move to guess it returns 0 indicating it's the start of the game,
//if there is a move to guess with a matching word it returns 1 for success, and if its invalid it returns -1.
int acceptInput(char *putIn) {

    //Declaration of an array of characters of length 100.
    char input[100];
    memset(input, '\0', sizeof(input));

    //Declaration of a pointer to a String and is set to hold the substring containing 'move=' with the guess of
    //of the user after the substring.
    char *lockedIn = strstr(putIn, "move=");
    if(lockedIn != NULL)
        lockedIn += 5;
    else    
        return 0;

    //Declaration of an array of characters of length 100.
    char boundary[100];
    memset(boundary, '\0', sizeof(boundary));
    strcpy(boundary, lockedIn);
    capitalizeWords(boundary);
    boundary[strcspn(boundary, "\r\n")] = '\0';

    for(int n = 0; n < strlen(boundary); n++) {
        if(boundary[n] != '\n' && boundary[n] != '\r')
                input[n] = boundary[n];
    }

    //Declaration of a pointer and is assigned to point to the gameListNode type Linked List.
    gameListNode *gameHolder = firstGameNode;
    while(gameHolder != NULL) {
        if(compareStrings(input, gameHolder->stringForGame)) {
            gameHolder->hasBeenFound = true;
            return 1;
        }
        else
            gameHolder = gameHolder->next;
    }
    return -1;
}
//To count the number of instances of each letter and store it in the designated index of a given array,
//with the index corresponding to a letter in alphabetical order, such as index 0 would be the instances of A.
//Finally, it returns an array that represents the letter distribution of a given word.
int *getLetterDistribution(char *string, int *numberOfInstances) {
    for(int i = 0; i < strlen(string); i++) {
        switch(string[i]) {
            case 'A':
                numberOfInstances[0]++;
                break;
            case 'B':
                numberOfInstances[1]++;
                break;
            case 'C':
                numberOfInstances[2]++;   
                break;       
            case 'D':
                numberOfInstances[3]++;
                break;
            case 'E':
                numberOfInstances[4]++;      
                break; 
            case 'F':
                numberOfInstances[5]++;
                break;
            case 'G':
                numberOfInstances[6]++;
                break;
            case 'H':
                numberOfInstances[7]++;
                break;
            case 'I':
                numberOfInstances[8]++;
                break;
            case 'J':
                numberOfInstances[9]++;
                break;
            case 'K':
                numberOfInstances[10]++;
                break;
            case 'L':
                numberOfInstances[11]++;
                break;
            case 'M':
                numberOfInstances[12]++;    
                break;      
            case 'N':
                numberOfInstances[13]++;
                break;
            case 'O':
                numberOfInstances[14]++;     
                break;  
            case 'P':
                numberOfInstances[15]++;
                break;
            case 'Q':
                numberOfInstances[16]++;
                break;
            case 'R':
                numberOfInstances[17]++;
                break;
            case 'S':
                numberOfInstances[18]++;
                break;
            case 'T':
                numberOfInstances[19]++;
                break;
            case 'U':
                numberOfInstances[20]++;
                break;
            case 'V':
                numberOfInstances[21]++;
                break;
            case 'W':
                numberOfInstances[22]++;  
                break;        
            case 'X':
                numberOfInstances[23]++;
                break;
            case 'Y':
                numberOfInstances[24]++; 
                break;      
            case 'Z':
                numberOfInstances[25]++;
                break;
        }
    }
    return numberOfInstances;
}
//This will compare the two distributions from the dictionary and user input. This will return true
//if the candidate word can be made from the choices and false otherwise.
bool compareCounts(int *dictionary, int *userData) {

    //Declaration of local integer type variable and is set to 0.
    int counter = 0;
    for(int i = 0; i < 26; i++) {
        if(dictionary[i] < userData[i])
            counter++;
    }
    if(counter != 0)
        return false;
    else
        return true;
}

//This function takes in the total number of words from the wordListNode type Linked List and then
//randomly picks and returns the master word of interest, a word that is more than 6 characters long. In the
//case it doesn't find the desired word it returns a NULL value.
char *getRandomWord(int pass) {
    reverseOrder();

    //Declaration of a pointer and is assigned to point to the wordListNode type Linked List.
    wordListNode *temporary = firstWordNode;
    
    //Declaration of a variable of the integer type and it set to 0. To keep track of the
    //of the traversal of the wordListNode type Linked List.
    int tracker = 0;

    //Declaration of a variable of the integer type and is set to a random number between 0 and the total
    //number of words contained in the wordListNode type Linked List.
    int randomWord = rand() % pass;
    while(tracker != randomWord) {
        temporary = temporary->next;
        tracker++;
    }
    while(temporary != NULL) {
        if(strlen((temporary->stringForWord)) > 6)
            return temporary->stringForWord;
        else
            temporary = temporary->next;
    }
    return NULL;
}

//The function takes in the master word, then proceeds to find and add the words that can be made from the
//letters of the master word to the gameListNode type Linked List.
void findWords(char *taken) {
    reverseOrder();

    //Declaration of an array of characters of length 50.
    char copyString [50];
    strcpy(copyString, taken);
    //Declaration of a pointer and is assigned to point to the wordListNode type Linked List.
    wordListNode *compare = firstWordNode;

    //Declaration of an array of integers with the length of 26 and is set to 0.
    int instanceOfMasterWord[26] = {0};

    //Declaration of a pointer and is assigned to point to the letter distribution of the master word.
    int *ptOfMasterWord = getLetterDistribution(copyString, instanceOfMasterWord);
    while(compare != NULL) {

        //Declaration of an array of integers with the length of 26 and is set to 0.
        int instanceOfLookingForWords[26] = {0};

        //Declaration of a pointer and is assigned to point to the letter distribution of the word to find.
        int *ptOfLookingForWords = getLetterDistribution(compare->stringForWord, instanceOfLookingForWords);
        if(compareCounts(ptOfMasterWord, ptOfLookingForWords)) {
            addGameListNode(compare->stringForWord);
        }
        compare = compare->next;
    }
}
//This function takes in a pointer to a string and adds every new node of the wordListNode type to the
//beginning of the Linked List.
void addWordListNode(char *dataForWordList) {

    //This allocates memory to a node and assigns it to a pointer of the same type, which is wordListNode.
    wordListNode *node = (wordListNode *)malloc(sizeof(wordListNode));
    strcpy(node->stringForWord, dataForWordList);
    node->next = firstWordNode;
    firstWordNode = node;
}

 //This function takes in a pointer to a string and adds every new node of the gameListNode type to the
 //beginning of the Linked List.
void addGameListNode(char *dataForGameList) {

    //To filter out duplicates from being added to the gameListNode type Linked List.
    gameListNode *temporary = firstGameNode;
    while(temporary != NULL) {
        if(compareStrings(dataForGameList, temporary->stringForGame))
            return;
        temporary = temporary->next;
    }
    //This allocates memory to a node and assigns it to a pointer of the same type, which is gameListNode.
    gameListNode *current = (gameListNode *)malloc(sizeof(gameListNode));
    strcpy(current->stringForGame, dataForGameList);
    current->hasBeenFound = false;
    current->next = firstGameNode;
    firstGameNode = current;
}

//This frees the allocated memory for the wordListNode type Linked List, by freeing each of its nodes
//from the start to end. Which frees the memory associated with the dictionary.
void cleanupWordListNodes() {

    //Declaration of a pointer of the wordListNode type Linked List.
    wordListNode *holder;
    while(firstWordNode != NULL) {
        holder = firstWordNode->next;
        free(firstWordNode);
        firstWordNode = holder;
    }
}

//This frees the allocated memory for the gameListNode type Linked List, by freeing each of its nodes
//from the start to end. Which the memory associated with the current game.
void cleanupGameListNodes() {

    //Declaration of a pointer of the gameListNode type Linked List.
    gameListNode *holder2;
    while(firstGameNode != NULL) {
        holder2 = firstGameNode->next;
        free(firstGameNode);
        firstGameNode = holder2;
    }
}

//This function reverses the order of the wordListNode type Linked List, to set it from Z-A to A-Z in 
//alphabetical order. 
void reverseOrder() {

    //Declaration of a pointer and set to point to the wordListNode type Linked List.
    wordListNode *node = firstWordNode;

    //Declaration of a pointer of the wordListNode type Linked List and set to point to a NULL value.
    wordListNode *before = NULL;

    //Declaration of a pointer of the wordListNode type Linked List.
    wordListNode *next;
    while(node != NULL) {
        next = node->next;
        node->next = before;
        before = node;
        node = next;
    }
    firstWordNode = before;
}
//This implements the Selection Sort Algorithm, to sort the characters of a String in increasing order
//or in alphabetical order, from A to Z. Then it returns this newly sorted String.
char *sortString(char *stringHolder) {
    for(int i = 0; i < strlen(stringHolder) - 1; i++){

        //Declaration of a variable of the integer type and set to the current index.
        int minimum = i;
        for(int k = i + 1; k < strlen(stringHolder); k++){
            if(stringHolder[k] < stringHolder[minimum])
                minimum = k;
        }
    swapFunction(stringHolder, i, minimum);
    }
    return stringHolder;
}

//This swaps and moves the smallest element, character, in the correct index or position.
void swapFunction(char *stringHolder, int currentPosition, int minimum) {
    char temporary = stringHolder[currentPosition];
    stringHolder[currentPosition] = stringHolder[minimum];
    stringHolder[minimum] = temporary;
}

//This returns a line of underscores based on the given length of the String, only executes for the words
//that haven't been found by the user yet.
 char *printUnderScores(char *template, char *holdBlankLines) {
    for(int i = 0; i < strlen(template); i++)
        sprintf(holdBlankLines + strlen(holdBlankLines), "_ ");
    return holdBlankLines;
}

//This compares the user input and any word to be found based on the master word. It compares both words 
//by length and character by character, if both given words are the same the function returns true. But
//otherwise it returns false.
bool compareStrings(char *userInput, char *wordToFind) {
    if(strlen(userInput) != strlen(wordToFind))
        return false;
    for(int i = 0; i < strlen(userInput); i++) {
        if(userInput[i] != wordToFind[i])
            return false;
    }
    return true;
}
//This function takes in a String and capatilizes all of its characters.
void capitalizeWords(char *word) {
    for(int i = 0; i < strlen(word); i++)
        word[i] = toupper(word[i]);
}

//Creates a server socket, binds it with an address and port, and listens for incomming connections to the
//server from the client side. Then it proceeds to accept connections with the client, this also proceeds
//to create a thread per client request which recieves and sends data based on the client request,
//enabling for multithreading, but no more than 50 threads at a time.
void networkCreation(char *rootDirectory) {

    signal(SIGINT, signalToKill);
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

    //Declaration of an integer variable and is set to hold the number of words from the loaded master dictionary.
    int acceptNumber = initialization();
	while(1) {

        //Create client structure to get and store client socket, root directory, number of words from the 
        //loaded master dictionary, and to hold the current initialized state of the user input.
        clientHolder *clientInformation = malloc(sizeof(clientHolder));
		sin_size = sizeof(theirAddress);
		clientInformation->clientSocket = accept(serverSocket, (struct sockaddr *)&theirAddress, &sin_size);
        strcpy(clientInformation->rootFilePath, rootDirectory);
        clientInformation->numberOfWords = acceptNumber;
        clientInformation->guessValidOrNot = 0;

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
char *newParseFunction(char *String) {

    //Declaration of a pointer to a String and is set to current URL.
    char *helm = String;

    //Declaration of a pointer to a String and is set to hold the first parsed String, which is "GET".
    char *str = strtok_r(helm, " ", &helm);

    //Declaration of a pointer to a String and is set to hold the second parsed String, which is the file path.
    char *holder = strtok_r(helm, " ", &helm);
    String = holder;
    return String;
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
    char *holderPointer = newParseFunction(data);
    if(holderPointer[0] == '/')
        holderPointer++;
        
    //Declaration of an array of characters of length 20.
    char taker[20];
    memset(taker, '\0', sizeof(taker));
    strcpy(taker, holderPointer);
    taker[5] = '\0';

    //Declaration of an array of characters of length 20 and set to hold the 'words' String literal.
    char stringToCompare[20] = "words";

    //To play the Words Without Friends Web Game.
    if(compareStrings(taker, stringToCompare)) {

        //Declaration of an integer variable and is set to the current state of the user input.
        int toValidate = retrieveClientInfo->guessValidOrNot;

        //Declaration of an integer variable and is set to the number of words form the loaded master
        //dictionary.
        int retrieve = retrieveClientInfo->numberOfWords;

        //Declaration of a pointer to a String.
        char *test;
        toValidate = acceptInput(holderPointer);

        //To check if a move by the user does not exist. If its true then it checks to see if there is a 
        //current game session, which is freed, and then it generates a new master word along with a
        //new game game puzzle.
        if(toValidate == 0) {
                    if(firstGameNode != NULL)  
                        cleanupGameListNodes();
            test = getRandomWord(retrieve);
            findWords(test);
        }

        //Checks to see if the game is completed, it keeps displaying the game state
        //while the puzzle is not completed yet.
        if(!isDone())
            displayWorld(test, retrieveClientInfo->clientSocket, toValidate);
        
        //Once the game is completed, it erases the game and sends a final congratulatory message to the user
        //for successfully finishing the game along with a link to start a new game if the user
        //wants to keep playing.
        else {
            cleanupGameListNodes();

            //Declaration of a pointer to a String and is set to hold the congratulatory message in HTML form.
            char *body = "<html><body style=\"background-color: blue; color: white;\"><h1 style=\"border: 4px dashed gray; font-size: 32px; text-align: center; color: black;\">Congratulations! You solved it! <a href=\"words\">Another?</a></h1></body></html>";

            //Declaration of a pointer to a String and is set to hold the HTTP header '200 OK'.
            char *header = "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8 \r\n\r\n";

            //Declaration of an array of characters of length 1000.
            char congratulations[1000];

            //Combines and creates the HTML form for the congratulatory message to the user.
            sprintf(congratulations, "%s%s", header, body);

            //Sends the congratulatory HTML message to the web browser.
            send(retrieveClientInfo->clientSocket, congratulations, strlen(congratulations), 0);
        }
        close(retrieveClientInfo->clientSocket);
        free(retrieveClientInfo); 
        pthread_exit(NULL); 
    }

    //Otherwise to serve files.

    //An array of characters of size 1200.
    char assembledPath[1200];
    //Combines the file paths to the absolute file path to be
    //opened and processed.
    sprintf(assembledPath, "%s/%s", homeDirectory, holderPointer);
    int file = open(assembledPath, O_RDONLY);
    if(file == -1) {

        //Declaration of an array of characters of length 1000 and is set to hold the response if the file
        //being requested for does not exist.
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