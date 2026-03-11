#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// Serial interpolation 
void interpolation(double *mesh_value, Points *points)
{
    for (int p = 0; p < NUM_Points; p++)
    {
        double x = points[p].x;
        double y = points[p].y;

        int i = (int)(x / dx);
        int j = (int)(y / dy);

        if (i >= NX) i = NX - 1;
        if (j >= NY) j = NY - 1;

        double Xi = i * dx;
        double Yj = j * dy;

        double dx_local = (x - Xi) / dx;
        double dy_local = (y - Yj) / dy;

        double w00 = (1.0 - dx_local) * (1.0 - dy_local);
        double w10 = dx_local * (1.0 - dy_local);
        double w01 = (1.0 - dx_local) * dy_local;
        double w11 = dx_local * dy_local;

        double scale = dx * dy;

        int idx00 = j * GRID_X + i;
        int idx10 = j * GRID_X + (i + 1);
        int idx01 = (j + 1) * GRID_X + i;
        int idx11 = (j + 1) * GRID_X + (i + 1);

        mesh_value[idx00] += w00 * scale;
        mesh_value[idx10] += w10 * scale;
        mesh_value[idx01] += w01 * scale;
        mesh_value[idx11] += w11 * scale;
    }
}




// Write mesh to file
void save_mesh(double *mesh_value) {

    FILE *fd = fopen("Mesh.out", "w");
    if (!fd) {
        printf("Error creating Mesh.out\n");
        exit(1);
    }

    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            fprintf(fd, "%lf ", mesh_value[i * GRID_X + j]);
        }
        fprintf(fd, "\n");
    }

    fclose(fd);
}
