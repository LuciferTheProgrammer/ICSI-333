#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

//The beginning of the program.
int main(void) {

    //An array of integers of length of 100 and set to 0.
    int arrayRandomNum[100] = {0};

    //Declaration of a variable of the integer type and set to 0. To hold the sum of a 100 random numbers from
    //0 to 99.
    int sum = 0;

    //Declaration of a variable of the integer type and set to 0.
    int i = 0;

    //Initialize the random number system with a seed value.
    srand(time(0));

    //To iterate through the array of integers of length 100 by storing a random number generated from
    //0 to 99 to each index of the array as an element. This also sums the 100 random numbers that were
    //generated.
    while(i < 100) {
        arrayRandomNum[i] = rand() % 100;
        sum += arrayRandomNum[i];
        i++;
    }
    //Declaration of an array of characters of length 50.
    char holder[50];
    sprintf(holder, "numbers.%d", sum);
    
    //Declaration of a file pointer to open an existing file or create a file to write binary data to it.
    FILE *file = fopen(holder, "wb");

    //Writes the contents of the array of integers in binary data to a file.
    fwrite(arrayRandomNum, sizeof(int), 100, file);
    fclose(file);
    printf("The sum is %d.\n", sum);
    return 0;
}
