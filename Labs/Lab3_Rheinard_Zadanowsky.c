#include <stdio.h>
#include <string.h>

//The beggining of the program.
int main() {

    //The local variable length of the integer type, to count the number of elements
    //that has values in an array of characters and is set to 0.
    int length = 0;

    //The array of characters to store the user input of size 100.
    char userInput[100];
    
    //Prompts the user to enter an input.
    printf("Enter any word: ");

    //Reads input from the user as a string and stores the string in an array of characters.
    scanf("%s", userInput);
    
    //Prints the string stored in an array of characters.
    printf("Your input is %s\n", userInput);

    //Local variable of the integer type set to 0.
    int i = 0;

    //The loop checks to see if each element in an array of characters has a value,
    //then the value of length is incremented by 1. When the element has no value
    //the loop ends.
    while(userInput[i] != '\0'){
        length++;
        i++;
    }

    //Prints the calculated length of the string and the length of the string using
    //a standard function such as strlen().
    printf("The length of the string without using the standard functions is %d\n", length);
    printf("The length of the string using the standard function strlen() is %zu\n", strlen(userInput));

    return 0;
}