#ifndef UTILS_H
#define UTILS_H
#include <time.h>
#include "init.h"

void interpolation(double *mesh_value, Points *points);


long long mover_serial(Points *points, double deltaX, double deltaY);
long long mover_serial_deferred(Points *points, double deltaX, double deltaY);
long long mover_parallel(Points *points, double deltaX, double deltaY);
long long mover_parallel_deferred(Points *points, double deltaX, double deltaY);

void save_mesh(double *mesh_value);

#endif
