CC = gcc

MAIN = main.c

OBJ = pollardrho.c point.c ecc.c pollardrho_serial.c \
	  hashtable.c 

TEST_DIR = tests

OBJ_TESTS = $(TEST_DIR)/testECC.c \
			$(TEST_DIR)/testPoint.c \
			$(TEST_DIR)/unittests.c \

LIBS = -W -g

TARGET = prog
TARGET_TESTS = $(TEST_DIR)/tests

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(MAIN) $(LIBS)

ctests: $(OBJ_TESTS)
	$(CC) -o $(TARGET_TESTS) $(OBJ) $(OBJ_TESTS) $(LIBS)

runtests:
	./$(TARGET_TESTS)
