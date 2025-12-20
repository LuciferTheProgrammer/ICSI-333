#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct wordListNode{
    char stringForWord[30];
    struct wordListNode *next;
}wordListNode;

typedef struct gameListNode{
    char stringForGame[30];
    bool hasBeenFound;
    struct gameListNode *next; 
}gameListNode;

gameListNode *firstGameNode;

wordListNode *firstWordNode;

//The array of characters to hold the user input of size 100.
char input[100];

//The array of characters to hold a given dictionary.
char DICTIONARY[] = "ADEFNORRSSW";

//The array of characters to hold the alphabet of size 26.
char alphabet [26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int initialization();
void gameLoop();
void tearDown();
void displayWorld();
char *acceptInput();
int *getLetterDistribution(char *x, int *numberOfInstances);
bool compareCounts(int *dictionary, int *userData);
wordListNode* getRandomWord(int pass);
void findWords(wordListNode *taken);
void addWordListNode(char *s);
void addGameListNode(char *s);
void freeMemory1();
void freeMemory2();
void reverseOrder();

//To load the dictionary into memory and will return the number of words in the dictionary.But for
//now it will return 0.
int initialization() {
    srand(time(NULL));
    FILE *file;
    char container[30];
    int counter = 0;
    file = fopen("2of12.txt", "r");
    while(!feof(file)){ 
        fgets(container, sizeof(container), file);
        container[strcspn(container, "\r\n")] = 0;
        addWordListNode(container);
        counter++;
        memset(container, '\0', sizeof(container));
    }
    fclose(file);
    printf("when adding nodes %d\n", counter);
    return counter;
}
//This is word game itself, will stop looping when the function isDone() returns true.
void gameLoop() {
    do {

    //An array of integers of size 26 and set to 0 to represent the user letter distribution.
    int frequenciesOfUserInput[26] = {0};

    //An array of integers of size 26 and set to 0 to represent the dictionary letter distribution.
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
    //An array of characters of size 100 to hold current user input.
    char boundary[100];
    fgets(boundary, sizeof(boundary), stdin);
    for(int i = 0; i < strlen(boundary); i++)
        boundary[i] = toupper(boundary[i]);
    boundary[strcspn(boundary, "\r\n")] = '\0';
    for(int n = 0; n < strlen(boundary); n++){
        if(boundary[n] != '\n' && boundary[n] != '\r')
                input[n] = boundary[n];
    }
    printf("Your guess is %s\n", input);
    return input;
}
//To count the number of instances of each letter and store it in the designated index of a given array,
//with the index corresponding to a letter in alphabetical order, such as index 0 would be the instances of A.
//Finally, it returns an array that represents the letter distribution of a given word.
int *getLetterDistribution(char *x, int *numberOfInstances) {
    for (int i = 0; i < strlen(x); i++){
        for(int s = 0; s < strlen(alphabet); s++){
            if(toupper(x[i]) == alphabet[s]){
                numberOfInstances[s]++;
            }
        }
    }
    return numberOfInstances;
}
//This will compare the two distributions from the dictionary and user input. This will return true
//if the candidate word can be made from the choices and false otherwise.
bool compareCounts(int *dictionary, int *userData){

    //Declaration of local integer type variable and is set to 0.
    int counter = 0;
    for(int i = 0; i < 26; i++){
        if(dictionary[i] < userData[i])
            counter++;
    }
    if(counter != 0)
        return false;
    else
        return true;
}

wordListNode* getRandomWord(int pass){
    reverseOrder();
    wordListNode *temporary = firstWordNode;
    int tracker = 0;
    //int randomWord = rand() % pass;
    int randomWord = 38529;
    while(tracker != randomWord){
        temporary = temporary->next;
        tracker++;
    }
    //To print correct position based on the 2of12.txt document.
    tracker++;
    printf("The current word %s is < 6 characters with a length of %ld\n", temporary->stringForWord, strlen(temporary->stringForWord));
   while(temporary!= NULL){
    if(strlen((temporary->stringForWord)) > 6){
        printf("The current word %s is > 6 characters with a length of %ld\n", temporary->stringForWord, strlen(temporary->stringForWord));
        return temporary;
    } 
    else
        temporary = temporary->next;
   }
   return NULL;
}
//This will print the puzzle, but for now will print a line of hyphens in the screen.
void displayWorld() {
    printf("------------------------------------------------------------\n");
    printf("Given Dictionary: %s\n", DICTIONARY);
}

void findWords(wordListNode *taken){
    wordListNode *compare = firstWordNode;
    int instanceOfMasterWord[26] = {0};
    int *ptOfMasterWord = getLetterDistribution(taken->stringForWord, instanceOfMasterWord);
    printf("The current master word is %s\n", taken->stringForWord);
    while(compare != NULL){
        int instanceOfLookingForWords[26] = {0};
        int *ptOfLookingForWords = getLetterDistribution(compare->stringForWord, instanceOfLookingForWords);
        if(compareCounts(ptOfMasterWord, ptOfLookingForWords)){
            addGameListNode(compare->stringForWord);
        }
        compare = compare->next;
    }
}

void addWordListNode(char *s){
    wordListNode *node = (wordListNode *)malloc(sizeof(wordListNode));
    strcpy(node->stringForWord, s);
    node->next = firstWordNode;
    firstWordNode = node;
}

void addGameListNode(char *s){
   gameListNode *current = (gameListNode *)malloc(sizeof(gameListNode));
    strcpy(current->stringForGame, s);
    current->next = firstGameNode;
    firstGameNode = current;
}

void cleanupWordListNodes(){
    wordListNode *holder;
    while(firstWordNode != NULL){
        holder = firstWordNode->next;
        free(firstWordNode);
        firstWordNode = holder;
    }
}

void cleanupGameListNodes(){
    gameListNode *holder2;
    while(firstGameNode != NULL){
        holder2 = firstGameNode->next;
        free(firstGameNode);
        firstGameNode = holder2;
    }
}

void reverseOrder(){
    wordListNode *node = firstWordNode;
    wordListNode *before = NULL;
    wordListNode *next;
    while(node != NULL){
        next = node->next;
        node->next = before;
        before = node;
        node = next;
    }
    firstWordNode = before;
}
//The beggining of the program.
int main() {
    wordListNode *test = getRandomWord(initialization());
    printf("The random word generated is %s\n", test->stringForWord);
    findWords(test);
    gameListNode *took = firstGameNode;
    while(took != NULL){
        printf("%s\n", took->stringForGame);
        took = took->next;
    }
    gameLoop();
    tearDown();
    cleanupWordListNodes();
    cleanupGameListNodes();
    return 0;
}
