#ifndef INIT_H
#define INIT_H

#include <stddef.h>

typedef struct {
    double x;
    double y;
} Points;

/* Global simulation parameters */
extern int GRID_X, GRID_Y, NX, NY;
extern long long NUM_Points;
extern int Maxiter;
extern double dx, dy;

/* Special marker for invalid particle */
#define INVALID_POS -1.0

void initializepoints(Points *points);
void reinitialize_point(Points *p);

#endif
