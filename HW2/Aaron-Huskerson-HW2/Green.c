#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int findGreen(int pixel) { 
    int greenPixel = (pixel>>8) & 0xFF; // Shift the pixel value to the right 8 bits, and  use bitwise "&" with 0xFF to retreive the middle 8 bits of the pixel value 
    pid_t thisProcess = getpid(); // Get the ID of this process
    printf("Green[%d]: Received coded value %d\nGreen[%d]: Decoded into %d\n", thisProcess, pixel, thisProcess, greenPixel);
    return greenPixel;
}

int main(int argc, const char* argv[] ) {
    int pixel = atoi(argv[1]);
    int greenPixel = findGreen(pixel);

    exit(greenPixel); // Use the green value of the pixel as the exit status code
}