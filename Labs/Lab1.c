#include <stdio.h>

//The beginning of the program.
int main() 
{
    //Local variable declared as an integer type.
    int number1;

    //Local variable declared as an integer type.
    int number2;

    //Prompts user to input the first number.
    printf("Please put in the first number: ");

    //Stores the user input in variable number1 as an integer type.
    scanf("%d",&number1);

    //Prompts user to input the second number.
    printf("Please put in the second number: ");

    //Stores user input in variable number2 as an integer type.
    scanf("%d",&number2);

    //Prints the numbers stored in variables number1 and number2.
    printf("The first number is %d\n", number1);
    printf("The second number is %d\n", number2);

    //Prints the quotient and the remainder of the variables number1 and number2.
    printf("The quotient looks like this: %d\n", (number1 / number2));
    printf("The remainder looks like this: %d\n", (number1 % number2));
    return 0;
}