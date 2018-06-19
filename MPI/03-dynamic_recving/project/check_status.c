/*
 * check_status.c
 *
 * Example showing how to query the MPI_Status object from an MPI_Recv call.
 *
 * Taken from https://github.com/wesleykendall/mpitutorial 
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size != 2)
    {
        fprintf(stderr, "Must use 2 MPI processes for this task\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int MAX_NUMBERS = 100;
    int numbers[MAX_NUMBERS];
    int number_amount;

    if (world_rank == 0)
    {
        // Pick a random amount of integers to send to MPI process 1.
        srand(time(NULL));
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("MPI process 0 sent %d numbers to MPI process 1\n", number_amount);
    }
    else if(world_rank == 1)
    {
        MPI_Status status;

        // Receive at most MAX_NUMBERS from MPI process 0.
        MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Get_count(&status, MPI_INT, &number_amount);
        printf("MPI process 1 received %d numbers from MPI process 0\n", number_amount);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
