#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>

#define BUFFER_SIZE 60000

int findGreen(int pixel) { 
    int greenPixel = (pixel>>8) & 0xFF; // Shift the pixel value to the right 8 bits, and  use bitwise "&" with 0xFF to retreive the middle 8 bits of the pixel value 
    pid_t thisProcess = getpid(); // Get the ID of this process
    printf("Green[%d]: Received coded value %d\nGreen[%d]: Decoded into %d\n", thisProcess, pixel, thisProcess, greenPixel);
    return greenPixel;
}

int main(int argc, const char* argv[] ) {
    int shared_mem_size = 60000;
    int greenPixels[5776];
    const char* shared_mem_name = argv[5777];
    for (int i=0; i<5776; ++i) {
        int pixel = atoi(argv[i+1]);
        greenPixels[i] = findGreen(pixel);
    }
    
    char green_str_array[BUFFER_SIZE];
    for (int i=0; i<5776; ++i) {   
        sprintf(green_str_array+strlen(green_str_array), "%d ", greenPixels[i]);
    }


    int shm_fd_green = shm_open(shared_mem_name, O_RDWR, 0666);
    void *shmPtr_green = mmap(0, shared_mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_green, 0);

    sprintf(shmPtr_green, "%s", green_str_array);


    exit(1); // Use the green value of the pixel as the exit status code
}