#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END	0
#define WRITE_END	1

int main(int argc, const char* argv[] ) {
    const char* pipe_write_loc_arg = argv[2];   // get the location of the pipe
    int pipe_write_location = atoi(pipe_write_loc_arg);
    const char* filename = argv[1]; // get the filename of the input file containing the values of all the pixels
    FILE* codedImage = fopen(filename, "r");    // open the file by that filename

    fseek(codedImage, 0, SEEK_END);
    long fsize = ftell(codedImage);
    fseek(codedImage, 0, SEEK_SET);  

    char *imageAsString = malloc(fsize + 1);    // allocate memory for the string that will contain every pixel
    fread(imageAsString, 1, fsize, codedImage); // put the contents of the file into the string of pixels
    fclose(codedImage);

    imageAsString[fsize] = 0;   // put 0 as the last element of the string, to mark the end of the string

    write(pipe_write_location, imageAsString, fsize+1); // write this string to the pipe that will be read by Starter

    close(pipe_write_location);

    free(imageAsString);    // free the memory previously allocated for the string of pixels

    exit(1);
}