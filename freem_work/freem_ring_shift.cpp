#include <iostream>
#include <iomanip>
#include <vector>
#include <mpi.h>
#include <cmath>
#include <chrono>

// Function to perform MPI ring shift
void ring_shift(MPI_Comm comm, int message_size) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Allocate memory for message
    std::vector<int> message(message_size / sizeof(int), 0);

    double start_time, end_time;
    
    // Start timing
    start_time = MPI_Wtime();

    // Perform ring shift
    MPI_Sendrecv(&message[0], message_size / sizeof(int), MPI_INT, (rank + 1) % size, 0,
                    &message[0], message_size / sizeof(int), MPI_INT, (rank + size - 1) % size, 0,
                    comm, MPI_STATUS_IGNORE);

    // End timing
    end_time = MPI_Wtime();

    // Calculate latency and bandwidth
    double latency = (end_time - start_time) * 1000000 / 2; // in microseconds
    double bandwidth = (message_size * 8) / ((end_time - start_time) * 1024 * 1024); // in Mbps

    // Output latency and bandwidth
    if (rank == 0) {
        std::cout << "Message size: " << std::setw(5) << message_size << " bytes | "
                    << "Latency: " << std::fixed << std::setprecision(6) << latency << " µs | "
                    << "Bandwidth: " << std::fixed << std::setprecision(6) << bandwidth << " Mbps" << std::endl;
    }
    
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Define message sizes from 4 bytes to 4096 bytes in powers of 2
    for (int message_size = 4; message_size <= 4096; message_size *= 2) {
        // Perform ring shift for 100 iterations
        ring_shift(MPI_COMM_WORLD, message_size);
    }

    MPI_Finalize();
    return 0;
}