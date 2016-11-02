CC = g++
MPICC = mpicc

MAIN = main.c

SRC = point.cpp \
	  ecc.cpp \
      pollardrho.cpp \
	  #pollardrho_serial.cpp \
	  #pollardrho_parallel_fork.c \
	  #pollardrho_parallel_mpi.c \
	  #mersenne_twister.c \
	  #watches.c \
	  #hashtable.c 

SRC_MPI = pollardrho.c \
    	  point.c \
		  ecc.c \
		  pollardrho_parallel_mpi_main.c \
	      mersenne_twister.c \
	      watches.c \
		  hashtable.c 

TEST_DIR = tests

SRC_TESTS = $(TEST_DIR)/testECC.cpp \
			$(TEST_DIR)/testPoint.cpp \
			$(TEST_DIR)/unittests.cpp \
			$(TEST_DIR)/testPollard.cpp \

CFLAGS = -W -g #-std=gnu99
LIBS = -lm
GMPLIB = -lgmp -lgmpxx

TARGET = prog
TARGET_MPI = mpiprog
TARGET_TESTS = $(TEST_DIR)/tests

all: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(MAIN) $(CFLAGS) $(LIBS)
	#$(MPICC) -o $(TARGET_MPI) $(SRC_MPI)


# Compile and Run Tests
ctests: $(SRC_TESTS)
	$(CC) -o $(TARGET_TESTS) $(SRC) $(SRC_TESTS) $(CFLAGS) $(LIBS) $(GMPLIB)

rtests:
	./$(TARGET_TESTS)
