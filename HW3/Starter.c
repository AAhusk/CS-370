#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>

#define READ_END	0
#define WRITE_END	1
#define BUFFER_SIZE 60000

// declarations of each function
char* getOutputFilename(const char* inputFile);
void Fork_RGB_Children(char** imageAsArray_arg, char* outputImage);
void createOutputFile(int RED_VALUE[], int GREEN_VALUE[], int BLUE_VALUE[], char* outputFilename);

int main(int argc, const char* argv[] )
{
    const char* filename;
    if (argc==1) { // This if statement is if no specified file is passed in as an argument; argc == 1 when only the program name is used to run the program
        filename = "coded_image_1.txt"; // Default file to use
    }
    else { // This else statement uses the name of the file that is passed in as an argument
        filename = argv[1]; 
    }
    
    char* OutputImage = getOutputFilename(filename); // This will get the name of the output file to be created, given the input file

    
    char img_str[BUFFER_SIZE];  // create the string that will contain every pixel value
    int status;
    int fd[2];
    if (pipe(fd) == -1) {       // create the pipe that will be used to communicate between FileReader and Starter
		fprintf(stderr,"Pipe failed");
		return 1;
	}
    char pipeLocation[200];
    sprintf(pipeLocation, "%d", fd[WRITE_END]); // save the location of the pipe so that it can be passed to FileReader
    pid_t child_pid;
    child_pid = fork();

    if (child_pid == 0) {                                                // Child process
        execlp("./FileReader", "./FileReader.c", filename, pipeLocation, (char *)NULL);
    }
    if (child_pid > 0) {                                                // Parent process
        wait(&status);                                                  // wait for FileReader to read the input file
        close(fd[WRITE_END]);                                              
        read(fd[READ_END], img_str, BUFFER_SIZE);                       // read the string, containing every pixel, that FileReader wrote to the pipe
        close(fd[READ_END]);
    }                                                                     

                                                   
    char** img_str_array;
    img_str_array = malloc(5779*sizeof(*img_str_array));   // allocate memory for an array of strings, where each string is a pixel
    for (int i=0; i<5779; ++i) {                          
        img_str_array[i] = malloc(10*sizeof(char));
    }

    int pixelIndex = 1;
    int lengthOfPrevious = 0;
    int position = 0;
    while (sscanf(img_str+position, "%s%n", img_str_array[pixelIndex], &lengthOfPrevious) == 1) {   // put each space separated pixel string from img_str into the string array
        position+=lengthOfPrevious;
        ++pixelIndex;
    }          

    Fork_RGB_Children(img_str_array, OutputImage);  // go into the function that will create shared memory and execute the subprocesses Red, Green and Blue

    free(img_str_array);    // free the memory allocated by the array of strings containing every pixel value

    return 0;
}

void Fork_RGB_Children(char** imageAsArray_arg, char* outputImage) {
    int shared_mem_size = 60000;
    int shm_fd_red = shm_open("RedMemory", O_CREAT | O_RDWR, 0666);     // create shared memory that will be used between Starter and the Red, Green, and Blue processes
    int shm_fd_green = shm_open("GreenMemory", O_CREAT | O_RDWR, 0666);
    int shm_fd_blue = shm_open("BlueMemory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd_red, shared_mem_size);     // allocate 60000 bytes of shared memory
    ftruncate(shm_fd_green, shared_mem_size);
    ftruncate(shm_fd_blue, shared_mem_size);
    void *shmPtr_red = mmap(0, shared_mem_size, PROT_READ, MAP_SHARED, shm_fd_red, 0);  // save the pointers to the shared memory in order to access it later
    void *shmPtr_green = mmap(0, shared_mem_size, PROT_READ, MAP_SHARED, shm_fd_green, 0);
    void *shmPtr_blue = mmap(0, shared_mem_size, PROT_READ, MAP_SHARED, shm_fd_blue, 0);
    if (shmPtr_blue == MAP_FAILED || shmPtr_green == MAP_FAILED || shmPtr_red == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}
    int red_status, green_status, blue_status;
                                                
    __pid_t red_child_pid = fork();
    if (red_child_pid == 0) {       // Call the Red program
        imageAsArray_arg[0] = "./RedMemory.c";
        imageAsArray_arg[5777] = "RedMemory";
        imageAsArray_arg[5778] = NULL;
        execvp("./Red", imageAsArray_arg);
    }
    
    __pid_t green_child_pid = fork();
    if (green_child_pid == 0) {     // Call the Green program
        imageAsArray_arg[0] = "./Green.c";
        imageAsArray_arg[5777] = "GreenMemory";
        imageAsArray_arg[5778] = NULL;
        execvp("./Green", imageAsArray_arg);
    }
    
    __pid_t blue_child_pid = fork();
    if (blue_child_pid == 0) {      // Call the Blue program
        imageAsArray_arg[0] = "./Blue.c";
        imageAsArray_arg[5777] = "BlueMemory";
        imageAsArray_arg[5778] = NULL;
        execvp("./Blue", imageAsArray_arg);
    }
    
    waitpid(red_child_pid, &red_status, 0); // wait for each of the subprocesses to finish
    waitpid(green_child_pid, &green_status, 0);
    waitpid(blue_child_pid, &blue_status, 0);


    int blue_int_array[5776]; // put each of the RGB values for every pixel into one of three arrays
    int red_int_array[5776];
    int green_int_array[5776];
    int pixelIndex = 0;
    int lengthOfPrevious = 0;
    int position = 0;
    while (sscanf(shmPtr_blue+position, "%d%n", &blue_int_array[pixelIndex], &lengthOfPrevious) == 1) { // put the Blue values into the blue pixel array from what Blue.c wrote to shared memory
        position+=lengthOfPrevious;
        ++pixelIndex;
    }
    pixelIndex = 0;
    lengthOfPrevious = 0;
    position = 0;
    while (sscanf(shmPtr_green+position, "%d%n", &green_int_array[pixelIndex], &lengthOfPrevious) == 1) { // put the Green values into the green pixel array from what Green.c wrote to shared memory
        position+=lengthOfPrevious;
        ++pixelIndex;
    }
    pixelIndex = 0;
    lengthOfPrevious = 0;
    position = 0;
    while (sscanf(shmPtr_red+position, "%d%n", &red_int_array[pixelIndex], &lengthOfPrevious) == 1) { // put the Red values into the red pixel array from what Red.c wrote to shared memory
        position+=lengthOfPrevious;
        ++pixelIndex;
    }   

    if (shm_unlink("RedMemory") == -1 || shm_unlink("GreenMemory") == -1 || shm_unlink("BlueMemory") == -1) { // unlink the shared memory used with the Red, Green, and Blue programs
		printf("Error removing %s\n","Memory");
		exit(-1);
	}
    
    createOutputFile(red_int_array, green_int_array, blue_int_array, outputImage); // using the arrays of the RGB values of every pixel, create the output image

    return;
}


void createOutputFile(int RED_VALUE[], int GREEN_VALUE[], int BLUE_VALUE[], char* outputFilename) {
    FILE *fout = fopen(outputFilename, "wb"); // Write the output image file
    int IMAGE_HEIGHT = 76;
    int IMAGE_WIDTH = 76;
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
