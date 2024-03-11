#include <iostream>
#include <iomanip>
#include <vector>
#include <mpi.h>
#include <cmath>
#include <chrono>

// Function to perform MPI ring shift
void ring_shift(MPI_Comm comm, int bytes) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Allocate memory for data
    int N_data = bytes / 4;
    std::vector<float> data(N_data);

    // populate 'data' with random floats:
    std::srand(static_cast<unsigned>(std::time(nullptr)) + rank);
    for (float& value : data) {
        value = static_cast<float>(std::rand()) / RAND_MAX;
    }

    double start_time, end_time;
    
    // Start timing
    start_time = MPI_Wtime();

    MPI_Isendrecv(&data.front(), N_data, MPI_FLOAT, (rank + 1) % size, 0,
                    &data.front(), N_data, MPI_FLOAT, (rank + size - 1) % size, 0,
                    comm);

    // End timing
    end_time = MPI_Wtime();

    // Calculate latency and bandwidth
    double latency = (end_time - start_time) * 1000000; // in microseconds
    double bandwidth = bytes / (end_time - start_time) / (1024 * 1024); // in Mbps

    // Output latency and bandwidth
    if (rank == 0) {
        std::cout << "data size: " << std::setw(5) << bytes << " bytes | "
                    << "Latency: " << std::fixed << std::setprecision(6) << latency << " µs | "
                    << "Bandwidth: " << std::fixed << std::setprecision(6) << bandwidth << " Mbps" << std::endl;
    }
    
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Define data sizes from 4 bytes to 4096 bytes in powers of 2
    for (int bytes = 4; bytes <= 4096; bytes *= 2) {
        // Perform ring shift for 100 iterations
        ring_shift(MPI_COMM_WORLD, bytes);
    }

    MPI_Finalize();
    return 0;
}