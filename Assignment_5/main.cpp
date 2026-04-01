#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include "init.h"
#include "utils.h"

// Global variables
int GRID_X, GRID_Y, NX, NY;
long long NUM_Points;
int Maxiter;
double dx, dy;

// Timer
static inline double now()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + 1e-9 * t.tv_nsec;
}

int main(int argc, char **argv)
{
    if (argc < 7)
    {
        printf("Usage: ./sim NX NY NUM_POINTS MAXITER THREADS MODE\n");
        return 1;
    }

    NX = atoi(argv[1]);
    NY = atoi(argv[2]);
    NUM_Points = atoll(argv[3]);
    Maxiter = atoi(argv[4]);
    int nthreads = atoi(argv[5]);
    int mode = atoi(argv[6]);

    GRID_X = NX + 1;
    GRID_Y = NY + 1;
    dx = 1.0 / NX;
    dy = 1.0 / NY;

    omp_set_num_threads(nthreads);

    double *mesh_value = (double *)calloc(GRID_X * GRID_Y, sizeof(double));
    Points *points = (Points *)calloc(NUM_Points, sizeof(Points));

    initializepoints(points);

    char filename[256];
    sprintf(filename, "results_NX%d_NY%d_P%lld_T%d_M%d.csv",
            NX, NY, NUM_Points, nthreads, mode);

    FILE *fp = fopen(filename, "w");
    fprintf(fp, "iter,interp,mover,total,deleted\n");

    printf("Iter\tInterp\t\tMover\t\tTotal\t\tDeleted\n");

    for (int iter = 0; iter < Maxiter; iter++)
    {
        memset(mesh_value, 0, GRID_X * GRID_Y * sizeof(double));

        double t0 = now();
        interpolation(mesh_value, points);
        double t1 = now();

        double t2 = now();

        long long deleted = 0;

        // ===== MODE SWITCH =====
        if (mode == 0)
            deleted = mover_serial(points, dx, dy);
        else if (mode == 1)
            deleted = mover_serial_deferred(points, dx, dy);
        else if (mode == 2)
            deleted = mover_parallel(points, dx, dy);
        else if (mode == 3)
            deleted = mover_parallel_deferred(points, dx, dy);

        double t3 = now();

        double interp_time = t1 - t0;
        double mover_time = t3 - t2;
        double total = interp_time + mover_time;

        printf("%d\t%lf\t%lf\t%lf\t%lld\n",
               iter + 1, interp_time, mover_time, total, deleted);

        fprintf(fp, "%d,%lf,%lf,%lf,%lld\n",
                iter + 1, interp_time, mover_time, total, deleted);
    }

    fclose(fp);
    free(mesh_value);
    free(points);

    return 0;
}
