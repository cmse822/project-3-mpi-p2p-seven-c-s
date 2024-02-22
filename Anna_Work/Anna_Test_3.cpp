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
        double total_data_sent_recv = 0.0;
        for (int iter = 0; iter < iterations; ++iter) {         // Running 100 iterations for each message size
            start_time = MPI_Wtime();  // Measure start time for both processes

            if (rank == 0) {
                // Process 0 sends a message to process 1
                MPI_Send(send_buffer.data(), message_size_bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD); // sending to processor 1
                total_data_sent_recv += message_size_bytes; // Process 0 sends to process 1
                MPI_Recv(recv_buffer.data(), message_size_bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receiving from processor 1
                total_data_sent_recv += message_size_bytes; // Process 0 receives from process 1
            } else {
                // Process 1 receives a message from process 0
                MPI_Recv(recv_buffer.data(), message_size_bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receiving from processor 0
                total_data_sent_recv += message_size_bytes; // Process 1 sends to process 0
                MPI_Send(send_buffer.data(), message_size_bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD); // sending to processor 0
                total_data_sent_recv += message_size_bytes; // Process 1 receives from process 0
            }

            end_time = MPI_Wtime();  // Measure end time for both processes

            if (rank == 0) {
                double elapsed_time = end_time - start_time;
                total_elapsed_time += elapsed_time;
            }
        }

        if (rank == 0) {
            double average_elapsed_time = total_elapsed_time / iterations;
            std::cout << "Message size (bytes): " << message_size_bytes << ", Average elapsed time: " << average_elapsed_time << " seconds, Total data sent and received: " << total_data_sent_recv * 2 << " bytes" << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
