/*
 * ============================================================================
 * Project:     Computer Architecture & Performance Lab
 * File:        laplace_omp.c
 * Author:      Gustavo Felipe Paluch Figueiredo
 * Description: 2D Laplace equation solver (Jacobi iteration) using OpenMP
 *              shared-memory parallel loops and error reduction.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define ABS(A)    (((A) < 0.0) ? (-(A)) : (A))

int main(int argc, char *argv[]) {
    int threads = (argc > 1) ? atoi(argv[1]) : 1;
    omp_set_num_threads(threads);

    int N = 500;
    int max_iter = 10000;
    double tol = 1e-4;

    if (argc > 2) N = atoi(argv[2]);
    if (argc > 3) max_iter = atoi(argv[3]);

    double *grid = (double *)calloc(N * N, sizeof(double));
    double *new_grid = (double *)calloc(N * N, sizeof(double));

    for (int i = 0; i < N; i++) {
        grid[i] = 1.0;
        new_grid[i] = 1.0;
    }

    double start = omp_get_wtime();
    int iter = 0;
    double max_diff = 1.0;

    while (iter < max_iter && max_diff > tol) {
        max_diff = 0.0;

        #pragma omp parallel for reduction(max:max_diff) schedule(static)
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                new_grid[i * N + j] = 0.25 * (grid[(i - 1) * N + j] +
                                              grid[(i + 1) * N + j] +
                                              grid[i * N + j - 1] +
                                              grid[i * N + j + 1]);
                double diff = ABS(new_grid[i * N + j] - grid[i * N + j]);
                max_diff = MAX(max_diff, diff);
            }
        }

        #pragma omp parallel for schedule(static)
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                grid[i * N + j] = new_grid[i * N + j];
            }
        }

        iter++;
    }

    double end = omp_get_wtime();

    printf("Elapsed time (s) = %.4lf\n", end - start);
    printf("Grid = %dx%d, iterations = %d, max_diff = %e, threads = %d\n", N, N, iter, max_diff, threads);

    free(grid);
    free(new_grid);

    return 0;
}
