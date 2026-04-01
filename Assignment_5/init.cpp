#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "init.h"

void initializepoints(Points *points)
{
    static int seeded = 0;
    if (!seeded)
    {
        seeded = 1;
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        srand((unsigned)(ts.tv_nsec ^ ts.tv_sec));
    }

    for (long long i = 0; i < NUM_Points; i++)
    {
        points[i].x = (double)rand() / RAND_MAX;
        points[i].y = (double)rand() / RAND_MAX;
    }
}

/* Used after deletion */
void reinitialize_point(Points *p)
{
    p->x = (double)rand() / RAND_MAX;
    p->y = (double)rand() / RAND_MAX;
}
