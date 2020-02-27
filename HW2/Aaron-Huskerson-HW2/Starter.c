#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int* FileToArray(FILE* fileToConvert);
void getRGBValues(int* arrayOfPixels, const char* outputFileName);
char* getOutputFilename(const char* inputFile);

int main(int argc, const char* argv[] )
{
    FILE* CodedImage; 
    const char* filename;
    if (argc==1) { // This if statement is if no specified file is passed in as an argument; argc == 1 when only the program name is used to run the program
        filename = "coded_image_1.txt"; // Default file to use
        CodedImage = fopen(filename, "r");
        if( CodedImage == NULL) {
            printf( "Could not open the file coded_image_1.txt"); 
            return 1;
        }
    }
    else { // This else statement uses the name of the file that is passed in as an argument
        filename = argv[1]; 
        CodedImage = fopen(filename, "r");
    }
    
    char* OutputImage = getOutputFilename(filename); // This will get the name of the output file to be created, given the input file

    int* imageAsArray =  FileToArray(CodedImage); // This function takes in the file(in .txt format) and returns an array of ints representing each pixel. I will refer to the int* as an array of ints
    fclose(CodedImage);

    getRGBValues(imageAsArray, OutputImage); // This function takes in the array of ints representing pixels and finds the Red, Green, and Blue value for each pixel. Then it writes the output file as an image

    free(imageAsArray); // This frees the memory that was allocated for the array of ints representing pixels
    return 0;
}

int* FileToArray(FILE* fileToConvert) {
    int* convertedFile;
    convertedFile = (int*) malloc(5776 * sizeof(int)); // Allocate memory on the heap for the array of ints, so that it still exists after returning the array from this function
    for (int i=0; i<5776; ++i) {
        fscanf(fileToConvert, "%d", &convertedFile[i]);
    }
        return convertedFile;
}


void getRGBValues(int* arrayOfPixels, const char* outputFileName) {
    __pid_t child_pid;
    int IMAGE_HEIGHT = 76; // Dimensions of the ouput image file
    int IMAGE_WIDTH = 76;
    int RedStatus, GreenStatus, BlueStatus;
    int redPixel, greenPixel, bluePixel;
    int RED_VALUE[IMAGE_HEIGHT*IMAGE_WIDTH], GREEN_VALUE[IMAGE_HEIGHT*IMAGE_WIDTH], BLUE_VALUE[IMAGE_HEIGHT*IMAGE_WIDTH]; // 3 arrays to hold one of each RGB value for every pixel
    int *redValuePtr = RED_VALUE; int *greenValuePtr = GREEN_VALUE; int *blueValuePtr = BLUE_VALUE; // This pointer will be used to assign each value in the array
    char pixelAsString[8];
    
    for (int i=0; i<5776; ++i) { // Loop through the 5776 pixels in the array
        int currentPixel = arrayOfPixels[i];
        sprintf(pixelAsString, "%d", currentPixel);
        child_pid = fork();
        if (child_pid == 0) { // Call the Red program
            execlp("./Red", "./Red.c", pixelAsString, (char *)NULL);
        }
        else {
            printf("Starter: Forked process with ID %d\n", child_pid);
            printf("Starter: Waiting for process [%d]\n", child_pid);
            wait(&RedStatus);   // Wait for child to finish, then insert the exit value of the Red program into the Red pixel array
            redPixel = WEXITSTATUS(RedStatus);
            *redValuePtr++ = redPixel;
            printf("Starter: Child process %d returned %d\n", child_pid, redPixel);

        }
        child_pid = fork();
        if (child_pid == 0) { // Call the Green program
            execlp("./Green", "./Green.c", pixelAsString, (char *)NULL);
        }
        else {
            printf("Starter: Forked process with ID %d\n", child_pid);
            printf("Starter: Waiting for process [%d]\n", child_pid);
            wait(&GreenStatus); // Wait for child to finish, then insert the exit value of the Green program into the Green pixel array
            greenPixel = WEXITSTATUS(GreenStatus);
            *greenValuePtr++ = greenPixel;
            printf("Starter: Child process %d returned %d\n", child_pid, greenPixel);
        }
        child_pid = fork();
        if (child_pid == 0) { // Call the Blue program
            execlp("./Blue", "./Blue.c", pixelAsString, (char *)NULL);
        }
        else {
            printf("Starter: Forked process with ID %d\n", child_pid);
            printf("Starter: Waiting for process [%d]\n", child_pid);
            wait(&BlueStatus);  // Wait for child to finish, then insert the exit value of the Blue program into the Blue pixel array
            bluePixel = WEXITSTATUS(BlueStatus);
            *blueValuePtr++ = bluePixel;
            printf("Starter: Child process %d returned %d\n", child_pid, bluePixel);
        }
    }
    
    FILE *fout = fopen(outputFileName, "wb"); // Write the output image file
    fprintf(fout, "P6\n%i %i 255\n", IMAGE_HEIGHT, IMAGE_WIDTH);
    for (int i = 0; i < IMAGE_HEIGHT*IMAGE_WIDTH; i++)
    {
        fputc(RED_VALUE[i], fout); // 0 .. 255
        fputc(GREEN_VALUE[i], fout); // 0 .. 255
        fputc(BLUE_VALUE[i], fout); // 0 .. 255
    }
    fclose(fout);
    return;
}

char* getOutputFilename(const char* filename) { //...get the output filename
        static char outputFileName[30]= "";
        char extension[12] = "_output.ppm";
        strcpy(outputFileName, filename);
        char* pos = strchr(outputFileName, '.');
        *pos = '\0';
        strcat(outputFileName, extension);
        return outputFileName;
    }
