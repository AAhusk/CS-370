#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int findBlue(int pixel) {
    int bluePixel = (pixel) & 0xFF; // Use bitwise "&" with 0xFF to retrieve the 8 right-most bits of the pixel value
    pid_t thisProcess = getpid(); // Get the ID of this process
    printf("Blue[%d]: Received coded value %d\nBlue[%d]: Decoded into %d\n", thisProcess, pixel, thisProcess, bluePixel);
    return bluePixel;
}

int main(int argc, const char* argv[] ) {
    int pixel = atoi(argv[1]);
    int bluePixel = findBlue(pixel);

    exit(bluePixel);  // Use the blue value of the pixel as the exit status code
}