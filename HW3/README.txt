Program Descriptions:

Starter.c - Begins by creating a pipe, and starting the child process, FileReader. After FileReader returns, Starter reads from the pipe the values of the pixels,
        all entered in a single string. The single string is then separated into a string array, which is passed to each of the three child processes Red, Blue, and
        Green. After these processes return, Starter reads 3 int arrays from shared memory, each representing one of the RGB values of every pixel. The Starter then
        creates the output image using these int arrays.

FileReader.c - Receives the name of the input file, parses it, and puts every pixel into one long string. It then writes this string to a pipe, which will be read by Starter.

Red.c - Finds the Red value of the pixel that is sent to this program as an argument. It does this by shifting the integer to the right 16 bits, in order to
    retrieve bits 17-24. It also prints the pixel value and the Red value contained within before exiting the program. The program puts each of these values into
    an array which it then writes into shared memory with Starter.

Green.c - Finds the Green value of the pixel that is sent to this program as an argument. It does this by shifting the integer to the right 8 bits, and calling
    "&" with 0xFF to retrieve bits 9-16. It also prints the pixel value and the Green value contained within before exiting the program. The program puts each of these values 
    into an array which it then writes into shared memory with Starter.

Blue.c - Finds the Blue value of the pixel that is sent to this program as an argument. It does this by calling "&" with the pixel value and 0xFF to retrieve 
    bits 1-8. It also prints the pixel value and the Blue value contained within before exiting the program. The program puts each of these values into
    an array which it then writes into shared memory with Starter.


Questions:
1) Pipe(int fd[]) is used to create a pipe. fd[0] accesses the read end, and fd[1] accesses the write end.
2) mmap() maps files and devices into memory.
3) shm_open() opens a shared memory object. It returns an int that is a file descriptor for that shared memory object. It returns -1 if there was an error opening the
    shared memory.