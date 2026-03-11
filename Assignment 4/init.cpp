#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "init.h"

/* Random particle initialization */
void initializepoints(Points *points) {
    /* seed once (caller may call this multiple times; seeding here is OK) */
    static int seeded = 0;
    if (!seeded) {
        seeded = 1;
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        srand((unsigned)(ts.tv_nsec ^ ts.tv_sec));
    }

    for (long long i = 0; i < NUM_Points; i++) {
        /* rand() is fine for initialization only; not used in mover hot loop */
        points[i].x = (double) rand() / (double) RAND_MAX;
        points[i].y = (double) rand() / (double) RAND_MAX;
    }
}
