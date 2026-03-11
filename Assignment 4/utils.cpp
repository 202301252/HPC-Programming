#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "utils.h"
#include "init.h"

// ================= INTERPOLATION =================
// Cloud-In-Cell bilinear interpolation (serial)
// Cache-friendly particle loop, minimal branching
void interpolation(double *mesh_value, Points *points)
{
    for (long long p = 0; p < NUM_Points; p++)
    {
        double x = points[p].x;
        double y = points[p].y;

        int i = (int)(x / dx);
        int j = (int)(y / dy);

        if (i >= NX) i = NX - 1;
        if (j >= NY) j = NY - 1;

        double x1 = i * dx;
        double y1 = j * dy;

        double wx = (x - x1) / dx;
        double wy = (y - y1) / dy;

        int base = j * GRID_X + i;

        mesh_value[base] += (1.0 - wx) * (1.0 - wy);
        mesh_value[base + 1] += wx * (1.0 - wy);
        mesh_value[base + GRID_X] += (1.0 - wx) * wy;
        mesh_value[base + GRID_X + 1] += wx * wy;
    }
}

// ================= SERIAL MOVER =================
void mover_serial(Points *points, double deltaX, double deltaY)
{
    for (long long p = 0; p < NUM_Points; p++)
    {
        double newx, newy;

        do
        {
            double rx = (double)rand() / RAND_MAX;
            double ry = (double)rand() / RAND_MAX;

            double dxp = (2.0 * rx - 1.0) * deltaX;
            double dyp = (2.0 * ry - 1.0) * deltaY;

            newx = points[p].x + dxp;
            newy = points[p].y + dyp;

        } while (newx < 0.0 || newx > 1.0 || newy < 0.0 || newy > 1.0);

        points[p].x = newx;
        points[p].y = newy;
    }
}

// ================= PARALLEL MOVER =================
void mover_parallel(Points *points, double deltaX, double deltaY)
{
#pragma omp parallel
    {
        unsigned int seed = 1234 + omp_get_thread_num();

#pragma omp for schedule(static)
        for (long long p = 0; p < NUM_Points; p++)
        {
            double newx, newy;

            do
            {
                double rx = (double)rand_r(&seed) / RAND_MAX;
                double ry = (double)rand_r(&seed) / RAND_MAX;

                double dxp = (2.0 * rx - 1.0) * deltaX;
                double dyp = (2.0 * ry - 1.0) * deltaY;

                newx = points[p].x + dxp;
                newy = points[p].y + dyp;

            } while (newx < 0.0 || newx > 1.0 || newy < 0.0 || newy > 1.0);

            points[p].x = newx;
            points[p].y = newy;
        }
    }
}

// ================= SAVE MESH =================
void save_mesh(double *mesh_value)
{
    FILE *fd = fopen("Mesh.out", "w");
    if (!fd)
    {
        printf("Error creating Mesh.out\n");
        exit(1);
    }

    for (int j = 0; j < GRID_Y; j++)
    {
        for (int i = 0; i < GRID_X; i++)
        {
            fprintf(fd, "%lf ", mesh_value[j * GRID_X + i]);
        }
        fprintf(fd, "\n");
    }

    fclose(fd);
}
