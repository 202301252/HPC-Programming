#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "utils.h"
#include "init.h"

// ================= FAST RNG (random number generator) =================
inline unsigned int xorshift32(unsigned int &state)
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

inline double fast_rand(unsigned int &state)
{
    return (double)xorshift32(state) / 4294967295.0;
}

// ================= INTERPOLATION =================
void interpolation(double *mesh_value, Points *points)
{
    for (long long p = 0; p < NUM_Points; p++)
    {
        Points *pt = &points[p];

        double x = pt->x;
        double y = pt->y;

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

// ================= SERIAL MOVER (IMMEDIATE) =================
long long mover_serial(Points *points, double deltaX, double deltaY)
{
    unsigned int seed = 1234;
    long long deleted = 0;

    for (long long p = 0; p < NUM_Points; p++)
    {
        Points *pt = &points[p];

        double rx = fast_rand(seed);
        double ry = fast_rand(seed);

        double dxp = (2.0 * rx - 1.0) * deltaX;
        double dyp = (2.0 * ry - 1.0) * deltaY;

        double newx = pt->x + dxp;
        double newy = pt->y + dyp;

        if (newx < 0.0 || newx > 1.0 || newy < 0.0 || newy > 1.0)
        {
            deleted++;
            pt->x = fast_rand(seed);
            pt->y = fast_rand(seed);
        }
        else
        {
            pt->x = newx;
            pt->y = newy;
        }
    }

    return deleted;
}

// ================= SERIAL MOVER (DEFERRED) =================
long long mover_serial_deferred(Points *points, double deltaX, double deltaY)
{
    unsigned int seed = 1234;
    long long write_idx = 0;

    for (long long p = 0; p < NUM_Points; p++)
    {
        Points *pt = &points[p];

        double rx = fast_rand(seed);
        double ry = fast_rand(seed);

        double dxp = (2.0 * rx - 1.0) * deltaX;
        double dyp = (2.0 * ry - 1.0) * deltaY;

        double newx = pt->x + dxp;
        double newy = pt->y + dyp;

        if (newx >= 0.0 && newx <= 1.0 && newy >= 0.0 && newy <= 1.0)
        {
            Points *dest = &points[write_idx];
            dest->x = newx;
            dest->y = newy;
            write_idx++;
        }
    }

    for (long long p = write_idx; p < NUM_Points; p++)
    {
        Points *pt = &points[p];
        pt->x = fast_rand(seed);
        pt->y = fast_rand(seed);
    }

    return NUM_Points - write_idx;
}

// ================= PARALLEL MOVER (IMMEDIATE) =================
long long mover_parallel(Points *points, double deltaX, double deltaY)
{
    long long deleted = 0;

#pragma omp parallel reduction(+:deleted)
    {
        unsigned int seed = 1234 + omp_get_thread_num();

#pragma omp for schedule(static)
        for (long long p = 0; p < NUM_Points; p++)
        {
            Points *pt = &points[p];

            double rx = fast_rand(seed);
            double ry = fast_rand(seed);

            double dxp = (2.0 * rx - 1.0) * deltaX;
            double dyp = (2.0 * ry - 1.0) * deltaY;

            double newx = pt->x + dxp;
            double newy = pt->y + dyp;

            if (newx < 0.0 || newx > 1.0 || newy < 0.0 || newy > 1.0)
            {
                deleted++;
                pt->x = fast_rand(seed);
                pt->y = fast_rand(seed);
            }
            else
            {
                pt->x = newx;
                pt->y = newy;
            }
        }
    }

    return deleted;
}

// ================= PARALLEL MOVER (DEFERRED) =================
long long mover_parallel_deferred(Points *points, double deltaX, double deltaY)
{
    int nthreads = omp_get_max_threads();

    Points **local_buf = (Points **)malloc(nthreads * sizeof(Points *));
    long long *local_count = (long long *)calloc(nthreads, sizeof(long long));

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        long long chunk = (NUM_Points + nthreads - 1) / nthreads;
        local_buf[tid] = (Points *)malloc(chunk * sizeof(Points));

        unsigned int seed = 1234 + tid;
        long long count = 0;

#pragma omp for schedule(static)
        for (long long i = 0; i < NUM_Points; i++)
        {
            Points *pt = &points[i];

            double rx = fast_rand(seed);
            double ry = fast_rand(seed);

            double dxp = (2.0 * rx - 1.0) * deltaX;
            double dyp = (2.0 * ry - 1.0) * deltaY;

            double newx = pt->x + dxp;
            double newy = pt->y + dyp;

            if (newx >= 0.0 && newx <= 1.0 &&
                newy >= 0.0 && newy <= 1.0)
            {
                Points *dest = &local_buf[tid][count];
                dest->x = newx;
                dest->y = newy;
                count++;
            }
        }

        local_count[tid] = count;
    }

    long long *offset = (long long *)malloc(nthreads * sizeof(long long));
    offset[0] = 0;

    for (int t = 1; t < nthreads; t++)
        offset[t] = offset[t - 1] + local_count[t - 1];

    long long total_valid = offset[nthreads - 1] + local_count[nthreads - 1];

#pragma omp parallel for
    for (int t = 0; t < nthreads; t++)
        for (long long j = 0; j < local_count[t]; j++)
            points[offset[t] + j] = local_buf[t][j];

#pragma omp parallel
    {
        unsigned int seed = 5678 + omp_get_thread_num();

#pragma omp for
        for (long long i = total_valid; i < NUM_Points; i++)
        {
            Points *pt = &points[i];
            pt->x = fast_rand(seed);
            pt->y = fast_rand(seed);
        }
    }

    for (int t = 0; t < nthreads; t++)
        free(local_buf[t]);

    free(local_buf);
    free(local_count);
    free(offset);

    return NUM_Points - total_valid;
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
