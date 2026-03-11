#ifndef INIT_H
#define INIT_H

#include <stddef.h>

typedef struct {
    double x;
    double y;
} Points;

/* Global simulation parameters (define in main) */
extern int GRID_X, GRID_Y, NX, NY;
extern long long NUM_Points;
extern int Maxiter;
extern double dx, dy;

void initializepoints(Points *points);

#endif
