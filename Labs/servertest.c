#include <stdio.h>

int main() {
    int array [1000000];
    int counter = 0;

    for(int i = 0; i < 1000000; i++) {
        array[i] = 9;
        counter += array[i];
    }    
    printf("The MAX value is %d.\n", counter);
    return 0;
}