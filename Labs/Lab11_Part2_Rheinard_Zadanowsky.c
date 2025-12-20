#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



//The beginning of the program.
int main () {

    //Creates a file descriptor for the shared memory area.
    int sharedMemoryDescriptor = shm_open("/myArea", O_RDWR, 0777);

    //Maps the shared memory.
    void *sharedMemoryMapping = mmap(NULL, 10000, PROT_READ, MAP_SHARED, sharedMemoryDescriptor, 0);

    //Creates a file descriptor for the new file 'data2'.
    int fileDescriptor = open("data2", O_RDWR|O_CREAT, 0777);

    //Sets the size of the file descriptor for the new file 'data2'.
    int truncate = ftruncate(fileDescriptor, 10000);

    //Maps the file memory of the new file.
    void *fileMemoryMapping = mmap(NULL, 10000, PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

    //Copies data from shared memory area to the new file memory.
    memcpy(fileMemoryMapping, sharedMemoryMapping, 10000);

    //Prints a succesful copy of data.
    printf("Data has been copied from the shared memory area to the new file.\n");

    //Unmaps the shared memory area.
    munmap(sharedMemoryMapping, 10000);

    //Unmaps the file memory of the new file.
    munmap(fileMemoryMapping, 10000);

    //Closes the file descriptor for the shared memory area.
    close(sharedMemoryDescriptor);

    //Closes the file descriptor for the new file 'data2'.
    close(fileDescriptor);
    return 0;
}
