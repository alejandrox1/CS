/*
 * random_walk.cc
 *
 * Domain decomposition for a one-dimensional random walk with periodic
 * boundary conditions.
 *
 * Taken from https://github.com/wesleykendall/mpitutorial/blob
 */
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

/**
 * Walker is a random walker data structure.
 */
typedef struct
{
    int location;
    int steps_left_in_walk;
} Walker;


void domain_decomposition(int domain_size, int world_rank, int world_size, 
        int *subdomain_start, int *subdomain_size);

void initialize_walkers(int walkers_per_proc, int max_walk_size, 
        int subdomain_start, std::vector<Walker> *incoming_walkers);

void walk(Walker *walker, int subdomain_start, int subdomain_size, 
        int domain_size, std::vector<Walker> *outgoing_walkers);

void send_outgoing_walkers(std::vector<Walker> *outgoing_walkers, 
        int world_rank, int world_size);


void receive_incoming_walkers(std::vector<Walker> *incoming_walkers,
        int world_rank, int world_size);



int main(int argc, char **argv)
{
    int domain_size, max_walk_size, walkers_per_proc;
    if (argc < 4)
    {
        std::cerr << "Usage: ./random_walk domain_size max_walk_size walkers_per_proc\n";
        exit(1);
    }
    domain_size      = atoi(argv[1]);
    max_walk_size    = atoi(argv[2]);
    walkers_per_proc = atoi(argv[3]);

    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(time(NULL) * world_rank);
    int subdomain_start, subdomain_size;
    std::vector<Walker> incoming_walkers, outgoing_walkers;

    // Begin by decomposing the domain.
    domain_decomposition(domain_size, world_rank, world_size, 
            &subdomain_start, &subdomain_size);

    // Initialize walkers in their subdomains.
    initialize_walkers(walkers_per_proc, max_walk_size, 
            subdomain_start, &incoming_walkers);

    std::cout << "MPI process " << world_rank << " initiated " << walkers_per_proc
        << " walkers in subdomain [" << subdomain_start << ", " 
        << subdomain_start + subdomain_size-1 << "]\n";

    // Determine the maximum number of sends and reeives needed to complete.
    int max_sends_recvs = max_walk_size / (domain_size / world_size) + 1;
    for (int m=0; m<max_sends_recvs; ++m)
    {
        // Process all incoming walkers.
        for (int i=0; i<incoming_walkers.size(); ++i)
        {
            walk(&incoming_walkers[i], subdomain_start, subdomain_size,
                    domain_size, &outgoing_walkers);

            std::cout << "MPI process " << world_rank << " sending " 
                << outgoing_walkers.size() << " outgoing walkers to MPI process "
                << (world_rank + 1) % world_size << "\n";

            // First MPI process will start by sending, so second proc should
            // receive any outbound walkers, and so on and so forth.
            if (world_rank % 2 == 0)
            {
                // Send all outgoing walkers to next MPI process.
                send_outgoing_walkers(&outgoing_walkers, world_rank, world_size);

                // Receive all incoming walkers.
                receive_incoming_walkers(&incoming_walkers, world_rank, world_size);
            }
            else
            {
                // Receive all incoming walkers.
                receive_incoming_walkers(&incoming_walkers, world_rank, world_size);

                // Send all outgoing walkers to next MPI process.
                send_outgoing_walkers(&outgoing_walkers, world_rank, world_size);
            }

            std::cout << "MPI process " << world_rank << " received " 
                << incoming_walkers.size() << " incoming walkers\n";
        }
    }
    std::cout << "Process " << world_rank << " done\n";

    MPI_Finalize();
    return 0;
}

/**
 * domain_decomposition
 *
 * domain_size: range for walkers to roam around.
 * world_rank: rank of MPI task.
 * world_size: number of MPI tasks for the given communicator.
 * subdomain_start: start of subdomain for a given MPI tasks (pass value by
 *      reference).
 * subdomain_size: size fo subdomain for a given MPI task (pass value by
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

/**
 * initialize_walkers takes subdomain bounds and adds walkers to a vector.
 *
 * walkers_per_proc: number of walkers per MPI process.
 * max_walk_size: maximum length a walker can walk.
 * subdomain_start: start of subdomain for MPI process.
 * incoming_walker: vector of walkers.
 */
void initialize_walkers(int walkers_per_proc, int max_walk_size,
        int subdomain_start, std::vector<Walker> *incoming_walkers)
{
    for (int i=0; i<walkers_per_proc; ++i)
    {
        Walker walker;
        walker.location           = subdomain_start;
        walker.steps_left_in_walk = (rand() / (float)RAND_MAX) * max_walk_size;

        incoming_walkers->push_back(walker);
    }
}

/**
 * walk moves a walker along. If a walker goes out of bounds then it is added
 * to outgoing_walkers.
 *
 * walker: Walker structure.
 * subdomain_start: start of subdomain for a given MPI process.
 * subdomain_size: size fo subdomain for a give MPI process.
 * domain_size: size of entire domain.
 * outgoing_walkers: walkers that have gone out of bounds for a given
 * subdomain.
 */
void walk(Walker *walker, int subdomain_start, int subdomain_size,
        int domain_size, std::vector<Walker> *outgoing_walkers)
{
    while (walker->steps_left_in_walk > 0)
    {
        // Walker is walking out of the domain then go back to begining.
        if (walker->location == subdomain_start + subdomain_size)
        {
            if (walker->location == domain_size)
                walker->location = 0;

            outgoing_walkers->push_back(*walker);
            break;
        }
        else
        {
            ++walker->location;
            --walker->steps_left_in_walk;
        }
    }
}


/**
 * send_outgoing_walkers sends outgoing walkers to next subdomain (MPI
 * process).
 *
 * outgoing_walkers: vector of walkers who have gone out of range for their
 * initial subdomain.
 * world_rank: MPI process rank.
 * world_size: number of MPI processes in the MPI communicator.
 */
void send_outgoing_walkers(std::vector<Walker> *outgoing_walkers,
        int world_rank, int world_size)
{
    //Send data as an array of MPI_BYTEs.
    MPI_Send((void *)outgoing_walkers->data(), 
            outgoing_walkers->size() * sizeof(Walker),
            MPI_BYTE,
            (world_rank + 1) % world_size,
            0,
            MPI_COMM_WORLD);

    outgoing_walkers->clear();
}


/**
 * receive_incoming_walkers receives the walkers sent by walking off their
 * initial subdomains.
 *
 * incoming_walkers: vector of incoming walkers.
 * world_rank: MPI process rank.
 * world_size: number of MPI processes in the MPI communicator.
 */
void receive_incoming_walkers(std::vector<Walker> *incoming_walkers,
        int world_rank, int world_size)
{
    int rank_of_incoming = (world_rank==0) ? world_size-1 : world_rank-1;

    int incoming_walkers_size;
    MPI_Status status;
    MPI_Probe(rank_of_incoming, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_BYTE, &incoming_walkers_size);

    incoming_walkers->resize(incoming_walkers_size / sizeof(Walker));
    
    MPI_Recv((void *)incoming_walkers->data(), incoming_walkers_size, 
            MPI_BYTE, rank_of_incoming, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}
