#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "init.h"
#include "utils.h"

#define CLK CLOCK_MONOTONIC

int main() {

    struct timespec start_e2e, end_e2e, start_alg, end_alg;

    int minProbSize = 1 << 1;   // 2
    int maxProbSize = 1 << 12;  // 4096

    double **m1, **m2, **result;

    printf("ProblemSize, E2ETime, AlgoTime\n");

    for (int Np = minProbSize; Np <= maxProbSize; Np *= 2) {

        clock_gettime(CLK, &start_e2e);

        init_matrices(Np, &m1, &m2, &result);

        clock_gettime(CLK, &start_alg);

        // ---- K-I-J LOOP ORDER ----
        matrix_multiplication_kij(m1, m2, result, Np);

        clock_gettime(CLK, &end_alg);
        clock_gettime(CLK, &end_e2e);

        double e2e_time =
            (end_e2e.tv_sec - start_e2e.tv_sec) +
            (end_e2e.tv_nsec - start_e2e.tv_nsec) * 1e-9;

        double alg_time =
            (end_alg.tv_sec - start_alg.tv_sec) +
            (end_alg.tv_nsec - start_alg.tv_nsec) * 1e-9;

        printf("%d, %.9lf, %.9lf\n", Np, e2e_time, alg_time);
        fflush(stdout);

        free_matrices(Np, m1, m2, result);
    }

    return 0;
}

