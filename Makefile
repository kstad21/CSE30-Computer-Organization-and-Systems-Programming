# if you type 'make' without arguments, this is the default
PROG = cipher
all: $(PROG)

# associated object files 
HEAD    = main.h cipher.h rdbuf.h setup.h Version.h
OBJ     = setup.o main.o dcrypt.o ecrypt.o rdbuf.o Cmain.o Cdcrypt.o Cecrypt.o Crdbuf.o

# special libraries
LIB	= libpa7.a
LIBFLAGS =-L ./ $(LIB)

# select the compiler and flags you can over-ride on command line e.g. make DEBUG= 
CC      = gcc
DEBUG	= -ggdb
CSTD	=
WARN	= -Wall -Wextra
CDEFS	=
CFLAGS	= -I. $(DEBUG) $(WARN) $(CSTD) $(CDEFS)
AR      = ar
ARFLAGS = -rvs 

# some flags to use with assembler tools
ASLIST  = -Wa,-adhln
ASFLAGS =

$(OBJ):             $(HEAD)

# specify how to assemble a .S file
%.o : %.S
	$(CC) $(CFLAGS) $(ASFLAGS) -c -o $@ $<

# specify how to compile the target
$(PROG):    $(OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFLAGS) -o $@

.PHONY: clean test1 test2

clean:
	rm -f $(OBJ) $(PROG)

test1:      $(PROG)
	-./runtest 1

test2:      $(PROG)
	-./runtest 2
