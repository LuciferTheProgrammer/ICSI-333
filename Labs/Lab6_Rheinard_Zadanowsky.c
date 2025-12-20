#include <stdio.h>

int main() {

   //Prompts the user to enter a number. 
    printf("Enter a number: ");

    //Declaration of a variable of the integer type.
    int input;

    //Accepts the user input and stores it in a variable.
    scanf("%d", &input);

    //Prints the user input in terms of an integer number.
    printf("Your number was %d\n", input);

    //Declaration of an unsigned variable of the integer type and is set to the value stored in the variable
    //input.
    unsigned int holder = (unsigned)input;
    
    //Declaration of an unsigned variable of the integer type and is set to 0.
    unsigned int counterBit1 = 0;

    //The loop to calculate the number of bits set to 1 in the binary form of an integer number.
    while(holder) {
        counterBit1 += holder & 1;
        holder >>= 1;
    } 

    //Declaration of an unsigned variable of the integer type and is set to the total number of bits of 32.
    unsigned int total = (sizeof(unsigned int) * 8);

    //Declaration of an unsigned variable of the integer type and is set to the number of bits that are 0.
    unsigned int counterBit0 = total - counterBit1;

    //Prints the number of bits that are set to 0 and 1 based on the integer number
    //the user inputs.
    printf("In %d, there are %u bits set to 0.\n", (int)input, counterBit0);
    printf("In %d, there are %u bits set to 1.\n", (int)input, counterBit1);
    return 0;
}