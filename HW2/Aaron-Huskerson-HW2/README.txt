DESCRIPTION OF SOURCE FILES
Starter.c - This program takes in an optional argument of the filename of a coded image 76 pixels by 76 pixes, in which each pixel is represented as an int.
    If no filename is provided, a default image is provided. This program then opens the file, and creates an array which contains each pixel represented as
    an integer. With the array, the program now closes the file. The program then enters a function which will determine the Red, Green, and Blue value of 
    each pixel. It does this in a loop, with each iteration being for each pixel; within this loop, 3 fork() calls are made, to start processes to determine
    each of the RGB values. Note that the next process does not start until the previous one ends. Once all the RGB values have been determined, and we exit
    the loop, we write each of these RGB values to an output file that will show the image itself.

Red.c - Finds the Red value of the pixel that is sent to this program as an argument. It does this by shifting the integer to the right 16 bits, in order to
    retrieve bits 17-24. It also prints the pixel value and the Red value contained within before exiting the program. The program exits by making
    an exit call with the Red value of the pixel passed as the exit code.

Green.c - Finds the Green value of the pixel that is sent to this program as an argument. It does this by shifting the integer to the right 8 bits, and calling
    "&" with 0xFF to retrieve bits 9-16. It also prints the pixel value and the Green value contained within before exiting the program. The program exits by
    making an exit call with the Green value of the pixel passed as the exit code.

Blue.c - Finds the Blue value of the pixel that is sent to this program as an argument. It does this by calling "&" with the pixel value and 0xFF to retrieve 
    bits 1-8. It also prints the pixel value and the Blue value contained within before exiting the program. The program exits by making an exit call with the
    Blue value of the pixel passed as the exit code.

QUESTIONS
1. WEXITSTATUS returns the 8 least significant bits of the status.
2. #include <sys/wait.h> has to be included to use WEXITSTATUS
3. The return value for fork() in a child process is 0.
4. If the fork() call would cause the system limit on the number of processes being executed to be exceeded, the fork() call would fail.
5. This program, Starter.c and the other source files it calls, is an instance of sequential processing: only one child process is executed at a time.