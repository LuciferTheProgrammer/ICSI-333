#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int num;
    char boundary[5];
    FILE *file = fopen("numbers", "rb");
    while(fread(&num, sizeof(int), 1, file) == 1) {
        sprintf(boundary, "%d", num);
        printf("%s\n", boundary);
    }
    fclose(file);
    return 0;
}