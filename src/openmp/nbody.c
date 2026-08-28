/*
 * ============================================================================
 * Project:     Computer Architecture & Performance Lab
 * File:        nbody.c
 * Author:      Gustavo Felipe Paluch Figueiredo
 * Description: Parallel N-Body gravitational simulation using OpenMP with
 *              stepwise position and velocity integration.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define G 6.674e-11
#define DT 0.01

int main(int argc, char *argv[]) {
    int threads = (argc > 1) ? atoi(argv[1]) : 1;
    omp_set_num_threads(threads);

    int N, steps;
    if (scanf("%d %d", &N, &steps) != 2) {
        fprintf(stderr, "Usage input: <N> <steps>\n");
        return 1;
    }

    double *x = (double *)malloc(N * sizeof(double));
    double *y = (double *)malloc(N * sizeof(double));
    double *z = (double *)malloc(N * sizeof(double));
    double *vx = (double *)calloc(N, sizeof(double));
    double *vy = (double *)calloc(N, sizeof(double));
    double *vz = (double *)calloc(N, sizeof(double));
    double *mass = (double *)malloc(N * sizeof(double));

    if (!x || !y || !z || !vx || !vy || !vz || !mass) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }

    srand(42);
    for (int i = 0; i < N; i++) {
        x[i] = ((double)rand() / RAND_MAX) * 100.0;
        y[i] = ((double)rand() / RAND_MAX) * 100.0;
        z[i] = ((double)rand() / RAND_MAX) * 100.0;
        mass[i] = 1.0 + ((double)rand() / RAND_MAX) * 10.0;
    }

    double start = omp_get_wtime();

    for (int t = 0; t < steps; t++) {
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++) {
            double ax = 0.0, ay = 0.0, az = 0.0;
            for (int j = 0; j < N; j++) {
                if (i == j) continue;
                double dx = x[j] - x[i];
                double dy = y[j] - y[i];
                double dz = z[j] - z[i];
                double dist2 = dx * dx + dy * dy + dz * dz + 1e-10;
                double dist = sqrt(dist2);
                double f = (G * mass[j]) / dist2;

                ax += f * (dx / dist);
                ay += f * (dy / dist);
                az += f * (dz / dist);
            }
            vx[i] += ax * DT;
            vy[i] += ay * DT;
            vz[i] += az * DT;
        }

        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; i++) {
            x[i] += vx[i] * DT;
            y[i] += vy[i] * DT;
            z[i] += vz[i] * DT;
        }
    }

    double end = omp_get_wtime();

    printf("Elapsed time (s) = %.4lf\n", end - start);
    printf("N = %d, steps = %d, threads = %d\n", N, steps, threads);

    free(x); free(y); free(z);
    free(vx); free(vy); free(vz);
    free(mass);

    return 0;
}
