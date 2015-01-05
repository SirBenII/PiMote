OBJ = main.o calibration.o file.o frame.o gamepad.o gpio.o uart.o
DEPS = main.h calibration.h file.h frame.h gamepad.h gpio.h uart.h types.h
LIBS = -lncurses
CFLAGS = -std=c99 -Wall
CC = gcc
EXTENSION = .c

#define a rule that applies to all files ending in the .o suffix, which says that the .o file depends upon the .c version of the file and all the .h files included in the DEPS macro.  Compile each object file
%.o: %$(EXTENSION) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#Combine them into the output file
#Set your desired exe output file name here
all: PiMote clean

PiMote: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Cleanup
.PHONY: clean

clean:
	rm -f *.o *~ core *~ 