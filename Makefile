CC = gcc

OBJ = pollardrho.c point.c main.c ecc.c

LIBS = -lgmp

TARGET = prog

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LIBS)