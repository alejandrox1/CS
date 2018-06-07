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
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define FLOPS_ARRAY_SIZE (1024*1024)
#define MAXFLOPS_ITER 100000000
#define LOOP_COUNT 128
// Fused Multiply Add (FMA)
#if defined(__FMA__) && defined(__AVX__)
# define FLOPSPERCALC 2
#else
# define FLOPSPERCALC 1
#endif

// 64 byte aligned for fast cache access.
float fa[FLOPS_ARRAY_SIZE] __attribute__((aligned(64)));
float fb[FLOPS_ARRAY_SIZE] __attribute__((aligned(64)));

double dtime();


int main(int argc, char** argv)
{
    int i, j, k;
    int numthreads;
    double tstart, tstop, ttime;
    double gflops = 0.0;
    float a = 1.1;

    int num_threads = 2;
    if (argc ==2)
        num_threads = strtol(argv[1], NULL, 10);

    /* Initialize the compute arrays. */
    printf("Initializing...\n");
    //omp_set_dynamic(0); // Explicitly disable dynamic teams.
    omp_set_num_threads(num_threads);
#if defined(__INTEL_COMPILER) && defined(_OPENMP)
    kmp_set_defaults("KMP_AFFINITY=compact");
#endif

#pragma omp parallel for
    for (i=0; i<FLOPS_ARRAY_SIZE; ++i)
    {
        if (i==0) numthreads = omp_get_num_threads();
        fa[i] = (float)i + 0.1;
        fb[i] = (float)i + 0.2;
    }

    printf("Starting compute on %d threads...\n", numthreads);

    tstart = dtime();
    // Loop many times to get some flops.
    // Use omp to scale calculation - need to set the env variables
    // OMP_NUM_THREADS and KMP_AFFINITY.
#pragma omp parallel private(j, k)
    for (i=0; i<numthreads; ++i)
    {
        // Eah thread works on its own section.
        int offset = i * LOOP_COUNT;

        for (j=0; j<MAXFLOPS_ITER; ++j)
        {
            // Scale fa and add fb.
            for (k=0; k<LOOP_COUNT; ++k)
                fa[k+offset] = a * fa[k+offset] + fb[k+offset];
        }
    }
    tstop = dtime();

    /* Calculate the number of FLOPS. */
    gflops = (double)(1.0e-9 * numthreads * LOOP_COUNT * MAXFLOPS_ITER * FLOPSPERCALC);
    // elapsed time.
    ttime = tstop - tstart;

    /* Output results. */
    if ((ttime) > 0)
        printf("GFlop = %10.3lf, Secs = %10.3lf, GFlop/s = %10.3lf\n",
                gflops, ttime, gflops/ttime);

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
