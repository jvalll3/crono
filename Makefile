CC = gcc
MPICC = mpicc
CXX = g++
CFLAGS  = -O0
LDFLAGS =
GFLAGS = -Og -g

TARGET  = program
OBJECTS = $(TARGET)

all: $(TARGET)

program: program.c crono.h crono.c
	$(MPICC) $(CFLAGS) $< -fopenmp -o $@ crono.c $(LDFLAGS)
	#$(MPICC) $(CFLAGS) $< -o $@ crono.c $(LDFLAGS)
	#$(CC) $(CFLAGS) $< -o $@ crono.c $(LDFLAGS)


clean: out_clean
	rm -rf $(OBJECTS)

out_clean:


submit: program
