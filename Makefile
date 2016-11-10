CC = g++
MPICC = mpic++

MAIN = main.cpp

SRC = point.cpp \
	  ecc.cpp \
      pollardrho.cpp \
	  pollardrho_serial.cpp \
	  pollardrho_parallel_fork.cpp \
	  hashtable.cpp \
	  pollardrho_parallel_mpi.cpp \
	  watches.c 

SRC_MPI = pollardrho.cpp \
    	  point.cpp \
		  ecc.cpp \
		  pollardrho_parallel_mpi_main.cpp \
		  hashtable.cpp \
	      watches.c 

TEST_DIR = tests

SRC_TESTS = $(TEST_DIR)/testECC.cpp \
			$(TEST_DIR)/testPoint.cpp \
			$(TEST_DIR)/unittests.cpp \
			$(TEST_DIR)/testPollard.cpp

CFLAGS = -W -Wall -pedantic -Wextra -g #-std=gnu99
LIBS = -lm
GMPLIB = -lgmp -lgmpxx

TARGET = prog
TARGET_MPI = mpiprog
TARGET_TESTS = $(TEST_DIR)/tests

all: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(MAIN) $(CFLAGS) $(LIBS) -I. -DTTMATH_DONT_USE_WCHAR
	$(MPICC) -o $(TARGET_MPI) $(SRC_MPI) $(CFLAGS) $(LIBS) -I. -DTTMATH_DONT_USE_WCHAR

gmp: $(SRC)
	$(CC) -D_LIB_GMP -o $(TARGET) $(SRC) $(MAIN) $(CFLAGS) $(LIBS) $(GMPLIB)
	$(MPICC) -D_LIB_GMP -o $(TARGET_MPI) $(SRC_MPI) $(CFLAGS) $(LIBS) $(GMPLIB)


# Compile and Run Tests
ctests: $(SRC_TESTS)
	$(CC) -o $(TARGET_TESTS) $(SRC) $(SRC_TESTS) $(CFLAGS) $(LIBS) $(GMPLIB)

rtests:
	./$(TARGET_TESTS)

clean: $(TARGET) $(TARGET_MPI)
	rm $(TARGET) $(TARGET_MPI)
	rm pfifo
