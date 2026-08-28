/*
 * ============================================================================
 * Project:     Computer Architecture & Performance Lab
 * File:        kmeans.c
 * Author:      Gustavo Felipe Paluch Figueiredo
 * Description: Parallel K-Means clustering algorithm using OpenMP with
 *              multi-threaded distance evaluation and array reduction.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DIM 3

int main(int argc, char *argv[]) {
    int threads = (argc > 1) ? atoi(argv[1]) : 1;
    omp_set_num_threads(threads);

    int i, j, c, k, n;
    double dmin, dx;
    double *x, *mean, *sum;
    int *cluster, *count, color;
    int flips;

    if (scanf("%d %d", &k, &n) != 2) {
        fprintf(stderr, "Error: Invalid input parameters for k and n.\n");
        return 1;
    }

    x = (double *)malloc(sizeof(double) * DIM * n);
    mean = (double *)malloc(sizeof(double) * DIM * k);
    sum = (double *)malloc(sizeof(double) * DIM * k);
    cluster = (int *)malloc(sizeof(int) * n);
    count = (int *)malloc(sizeof(int) * k);

    if (!x || !mean || !sum || !cluster || !count) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }

    for (i = 0; i < n; i++) cluster[i] = 0;

    for (i = 0; i < k; i++) {
        if (scanf("%lf %lf %lf", mean + i * DIM, mean + i * DIM + 1, mean + i * DIM + 2) != 3) {
            fprintf(stderr, "Error reading initial cluster means.\n");
            return 1;
        }
    }

    for (i = 0; i < n; i++) {
        if (scanf("%lf %lf %lf", x + i * DIM, x + i * DIM + 1, x + i * DIM + 2) != 3) {
            fprintf(stderr, "Error reading data points.\n");
            return 1;
        }
    }

    double start = omp_get_wtime();
    flips = n;
    int iteracoes = 0;
    const int MAX_ITER = 100;

    while (flips > 0 && iteracoes < MAX_ITER) {
        iteracoes++;
        flips = 0;

        #pragma omp parallel for private(i, c, color, dmin, dx) reduction(+:flips)
        for (i = 0; i < n; i++) {
            dmin = -1.0;
            color = cluster[i];

            for (c = 0; c < k; c++) {
                dx = 0.0;
                for (j = 0; j < DIM; j++) {
                    double diff = x[i * DIM + j] - mean[c * DIM + j];
                    dx += diff * diff;
                }
                if (dx < dmin || dmin == -1.0) {
                    color = c;
                    dmin = dx;
                }
            }

            if (cluster[i] != color) {
                flips++;
                cluster[i] = color;
            }
        }

        for (j = 0; j < k; j++) {
            count[j] = 0;
            for (i = 0; i < DIM; i++) sum[j * DIM + i] = 0.0;
        }

        #pragma omp parallel for private(i, j) reduction(+:count[0:k], sum[0:k * DIM])
        for (i = 0; i < n; i++) {
            count[cluster[i]]++;
            for (j = 0; j < DIM; j++) {
                sum[cluster[i] * DIM + j] += x[i * DIM + j];
            }
        }

        for (i = 0; i < k; i++) {
            if (count[i] > 0) {
                for (j = 0; j < DIM; j++) {
                    mean[i * DIM + j] = sum[i * DIM + j] / count[i];
                }
            }
        }
    }

    double end = omp_get_wtime();

    printf("Elapsed time (s) = %.4lf\n", end - start);
    printf("k = %d, n = %d, iterations = %d, threads = %d\n", k, n, iteracoes, threads);

    free(x);
    free(mean);
    free(sum);
    free(cluster);
    free(count);

    return 0;
}
