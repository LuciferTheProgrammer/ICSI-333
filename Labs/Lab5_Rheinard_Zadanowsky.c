#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//The structure to create a node for a Linked List to hold a random number
//between 0 and 50 and a pointer to the next node.
typedef struct randomNumberNode{
    int data;
    struct randomNumberNode *next;
}randomNumberNode;

//The pointer to the Linked List that hold random numbers between 0 and 50.
randomNumberNode *headerNode;

//Declaration of the method to add random numbers between 0 and 50 to the Linked List.
void addRandomNumberNode(int x);

//This function adds nodes that contain random numbers between 0 and 50 to the beginning
//of the Linked List.
void addRandomNumberNode(int number){
    
    //This allocates memory to a node and assigns it to a pointer of the same type, which is randomNumberNode.
    randomNumberNode *currentNode = (randomNumberNode *)malloc(sizeof(randomNumberNode));
    currentNode->data = number;
    currentNode->next = headerNode;
    headerNode = currentNode;
}

//The beginning of the program.
int main(){

    //Initialize the random number system with a seed value.
    srand(time(0));

    //Declaration of a variable of the integer type and assign it a generated random number
    //between 0 and 50.
    int randomNumber = rand() % 51;

    //This loop keeps adding random numbers generated between 0 and 50 to the Linked List
    //until the random number generated is 49, then it terminates.
    while(randomNumber!= 49){
        addRandomNumberNode(randomNumber); 
        randomNumber = rand() % 51;
    }

    //Declaration of a pointer to a node of the randomNumberNode type.
    randomNumberNode *holder;

    //The loops frees the memory allocated to the nodes of the Linked List.
    while(headerNode != NULL){
        printf("This is the random number generated: %d\n", headerNode->data);
        holder = headerNode->next;
        free(headerNode);
        headerNode = holder;
    }
    return 0;
}