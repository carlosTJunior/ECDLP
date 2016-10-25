CC = gcc
MPICC = mpicc

MAIN = main.c

OBJ = pollardrho.c \
      point.c \
	  ecc.c \
	  pollardrho_serial.c \
	  pollardrho_parallel_fork.c \
	  pollardrho_parallel_mpi.c \
	  hashtable.c 

OBJ_MPI = pollardrho.c \
    	  point.c \
		  ecc.c \
		  pollardrho_parallel_mpi_main.c \
		  hashtable.c 

TEST_DIR = tests

OBJ_TESTS = $(TEST_DIR)/testECC.c \
			$(TEST_DIR)/testPoint.c \
			$(TEST_DIR)/testPollard.c \
			$(TEST_DIR)/unittests.c \

LIBS = -W -g

TARGET = prog
TARGET_MPI = mpiprog
TARGET_TESTS = $(TEST_DIR)/tests

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(MAIN) $(LIBS)
	$(MPICC) -o $(TARGET_MPI) $(OBJ_MPI)


# Compile and Run Tests
ctests: $(OBJ_TESTS)
	$(CC) -o $(TARGET_TESTS) $(OBJ) $(OBJ_TESTS) $(LIBS)

rtests:
	./$(TARGET_TESTS)
