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

// High-resolution timer
static inline double now()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + 1e-9 * t.tv_nsec;
}

int main(int argc, char **argv)
{
    // ====== CHANGE THESE PER EXPERIMENT ======
NX = 1000;
NY = 400;
NUM_Points = 14000000;
Maxiter = 10;
    // =========================================

    GRID_X = NX + 1;
    GRID_Y = NY + 1;
    dx = 1.0 / NX;
    dy = 1.0 / NY;

    omp_set_num_threads(4);

    // Allocate memory
    double *mesh_value = (double *)calloc(GRID_X * GRID_Y, sizeof(double));
    Points *points = (Points *)calloc(NUM_Points, sizeof(Points));

    // Initialize particles ONCE (Exp-3 requirement)
    initializepoints(points);

    printf("Iter\tInterp\t\tMover\t\tTotal\n");

    for (int iter = 0; iter < Maxiter; iter++)
    {
    	
    	
        // Reset mesh each iteration
        memset(mesh_value, 0, GRID_X * GRID_Y * sizeof(double));

        double t0 = now();
        interpolation(mesh_value, points);
        double t1 = now();

#ifdef USE_PARALLEL
        double t2 = now();
        mover_parallel(points, dx, dy);
        double t3 = now();
#else
        double t2 = now();
        mover_serial(points, dx, dy);
        double t3 = now();
#endif

        double interp_time = t1 - t0;
        double mover_time = t3 - t2;
        double total = interp_time + mover_time;

        printf("%d\t%lf\t%lf\t%lf\n", iter + 1, interp_time, mover_time, total);
    }

    free(mesh_value);
    free(points);
    return 0;
}
