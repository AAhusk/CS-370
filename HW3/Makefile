GCC = gcc
EXE = Starter
EXE1 = Red
EXE2 = Green
EXE3 = Blue
EXE4 = FileReader
GCC_FLAGS = -std=gnu99 -g -Wall -c -lrt
LD_FLAGS = -std=gnu99 -g -Wall -o
LIB_OPT = -lrt


build: Starter Red Green Blue FileReader

Starter: Starter.c
	$(GCC) Starter.c $(LD_FLAGS) $(EXE) $(LIB_OPT)

Red: Red.c
	$(GCC) Red.c $(LD_FLAGS) $(EXE1) $(LIB_OPT)

Green: Green.c
	$(GCC) Green.c $(LD_FLAGS) $(EXE2) $(LIB_OPT)

Blue: Blue.c
	$(GCC) Blue.c $(LD_FLAGS) $(EXE3) $(LIB_OPT)

FileReader: FileReader.c
	$(GCC) FileReader.c $(LD_FLAGS) $(EXE4) $(LIB_OPT)
clean: 
	rm -f *.o *~ $(EXE) $(EXE1) $(EXE2) $(EXE3) $(EXE4) $(LIB_OPT)
package:
	tar -cvzf Aaron-Huskerson-HW3.tar *.c *.txt Makefile