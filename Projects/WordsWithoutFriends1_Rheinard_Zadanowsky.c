#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

//The array of characters to hold the user input of size 100.
char input[100];

//The array of characters to hold a given dictionary.
char DICTIONARY[] = "ADEFNORRSSW";

//The array of characters to hold the alphabet of size 26.
char alphabet [26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

//To load the dictionary into memory and will return the number of words in the dictionary.But for
//now it will return 0.
int initialization() {
    srand(time(NULL));
    return 0;
}

//This will print the puzzle, but for now will print a line of hyphens in the screen.
void displayWorld() {
    printf("------------------------------------------------------------\n");
    printf("Given Dictionary: %s\n", DICTIONARY);
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

//To count the number of instances of each letter and store it in the designated index of a given array,
//with the index corresponding to a letter in alphabetical order, such as index 0 would be the instances of A.
//Finally, it returns an array that represents the letter distribution of a given word.
int *getLetterDistribution(char *x, int *numberOfInstances) {
    for (int i = 0; i < strlen(x); i++){
        for(int s = 0; s < strlen(alphabet); s++){
            if(x[i] == alphabet[s]){
                numberOfInstances[s]++;
            }
        }
    }
    return numberOfInstances;
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

//This returns true when all the possible words that can be formed within a given dictionary
//has been guessed by the user and false otherwise. But for now it only returns true.
bool isDone() {
    return true;
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

    //To set the global array of characters as an empty string before the next iteration.
    memset(input, '\0', sizeof(input));
    } while(!isDone());
}

//Prints the message "All Done" in the screen, indicating the game is finished.
void tearDown() {
    printf("All Done!\n");
}

//The beggining of the program.
int main() {
    initialization();
    gameLoop();
    tearDown();
    return 0;
}