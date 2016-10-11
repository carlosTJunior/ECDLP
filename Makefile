CC = gcc

OBJ = pollardrho.c point.c main.c ecc.c pollardrho_serial.c \
	  hashtable.c pollardrho_parallel_fork.c

LIBS = -lgmp

TARGET = prog

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LIBS)
