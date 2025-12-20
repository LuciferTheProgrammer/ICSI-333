#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//The beginning of the program.
int main() {

//Declaration of a file pointer variable.
FILE *fp;

//The array of characters that stores the read string until the next line.
char holder[50];

//Opens the file for reading.
fp = fopen("Test.txt", "r");

//Prints the data contained in the file.
printf("The contents of the file are:\n");

//Reads and prints the data contained in the file line by line using a loop.
while(fgets(holder, sizeof(holder), fp)){
    printf("%s", holder);
}
printf("\n");
//Closes the file.
fclose(fp);
return 0;
}

