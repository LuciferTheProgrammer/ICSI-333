#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>


//Declaration of a variable of the integer type and set to 0.
int carrier = 0;

//Declaration of the functions to be used in the program with specific return types and parameter types.

void toSumNumbersFile(char *nameOfFile);

bool checkString(char *data1);

//The beginning of the program.
int main() {

    //Declaration of a pointer to a directory entry.
    struct dirent *pDirectory;

    //Declaration of a pointer to a DIR type and is set to point to a DIR type that is returned
    //from opendir(), which is the current directory.
    DIR *directory = opendir(".");

    //To check if the directory can be opened. If it can't it prints an error message indicating that 
    //no such file directory exist.
    if(directory == NULL) {
        printf("There was an error: %s\n", strerror(errno));
        exit(1);
    }

    //To iterate through the directory entries while checking if each file starts with the entry or name of
    //"numbers.". If it does, the loop terminates.
    while((pDirectory = readdir(directory)) != NULL) {
        
        //Declaration of an array of characters of length 30.
        char placement[30];
        if(strlen(pDirectory->d_name) == 12) {
            strcpy(placement, pDirectory->d_name);
            if(checkString(placement)) {
                toSumNumbersFile(pDirectory->d_name);
                break;
            }
        }
    }
    closedir(directory);
    if(carrier == 0)
        printf("There is no such file named: numbers.XXXX\n");
    return 0;
}

//To open a file and read the binary data from it, of which are integer numbers. While also getting the total 
//of the integer numbers, then it proceeds to close the file and prints the name of the file and the addded sum.
void toSumNumbersFile(char *nameOfFile) {

    //Declaration of a variable of the integer type.
    int number;

    //Declaration of a variable of the integer type and set to 0. This is also to hold the sum of the
    //integer numbers.
    int sum = 0;

    //Declaration of a file pointer and set to point to a file for reading binary data from it.
    FILE *file = fopen(nameOfFile, "rb");
    if(file == NULL) {
        printf("The file does not exist!\n");
        exit(1);
    }
    while(fread(&number, sizeof(int), 1, file) == 1) {
            sum += number;
    }
    fclose(file);
    printf("The name of the file is %s with a sum of %d.\n", nameOfFile, sum);
    carrier++;
}
//The function to check if the file name entry starts with "numbers." by checking its characters.
//If it is a match it returns true and if it's not, it returns false.
bool checkString(char *data1) {
    
    //Declaration of an array of characters of length 30.
    char holder1[30];
    
    //Declaration of an array of characters of length 30.
    char holder2[30] = "numbers.";
    strncpy(holder1, data1, 8);
    holder1[8] = '\0';
    for(int i = 0; i < 8; i++) {
        if(holder1[i] != holder2[i])
            return false;
    }
return true;
}