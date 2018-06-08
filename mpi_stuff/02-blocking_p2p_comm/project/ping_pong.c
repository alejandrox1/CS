/*
 * ping_pong.c
 *
 * Sample usage of MPI_Send and MPI_Recv methods. Two processes ping-pong a
 * message between the two, inrementing it each time until it reaches a cetain
 * value.
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size < 2)
    {
        fprintf(stderr, "Number of MPI tasks must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int PING_PONG_LIMIT = 10;
    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 2;
    while (ping_pong_count < PING_PONG_LIMIT)
    {
        if (world_rank == ping_pong_count % 2)
        {
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            fprintf(stdout, "%d sent %d to %d\n", world_rank, ping_pong_count, partner_rank);
        }
        else
        {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fprintf(stdout, "\t%d received %d from %d\n", world_rank, ping_pong_count, partner_rank);
        }
    }

    MPI_Finalize();
    return 0;
}
