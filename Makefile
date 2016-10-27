CC = gcc
MPICC = mpicc

MAIN = main.c

SRC = pollardrho.c \
      point.c \
	  ecc.c \
	  pollardrho_serial.c \
	  pollardrho_parallel_fork.c \
	  pollardrho_parallel_mpi.c \
	  mersenne_twister.c \
	  watches.c \
	  hashtable.c 

SRC_MPI = pollardrho.c \
    	  point.c \
		  ecc.c \
		  pollardrho_parallel_mpi_main.c \
	      mersenne_twister.c \
	      watches.c \
		  hashtable.c 

TEST_DIR = tests

SRC_TESTS = $(TEST_DIR)/testECC.c \
			$(TEST_DIR)/testPoint.c \
			$(TEST_DIR)/testPollard.c \
			$(TEST_DIR)/unittests.c \

CFLAGS = -W -g -std=gnu99
LIBS = -lm

TARGET = prog
TARGET_MPI = mpiprog
TARGET_TESTS = $(TEST_DIR)/tests

all: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(MAIN) $(CFLAGS) $(LIBS)
	$(MPICC) -o $(TARGET_MPI) $(SRC_MPI)


# Compile and Run Tests
ctests: $(SRC_TESTS)
	$(CC) -o $(TARGET_TESTS) $(SRC) $(SRC_TESTS) $(CFLAGS) $(LIBS)

rtests:
	./$(TARGET_TESTS)
