#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int findRed(int pixel) {
    int redPixel = (pixel>>16) & 0xFF; // Shift the pixel value to the right 16 bits, in order to get the 8 left-most bits of the pixel value
    pid_t thisProcess = getpid(); // Get the ID of this process
    printf("Red[%d]: Received coded value %d\nRed[%d]: Decoded into %d\n", thisProcess, pixel, thisProcess, redPixel);
    return redPixel;
}

int main(int argc, const char* argv[] ) {
    int pixel = atoi(argv[1]);
    int redPixel = findRed(pixel);

    exit(redPixel); // Use the red value of the pixel as the exit status code
}