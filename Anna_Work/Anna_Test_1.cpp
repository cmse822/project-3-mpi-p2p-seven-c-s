#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int iterations = 10; // Number of ping-pong iterations
    const int message_size = 100; // Size of the message to be exchanged

    if (size != 2) {
        std::cerr << "This program must be run with exactly 2 processes" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (int i = 0; i < iterations; ++i) {
        if (rank == 0) {
            // Process 0 sends a message to process 1
            MPI_Send(&message_size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message_size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            // Process 1 receives a message from process 0
            MPI_Recv(&message_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&message_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
