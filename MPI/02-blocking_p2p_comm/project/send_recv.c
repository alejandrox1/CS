/*
 * send_recv.c
 *
 * Make usage of the MPI_send and MPI_Recv methods to communiate an int between
 * two MPI tasks.
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size < 2)
    {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number;
    if (world_rank == 0)
    {
        number = -1;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        fprintf(stdout, "MPI task %d sent %d\n", world_rank, number);
    } 
    else if (world_rank == 1)
    {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        fprintf(stdout, "MPI task %d received %d\n", world_rank, number);
    }

    MPI_Finalize();
    return 0;
}
