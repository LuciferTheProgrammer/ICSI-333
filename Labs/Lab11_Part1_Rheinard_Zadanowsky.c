#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


//The beginning of the program.
int main () {

    //Creates the file descriptor for the file 'data'.
    int fileDescriptor = open("data", O_RDONLY);

    //Maps the file memory.
    void *fileMemoryMapping = mmap(NULL, 10000, PROT_READ, MAP_PRIVATE, fileDescriptor, 0);

    //Creates a file descriptor for the shared memory area.
    int sharedMemoryDescriptor = shm_open("/myArea", O_RDWR | O_CREAT, 0777);

    //Sets the size for the file descriptor of the shared memory area.
    int truncate = ftruncate(sharedMemoryDescriptor, 10000);

    //Maps the shared memory.
    void *sharedMemoryMapping = mmap(NULL, 10000, PROT_WRITE, MAP_SHARED, sharedMemoryDescriptor, 0);

    //Copies data from the file memory to the shared memory area.
    memcpy(sharedMemoryMapping, fileMemoryMapping, 10000);

    //Prints a successful copy of data.
    printf("Data has been copied from the file memory to the shared memory area.\n");

    //Unmaps the shared memory area.
    munmap(sharedMemoryMapping, 10000);

    //Unmaps the file memory.
    munmap(fileMemoryMapping, 10000);

    //Closes file descriptor for the shared memory area.    
    close(sharedMemoryDescriptor);

    //Closes file descriptor for the file 'data'.
    close(fileDescriptor);
    return 0;
}
