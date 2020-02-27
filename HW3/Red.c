#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>

#define BUFFER_SIZE 60000

int findRed(int pixel) {
    int redPixel = (pixel>>16) & 0xFF; // Shift the pixel value to the right 16 bits, in order to get the 8 left-most bits of the pixel value
    pid_t thisProcess = getpid(); // Get the ID of this process
    printf("Red[%d]: Received coded value %d\nRed[%d]: Decoded into %d\n", thisProcess, pixel, thisProcess, redPixel);
    return redPixel;
}

int main(int argc, const char* argv[] ) {
    int shared_mem_size = 60000;
    int redPixels[5776];
    const char* shared_mem_name = argv[5777];
    for (int i=0; i<5776; ++i) {       // convert the array of pixels as strings to an array of pixels as ints and find the red value for each pixel
        int pixel = atoi(argv[i+1]);
        redPixels[i] = findRed(pixel);
    }
   
    char red_str_array[BUFFER_SIZE];        
    for (int i=0; i<5776; ++i) {        // convert array of pixels as ints to a single string
        sprintf(red_str_array+strlen(red_str_array), "%d ", redPixels[i]);
    }

    int shm_fd_red = shm_open(shared_mem_name, O_RDWR, 0666);
    void *shmPtr_red = mmap(0, shared_mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_red, 0);
    
    sprintf(shmPtr_red, "%s", red_str_array);

    exit(1); // Use the red value of the pixel as the exit status code
}