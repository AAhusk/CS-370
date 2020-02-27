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

int findBlue(int pixel) {
    int bluePixel = (pixel) & 0xFF; // Use bitwise "&" with 0xFF to retrieve the 8 right-most bits of the pixel value
    pid_t thisProcess = getpid(); // Get the ID of this process
    printf("Blue[%d]: Received coded value %d\nBlue[%d]: Decoded into %d\n", thisProcess, pixel, thisProcess, bluePixel);
    return bluePixel;
}

int main(int argc, const char* argv[] ) {
    int shared_mem_size = 60000;
    int bluePixels[5776];
    const char* shared_mem_name = argv[5777];
    for (int i=0; i<5776; ++i) {
        int pixel = atoi(argv[i+1]);
        bluePixels[i] = findBlue(pixel);
    }

    char blue_str_array[BUFFER_SIZE];
    for (int i=0; i<5776; ++i) {   
        sprintf(blue_str_array+strlen(blue_str_array), "%d ", bluePixels[i]);
    }

    int shm_fd_blue = shm_open(shared_mem_name, O_RDWR, 0666);
    void *shmPtr_blue = mmap(0, shared_mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_blue, 0);

    sprintf(shmPtr_blue, "%s", blue_str_array);

    exit(1);  // Use the blue value of the pixel as the exit status code
}