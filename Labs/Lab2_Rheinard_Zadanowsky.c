#include <stdio.h>

//The increment function which takes a variable of the integer type and adds 1 value to the
//number stored inside.
void increment(int x) {
    x++;
}

//The incrementByReference function which takes a pointer variable of the integer type,
//dereferences it, and adds 1 value to the number stored inside.
void incrementByReference(int *x) {
    (*x)++;
}

//The beginning of the program.
int main () {

    //Declaration of the variable a of the integer type and set to 0.
    int a = 0;

    //Declaration of the variable b of the integer type and set to 0.
    int b = 0;

    //Declaration of the pointer variable pa of the integer type
    //and set to point to the same memory that a does.
    int *pa = &a;

    //Declaration of the pointer variable pb of the integer type
    //and set to point to the same memory that b does.
    int *pb = &b;

    //Set the value 5 to the variable a.
    a = 5;

    //Dereferences the pointer variable pb by changing its value to 6.
    *pb = 6;

    //Prints the 4 values of a, b, pa, and pb before incrementing, after incrementing b, and
    //incrementing pb. The pointer pb changed the value of b from 0 to 6 because pb has the memory
    //address of b stored. This directly leads the pointer pb to access b and change it's value through
    //dereferencing. So, when the function incrementByReference(pb) is called it directly
    //changes the values of b and pb from 6 to 7.
    //However, when the function increment(b) is called it doesn't change the values of
    //b and pb.
    printf("The values of a, b, pa, and pb before incrementing are %d, %d, %d, and %d\n", a, b, *pa, *pb);
    increment(b);
    printf("The values of a, b, pa, and pb when incrementing b are %d, %d, %d, and %d\n", a, b, *pa, *pb);
    incrementByReference(pb);
    printf("The values of a, b, pa, and pb when incrementing pb are %d, %d, %d, and %d\n", a, b, *pa, *pb);
    
    return 0;
}