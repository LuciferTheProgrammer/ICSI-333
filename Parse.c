#include <stdio.h>
#include <string.h>

char *parseHeader(char *String) {
    char *stringToParse;
    char *holder = String;
    stringToParse = strtok_r(holder, "GET", &holder);
	stringToParse++;
	stringToParse[strlen(stringToParse) - 2] = '\0';
    return(stringToParse);
}

int main() {
char str[] = "GET /mnt/c/Users/David/Downloads/text of 2.html HTTP/1.1";

char *holder = parseHeader(str);
printf("%s\n", holder);
return 0;
}