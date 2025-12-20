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
wordListNode *firstWordNode = NULL;

//A pointer to the gameListNode type Linked List that contains words that can be made up from the master word.
gameListNode *firstGameNode = NULL;

//The array of characters to hold the user input of length 100.
char input[100];

//Declaration of functions to be used in the program with specific return types and parameter types.

int initialization();
void gameLoop(char *masterWord);
bool isDone();
void tearDown();
void displayWorld(char *masterWord);
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
char *sortString(char *stringHolder);
void swapFunction(char *stringHolder, int currentPosition, int minimum);
void printUnderScores(char *template);
bool compareStrings(char *userInput, char *wordToFind);
void capitalizeWords(char *word);

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
//This is word game itself, it will stop looping once all the words that are derived from the master word
//has been guessed by the user, when isDone() returns true. 
void gameLoop(char *masterWord) {
    do {
        displayWorld(masterWord);
        
        //Declaration of a pointer to a String and is set to NULL.
        char *holder = NULL;

        //To validate the user input to ensure that the user inputs the correct guess.
        do {
            holder = acceptInput();
            if(holder == NULL)
                printf("INVALID GUESS\n");
        } while(holder == NULL);
        printf("------------------------------------------------------------\n");

        //This is meant to display the final updated version of the game, when all the words
        //has been guessed by the user before exiting out.
        if(isDone())
            displayWorld(masterWord);
    } while(!isDone());
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
//This will print the display of the puzzle game, by first showing the alphabetically sorted master word, 
//the words to find based on the master word which are in hyphens, and the words that are correctly found
//by the user.
void displayWorld(char *masterWord) {

    //Declaration of a pointer to a String and is set to point to the sorted master word.
    char *placement = sortString(masterWord);
    for(int i = 0; i < strlen(placement); i++)
        printf("%c ", placement[i]);
    printf("\n");
    printf("------------------------------------------------------------\n");
    
    //Declaration of a pointer and is assigned to point to the gameListNode type Linked List.
    gameListNode *took = firstGameNode;
    while(took != NULL) {
        if(took->hasBeenFound)
            printf("FOUND: %s\n", took->stringForGame);
        else {
            printUnderScores(took->stringForGame);
            printf("\n");
        }
        took = took->next;
    }
}
//This will prompt the user to enter an input that is a String represented by an array of 
//characters that is capitalized and stripped off of newline feeds and carriage returns. While
//also checking to see if the user input is a match for any words to be found and if it is then it
//marks that specific word has been found and it returns the user input.
char *acceptInput() {
    printf("Enter a guess: ");
    //An array of characters of length 100 to hold current user input.
    char boundary[100];
    fgets(boundary, sizeof(boundary), stdin);
    capitalizeWords(boundary);
    boundary[strcspn(boundary, "\r\n")] = '\0';
    memset(input, '\0', sizeof(input));
    for(int n = 0; n < strlen(boundary); n++) {
        if(boundary[n] != '\n' && boundary[n] != '\r')
                input[n] = boundary[n];
    }

    //Declaration of a pointer and is assigned to point to the gameListNode type Linked List.
    gameListNode *gameHolder = firstGameNode;
    while(gameHolder != NULL) {
        if(compareStrings(input, gameHolder->stringForGame)) {
            gameHolder->hasBeenFound = true;
            return input;
        }
        else
            gameHolder = gameHolder->next;
    }
    return NULL;
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
    int randomWord = 24876;
    //int randomWord = rand() % pass;
    while(tracker != randomWord) {
        temporary = temporary->next;
        tracker++;
    }
    return temporary->stringForWord;
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

//This prints a line of hyphens based on the given length of the String, only executes for the words
//that haven't been found by the user yet.
 void printUnderScores(char *template) {
    for(int i = 0; i < strlen(template); i++)
        printf("_ ");
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

//The beggining of the program.
int main() {

    //Declaration of a variable of the character type to hold the user's
    //choice whether to keep playing or not.
    char choice;

    //To keep playing games if the user chooses to do so.
    do {
        //Declaration of an integer type variable and is set to store the total number of words.
        int retrieve = initialization();
    
        //Declaration of a pointer to a String.
        char *test;

        //To test and ensure that the random word generated is of interest, more than 6 characters.
        do { 
            test = getRandomWord(retrieve);
        } while(test == NULL);
        printf("The random word is %s\n", test);
        findWords(test);
        gameLoop(test);
        tearDown();
        cleanupGameListNodes();
        cleanupWordListNodes();
        
        //This prompts the user to choose whether to play another game or not.
        printf("Do you want to keep playing? 'Y' for yes and 'N' for no: ");

        //To read the user input and store it in the choice variable.
        scanf("%c", &choice);

        //This flushes or clears the input buffer.
        while((getchar()) != '\n');
    } while(toupper(choice) == 'Y');
    return 0;
}
