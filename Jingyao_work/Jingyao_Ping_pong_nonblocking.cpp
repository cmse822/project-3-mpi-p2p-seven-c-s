#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstring>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        std::cerr << "This program requires at least two processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int iteration = 100;
    double start_time, end_time;
    for (int message_size_bytes = 2; message_size_bytes <= 4096; message_size_bytes *= 2) {
        std::vector<char> send_buffer(message_size_bytes, 'a');
        std::vector<char> recv_buffer(message_size_bytes);

        double total_elapsed_time = 0.0;
        double total_data_sent_recv = 0.0;
        for (int iter = 0; iter < iteration; ++iter) {
            double start_time = MPI_Wtime();

            MPI_Request send_request, recv_request;

            if (rank == 0) {
                MPI_Isend(send_buffer.data(), message_size_bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &send_request);
                MPI_Irecv(recv_buffer.data(), message_size_bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &recv_request);
            } else {
                MPI_Irecv(recv_buffer.data(), message_size_bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &recv_request);
                MPI_Isend(send_buffer.data(), message_size_bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &send_request);
            }

            // Wait for both send and receive to complete
            MPI_Wait(&send_request, MPI_STATUS_IGNORE);
            MPI_Wait(&recv_request, MPI_STATUS_IGNORE);

            double end_time = MPI_Wtime();

            if (rank == 0) {
                double elapsed_time = end_time - start_time;
                total_elapsed_time += elapsed_time;
                total_data_sent_recv += 2 * message_size_bytes;  // Counting both send and receive for each iteration
            }
        }

        if (rank == 0) {
            double average_elapsed_time = total_elapsed_time / iteration;
            std::cout << "Message size (bytes): " << message_size_bytes
                      << ", Average elapsed time: " << average_elapsed_time << " seconds"
                      << ", Total data sent and received: " << total_data_sent_recv << " bytes" << std::endl;
        }
    }
    MPI_Finalize();
    return 0;
}
