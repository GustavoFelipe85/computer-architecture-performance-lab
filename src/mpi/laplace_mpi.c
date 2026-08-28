/*
 * ============================================================================
 * Project:     Computer Architecture & Performance Lab
 * File:        laplace_mpi.c
 * Author:      Gustavo Felipe Paluch Figueiredo
 * Description: 2D Laplace equation solver using MPI domain decomposition by
 *              rows, ghost-cell exchange with MPI_Sendrecv, and MPI_Allreduce.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define ABS(A)    (((A) < 0.0) ? (-(A)) : (A))

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 500;
    int max_iter = 10000;
    double tol = 1e-4;

    if (argc > 1) N = atoi(argv[1]);
    if (argc > 2) max_iter = atoi(argv[2]);

    int local_rows = N / size;
    int start_row = rank * local_rows;
    int total_local_rows = local_rows + 2;

    double *u = (double *)calloc(total_local_rows * N, sizeof(double));
    double *u_new = (double *)calloc(total_local_rows * N, sizeof(double));

    if (rank == 0) {
        for (int j = 0; j < N; j++) {
            u[1 * N + j] = 1.0;
            u_new[1 * N + j] = 1.0;
        }
    }

    int top_neighbor = (rank == 0) ? MPI_PROC_NULL : rank - 1;
    int bottom_neighbor = (rank == size - 1) ? MPI_PROC_NULL : rank + 1;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    int iter = 0;
    double global_diff = 1.0;

    while (iter < max_iter && global_diff > tol) {
        MPI_Sendrecv(&u[1 * N], N, MPI_DOUBLE, top_neighbor, 0,
                     &u[(local_rows + 1) * N], N, MPI_DOUBLE, bottom_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Sendrecv(&u[local_rows * N], N, MPI_DOUBLE, bottom_neighbor, 1,
                     &u[0 * N], N, MPI_DOUBLE, top_neighbor, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        double local_diff = 0.0;
        for (int i = 1; i <= local_rows; i++) {
            int global_i = start_row + (i - 1);
            if (global_i == 0 || global_i == N - 1) continue;

            for (int j = 1; j < N - 1; j++) {
                u_new[i * N + j] = 0.25 * (u[(i - 1) * N + j] +
                                           u[(i + 1) * N + j] +
                                           u[i * N + j - 1] +
                                           u[i * N + j + 1]);
                double diff = ABS(u_new[i * N + j] - u[i * N + j]);
                local_diff = MAX(local_diff, diff);
            }
        }

        for (int i = 1; i <= local_rows; i++) {
            for (int j = 1; j < N - 1; j++) {
                u[i * N + j] = u_new[i * N + j];
            }
        }

        MPI_Allreduce(&local_diff, &global_diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        iter++;
    }

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Elapsed time (s) = %.4lf\n", end - start);
        printf("Grid = %dx%d, iterations = %d, max_diff = %e, MPI processes = %d\n", N, N, iter, global_diff, size);
    }

    free(u);
    free(u_new);
    MPI_Finalize();

    return 0;
}
