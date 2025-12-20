#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

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

//A pointer to the wordListNode type Linked List that contains words that is read from a file.
wordListNode *firstWordNode;

//A pointer to the gameListNode type Linked List that contains words that can be made up from the master word.
gameListNode *firstGameNode;

//The array of characters to hold the user input of length 100.
char input[100];

//The array of characters to hold a given dictionary.
char DICTIONARY[] = "ADEFNORRSSW";

//Declaration of functions to be used in the program with specific return types and parameter types.

int initialization();
void gameLoop();
bool isDone();
void tearDown();
void displayWorld();
char *acceptInput();
int *getLetterDistribution(char *string, int *numberOfInstances);
bool compareCounts(int *dictionary, int *userData);
char *getRandomWord(int pass);
void findWords(char *taken);
void addWordListNode(char *dataForWordList);
void addGameListNode(char *dataForGameList);
void cleanupWordListNodes();
void cleanupGameListNodes();
void reverseOrder();

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
    while(fgets(container, sizeof(container), file) != NULL) { 
        container[strcspn(container, "\r\n")] = 0;

        //To detect and filter out the last empty line from the file.
        if(strlen(container) == 0)
            break;
        addWordListNode(container);
        counter++;
        memset(container, '\0', sizeof(container));
    }
    fclose(file);
    printf("The total number of words is %d\n", counter);
    return counter;
}
//This is word game itself, will stop looping when the function isDone() returns true.
void gameLoop() {
    do {
        //An array of integers of length 26 and set to 0 to represent the user letter distribution.
        int frequenciesOfUserInput[26] = {0};

        //An array of integers of length 26 and set to 0 to represent the dictionary letter distribution.
        int frequenciesOfDictionary[26] = {0};

        displayWorld();

        //A pointer to an array of characters, which points to the user input.
        char *holder = acceptInput();

        //A pointer to an array of integers, which points to the user letter distribution.
        int *instanceOfUserInput = getLetterDistribution(holder, frequenciesOfUserInput);

        //A pointer to an array of integers, which points to the dictionary letter distribution.
        int *instanceOfDICTIONARY = getLetterDistribution(DICTIONARY, frequenciesOfDictionary);

        if(compareCounts(instanceOfDICTIONARY, instanceOfUserInput))
            printf("FOUND\n");
        else    
            printf("NOT FOUND\n");
        memset(input, '\0', sizeof(input));
    } while(!isDone());
}
//This returns true when all the possible words that can be formed within a given dictionary
//has been guessed by the user and false otherwise. But for now it only returns true.
bool isDone() {
    return true;
}
//Prints the message "All Done" in the screen, indicating the game is finished.
void tearDown() {
    printf("All Done!\n");
}
//This will print the puzzle, but for now will print a line of hyphens in the screen.
void displayWorld() {
    printf("------------------------------------------------------------\n");
    printf("Given Dictionary: %s\n", DICTIONARY);
}
//This will prompt the user to enter an input and returns a string represented by an array of 
//characters that is capitalized and stripped off of newline feeds and carriage returns.
char *acceptInput() {
    printf("Enter a guess: ");
    //An array of characters of length 100 to hold current user input.
    char boundary[100];
    fgets(boundary, sizeof(boundary), stdin);
    for(int i = 0; i < strlen(boundary); i++)
        boundary[i] = toupper(boundary[i]);
    boundary[strcspn(boundary, "\r\n")] = '\0';
    for(int n = 0; n < strlen(boundary); n++) {
        if(boundary[n] != '\n' && boundary[n] != '\r')
                input[n] = boundary[n];
    }
    printf("Your guess is %s\n", input);
    return input;
}
//To count the number of instances of each letter and store it in the designated index of a given array,
//with the index corresponding to a letter in alphabetical order, such as index 0 would be the instances of A.
//Finally, it returns an array that represents the letter distribution of a given word.
int *getLetterDistribution(char *string, int *numberOfInstances) {
    for(int i = 0; i < strlen(string); i++) {
        switch(toupper(string[i])) {
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
    int randomWord = 15363;
    char *str = "halberd";
    //int randomWord = rand() % pass;
    while(temporary != NULL) {
        if(strcmp(temporary->stringForWord, str) == 0)
            return (temporary->stringForWord);
        temporary = temporary->next;
        //tracker++;
    }
    return(temporary->stringForWord);
    //while(temporary != NULL) {
    //    if(strlen((temporary->stringForWord)) > 6)
    //        return temporary->stringForWord;
    //    else
    //        temporary = temporary->next;
    //}
    //return NULL;
}

//The function takes in the master word, then proceeds to find and add the words that can be made from the
//letters of the master word to the gameListNode type Linked List.
void findWords(char *taken) {
    reverseOrder();

    //Declaration of a pointer and is assigned to point to the wordListNode type Linked List.
    wordListNode *compare = firstWordNode;

    //Declaration of an array of integers with the length of 26 and is set to 0.
    int instanceOfMasterWord[26] = {0};

    //Declaration of a pointer and is assigned to point to the letter distribution of the master word.
    int *ptOfMasterWord = getLetterDistribution(taken, instanceOfMasterWord);
    while(compare != NULL) {

        //Declaration of an array of integers with the length of 26 and is set to 0.
        int instanceOfLookingForWords[26] = {0};

        //Declaration of a pointer and is assigned to point to the letter distribution of the word to find.
        int *ptOfLookingForWords = getLetterDistribution(compare->stringForWord, instanceOfLookingForWords);
        if(compareCounts(ptOfMasterWord, ptOfLookingForWords))
            addGameListNode(compare->stringForWord);
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

    //This allocates memory to a node and assigns it to a pointer of the same type, which is gameListNode.
    gameListNode *current = (gameListNode *)malloc(sizeof(gameListNode));
    strcpy(current->stringForGame, dataForGameList);
    current->next = firstGameNode;
    firstGameNode = current;
}

//This frees the allocated memory for the wordListNode type Linked List, by freeing each of its nodes
//from the start to end.
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
//from the start to end.
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
//The beggining of the program.
int main() {

    //Declaration of a pointer to a string.
    char *test;
    
    //Declaration of an integer type variable and is set to store the total number of words.
    int retrieve = initialization();
    do { 
        test = getRandomWord(retrieve);
    } while(test == NULL);
    printf("The random word generated is %s\n", test);
    printf("The matching words are: \n");
    findWords(test);

    //Declaration of a pointer and is assigned to point to the gameListNode type Linked List.
    gameListNode *took = firstGameNode;
    while(took != NULL) {
        printf("%s\n", took->stringForGame);
        took = took->next;
    }
    gameLoop();
    tearDown();
    cleanupWordListNodes();
    cleanupGameListNodes();
    return 0;
}
