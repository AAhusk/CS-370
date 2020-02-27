GCC = gcc
EXE = Starter
EXE1 = Red
EXE2 = Green
EXE3 = Blue
GCC_FLAGS = -std=c11 -g -Wall -c -l

build: Starter Red Green Blue

Starter: Starter.c
	$(GCC) -g -o $(EXE) Starter.c

Red: Red.c
	$(GCC) -g -o $(EXE1) Red.c

Green: Green.c
	$(GCC) -g -o $(EXE2) Green.c

Blue: Blue.c
	$(GCC) -g -o $(EXE3) Blue.c

clean: 
	rm -f *.o *~ $(EXE) $(EXE1) $(EXE2) $(EXE3)
package:
	tar -cvzf Aaron-Huskerson-HW2.tar *.c *.txt Makefile