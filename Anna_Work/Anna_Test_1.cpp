#include <iostream>
#include <mpi.h>
#include <vector>
#include <cstring>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int iterations = 100; // Number of ping-pong iterations

    // Required as ping pong communication is done between 2 processors
    if (size != 2) {
        std::cerr << "This program must be run with exactly 2 processors" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    double start_time, end_time;
    for (int message_size_bytes = 2; message_size_bytes <= 4096; message_size_bytes *= 2) {
        std::vector<char> send_buffer(message_size_bytes, 'a');
        std::vector<char> recv_buffer(message_size_bytes);

        double total_elapsed_time = 0.0;
        for (int iter = 0; iter < iterations; ++iter) {         // Running 100 iterations for each message size
            start_time = MPI_Wtime();  // Measure start time for both processes

            if (rank == 0) {
                // Processor 0 send/receives to/from Processor 1
                MPI_Send(send_buffer.data(), message_size_bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD); // processor 0 sends to processor 1
                MPI_Recv(recv_buffer.data(), message_size_bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // processor 0 receives from processor 1
            } else {
                // Processor 1 send/receives to/from Processor 0
                MPI_Recv(recv_buffer.data(), message_size_bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // processor 1 sends to processor 0
                MPI_Send(send_buffer.data(), message_size_bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD); // processor 1 receives from processor 0
            }

            end_time = MPI_Wtime();  // Measure end time for both processes

                     if (rank == 0) {
                double elapsed_time = end_time - start_time;
                total_elapsed_time += elapsed_time;
            }
        }

        if (rank == 0) {
            double average_elapsed_time = total_elapsed_time / iterations;
            std::cout << "Message size (bytes): " << message_size_bytes << ", Average elapsed time: " << average_elapsed_time << " seconds" << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
