/*
 * ringc
 *
 * Sample usage of MPI_Send and MPI_Recv. Pass a message from process to
 * process.
 */
#include <mpi.h>
#include <stdio.h>

int main()
{
    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int msg;

    if (world_rank != 0)
    {
        MPI_Recv(&msg, 1, MPI_INT, world_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d from %d\n", world_rank, msg, world_rank-1);
    }
    else // Process 0 sets msg value.
    {
        msg = -1;
    }

    msg++;
    MPI_Send(&msg, 1, MPI_INT, (world_rank+1)%world_size, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        MPI_Recv(&msg, 1, MPI_INT, world_size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d from %d\n", world_rank, msg, world_size-1);
    }

    MPI_Finalize();
    return 0;
}
