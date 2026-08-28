CC = gcc
MPICC = mpicc
CFLAGS = -O3 -Wall -Wextra
OMPFLAGS = -fopenmp -lm
MPIFLAGS = -lm

OPENMP_SRCS = openmp/kmeans.c openmp/nbody.c openmp/laplace_omp.c
MPI_SRCS = mpi/laplace_mpi.c

BINS = bin/kmeans bin/nbody bin/laplace_omp bin/laplace_mpi

all: dirs $(BINS)

dirs:
	mkdir -p bin

bin/kmeans: openmp/kmeans.c
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@

bin/nbody: openmp/nbody.c
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@

bin/laplace_omp: openmp/laplace_omp.c
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@

bin/laplace_mpi: mpi/laplace_mpi.c
	$(MPICC) $(CFLAGS) $< $(MPIFLAGS) -o $@

clean:
	rm -rf bin
