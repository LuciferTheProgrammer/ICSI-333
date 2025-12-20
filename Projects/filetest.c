#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 char *newParseFunc(char *String) {
    char *helm = String;
    char *str = strtok_r(helm, " ", &helm);
    char *help1 = str;
    printf("help1 is %s\n", help1);
    char *holder = strtok_r(helm, " ", &helm);
    char *help2 = holder;
    printf("help2 is %s\n", help2);
    return help2;
    }
char *parseHeader(char *String) {

    //Pointer to a String.
    char *stringToParse;

    //Pointer to a String.
    char *holder = String;
    stringToParse = strtok_r(holder, "GET ", &holder);
    return(stringToParse);
}

int main () {
    
    char input[100] = "GET /Downloads/WordsWithoutFriends4_Rheinard_Zadanowsky.c HTTP/1.1";
    char *taker = newParseFunc(input);
    printf("Before incrementing: %s\n", taker);
    if(taker[0] == '/')
        taker++;
    printf("After incrementing: %s\n", taker);
    /*char input2[100];
    strcpy(input2, input);
    printf("This is the copied request %s\n", input2);
    char *new = newParseFunc(input);
    char *new2 = parseHeader(input2);
    printf("From newParseFunc() %s\n", new); 
    printf("From parseHeader() %s\n", new2);
 */
    return 0;
}