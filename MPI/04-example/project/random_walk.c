/*
 * random_walk.c
 *
 * Domain decomposition for a one-dimensional random walk with periodic
 * boundary conditions.
 *
 * Taken from https://github.com/wesleykendall/mpitutorial/blob
 */
#include <mpi.h>

void domain_decomposition(int domain_size, int world_rank, int world_size, 
        int *subdomain_start, int *subdomain_size);

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Finalize();
}

/**
 * domain_decomposition
 *
 * @domain_size: range for walkers to roam around.
 * @world_rank: rank of MPI task.
 * @world_size: number of MPI tasks for the given communicator.
 * @subdomain_start: start of subdomain for a given MPI tasks (pass value by
 *      reference).
 * @subdomain_size: size fo subdomain for a given MPI task (pass value by
 *      reference).
 */
void domain_decomposition(int domain_size, int world_rank, int world_size, 
        int *subdomain_start, int *subdomain_size)
{
    // TODO: Ignore the special case. Assume the domain_size is greater than
    // the world_size (each MPI task gets a large chunk).
    if (world_size > domain_size)
        MPI_Abort(MPI_COMM_WORLD, 1);

    *subdomain_start = (domain_size / world_size) * world_rank;
    *subdomain_size  = domain_size / world_size;

    // Give the last MPI tasks the remainder of the domain.
    if (world_rank == world_size-1)
        *subdomain_size += domain_size % world_size;
}