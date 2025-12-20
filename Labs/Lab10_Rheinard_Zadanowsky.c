#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

//Threaded sum.
int threadCounter = 0;

//Size of array.
#define SIZE 1000000

//Global integer array of size 1000000. 
int myArray[SIZE];

//Semaphore to lock and unlock access for threads.
sem_t mySem;

//Function for threads to sum over 1/2 of the integer array. Then proceeds to store the threaded sums to 
//a global counter variable, with each thread having to wait to gain access to the global variable so 
//no thread modified data gets overriden by another thread.
void *threadCountingMethod(void *pass) {

	//Declaration of an integer pointer variable.
	int *switchAround = (int*)pass;
	//Local integer counter variable set to 0.
	int localCounter = 0;
	if (*switchAround == 0) { 
		for(int i = 0; i < (SIZE/2); i++) {
			localCounter += myArray[i];
		}
	}		
	else
		for(int i = (SIZE/2); i < SIZE; i++) {
			localCounter += myArray[i];
}				
	//Decrements Semaphore by 1 which means data is being modified and blocks and in coming access.
	sem_wait(&mySem);
	threadCounter += localCounter;

	//Increments Semaphore by 1 which means now data can be modified and unblocks in coming access.
	sem_post(&mySem);

	//To ensure thread process terminates when finished.
	pthread_exit(NULL);
}
//The start of the program.
int main() {

	//Local integer variable set to 0.
	int tracker = 0;

	//Number Of threads to create.
	int numThreads = 2;

	//To seed random number integer values.
	srand(time(0));

	//Local integer counter variable set to 0.
	int counter = 0;

	//Initialize Semaphore to 1 for blocking and unblocking access to critical section.
	sem_init(&mySem, 0, 1);
       	for(int i = 0; i < SIZE; i++) {

		//Local random variable to an integer value from 0 to 9.
		int randomNumber = rand() % 10;
		myArray[i] = randomNumber;
		counter += myArray[i];
	}
	//Prints the sum without threading, the real answer.
	printf("The sum without threading is %d.\n", counter);

	//Array of threads to store threads.
	pthread_t myThreads[numThreads];

	//Indentifier for each thread created.
	int holder[numThreads];

	//Threads are created and processed.
	for (int i = 0; i < numThreads; i++) {
		holder[i] = i;
		pthread_create(&myThreads[i], NULL, threadCountingMethod, &holder[i]);
	}

	//To join all finished thread processes.
	for(int i = 0; i < numThreads; i++) {
		pthread_join(myThreads[i], NULL);

	}
	//Prints the threaded sum.
	printf("The threaded sum is %d.\n", threadCounter);
	
	//Semaphore is no longer needed and destroyed.
	sem_destroy(&mySem);
	return 0;
 }
