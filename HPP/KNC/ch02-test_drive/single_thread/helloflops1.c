/*
 * helloflops1.c
 *
 * Simple SAXPY calculation.
 *
 * Taken from Intel Xeon Phi Coprocessor High-Performance Programming.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define FLOPS_ARRAY_SIZE (1024 * 1024)
#define MAXFLOPS_ITER 100000000
#define LOOP_COUNT 128
// Fused Multiply Add (FMA)
#if defined(__FMA__) && defined(__AVX__)
#define FLOPSPERCALC 2
#else
#define FLOPSPERCALC 1
#endif

// 64 byte aligned for fast cache access.
float fa[FLOPS_ARRAY_SIZE] __attribute__((aligned(64)));
float fb[FLOPS_ARRAY_SIZE] __attribute__((aligned(64)));

double dtime();

int main(void)
{
    int    i, j, k;
    double tstart, tstop, ttime;
    double gflops = 0.0;
    float  a      = 1.1;

    /* Initialize the compute arrays. */
    printf("Initializing...\n");
    for (i = 0; i < FLOPS_ARRAY_SIZE; ++i)
    {
        fa[i] = (float)i + 0.1;
        fb[i] = (float)i + 0.2;
    }

    printf("Starting compute...\n");

    tstart = dtime();
    // Loop many times to get some flops.
    for (j = 0; j < MAXFLOPS_ITER; ++j)
    {
        // Scale fa and add fb.
        for (k = 0; k < LOOP_COUNT; ++k)
            fa[k] = a * fa[k] + fb[k];
    }
    tstop = dtime();

    /* Calculate the number of FLOPS. */
    gflops = (double)(1.0e-9 * LOOP_COUNT * MAXFLOPS_ITER * FLOPSPERCALC);
    // elapsed time.
    ttime = tstop - tstart;

    /* Output results. */
    if ((ttime) > 0)
        printf("GFlop = %10.3lf, Secs = %10.3lf, GFlop/s = %10.3lf\n",
               gflops,
               ttime,
               gflops / ttime);

    exit(EXIT_SUCCESS);
}

/**
 * dtime - returns current wall clock time.
 */
double dtime()
{
    struct timeval mytime;
    gettimeofday(&mytime, NULL);
    return (double)(mytime.tv_sec + mytime.tv_usec * 1.0e-6);
}
