#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int provided, claimed;

    /* Select ONE of the following:
    MPI_Init_thread(0, 0, MPI_THREAD_SINGLE,     &provided); // level 0
    MPI_Init_thread(0, 0, MPI_THREAD_FUNNELED,   &provided); // level 1
    MPI_Init_thread(0, 0, MPI_THREAD_SERIALIZED, &provided); // level 2
    MPI_Init_thread(0, 0, MPI_THREAD_MULTIPLE,   &provided); // level 3
    */
    MPI_Init_thread(0, 0, MPI_THREAD_MULTIPLE,   &provided);
    
    MPI_Query_thread(&claimed);
    printf("Query thread level=%d Init_thread level=%d\n", claimed, provided);

    MPI_Finalize();
}
