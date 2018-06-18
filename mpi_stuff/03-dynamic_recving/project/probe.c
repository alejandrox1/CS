/*
 * probe.c
 *
 * Example using MPI_Probe to find out message size before receiving it.
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
        fprintf(stderr, "Must use 2 MPI processes for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    int number_amount;

    if (world_rank == 0)
    {
        const int MAX_NUMBERS = 100;
        int numbers[MAX_NUMBERS];
    
        srand(time(NULL));
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("MPI process 0 sent %d numbers to MPI process 1\n", number_amount);
    }
    else if (world_rank == 1)
    {
        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &number_amount);

        int *number_buf = (int *)malloc(sizeof(int)*number_amount);

        MPI_Recv(number_buf, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("MPI process 1 dynamically allocated space for %d numbers which were sent by MPI process 0\n", number_amount);

        free(number_buf);
    }

    MPI_Finalize();
}
