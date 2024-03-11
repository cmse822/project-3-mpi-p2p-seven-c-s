#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <mpi.h>
#include <unistd.h> // For sleep() function

std::vector<float> generate_data(int N_data) {
    /**
     * Function which takes desired size (in bytes) of a data object, and returns a
     * vector object of that size. 
     * 
     * Float values require 4 bytes, so N = bytes / 4
     * This means the smallest object we can pass if 4 bytes large.
     * */

    // Initialize a vector called 'data' that is N long
    std::vector<float> data(N_data);

    // Seed the random number generator based on time
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Initialize data with random floats
    for (float& value : data) {
        value = static_cast<float>(std::rand()) / RAND_MAX;
    }
    return data;
}

int main(int argc, char* argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;             
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int name_length = MPI_MAX_PROCESSOR_NAME;       // For outputting node information

    // Throw an error if run without at least 2 processors
    if (size < 2) {
        std::cerr << "This program requires at least 2 processors." << std::endl;
        std::cout << "Breaking due to processor number" << std::endl;
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // Initialize variables for timing scheme
    double starttime_1, starttime_2, endtime_1, endtime_2, starttime_tot, endtime_tot;

    // size of data object in bytes
    int bytes = atoi(argv[1]);
    int N_data = bytes / 4;     // How many entries in the vector required to achieve data size

    // Initialize a vector called 'data' that is the size of 'bytes'
    std::vector<float> data(N_data);

    // Open a file called "data.txt" to save run information
    std::ofstream outputFile("pp_NB_data" + std::to_string(bytes) + ".txt");
    std::time_t time_obj = std::time(nullptr);
    // Output file heading comments
    outputFile << "# Ping pong run at " << std::asctime(std::localtime(&time_obj));
    outputFile << "# Data Size " << bytes << std::endl;

    if(rank==0) {
        char proc_name0[name_length], proc_name1[name_length];
        MPI_Get_processor_name(proc_name0, &name_length);
        MPI_Recv(proc_name1, name_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        outputFile << "# Processor 0 with MPI name: " << proc_name0 << std::endl;
        outputFile << "# Processor 1 with MPI name: " << proc_name1 << std::endl; 
    } else if(rank == 1) {
        char proc_name1[name_length];
        MPI_Get_processor_name(proc_name1, &name_length);
        // Send the name of the processor 1 to processor 0
        MPI_Send(proc_name1, name_length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    outputFile << "# iteration, total time taken" << std::endl;

    for(int i=1; i<=100; i++) {
        // Start universal clock
        starttime_tot = MPI_Wtime();

        MPI_Request req[2];

        if (rank == 0) {
            // Fill data vector with random values
            data = generate_data(N_data);

            // Start clock
            starttime_1 = MPI_Wtime();

            // Send data from processor 0 to processor 1
            // Note here we use data.front() as the memory location of the first vector entry
            MPI_Isend(&data.front(), N_data, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &req[0]);

            // Receive data back from processor 1
            MPI_Irecv(&data.front(), N_data, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &req[1]);
            MPI_Waitall(2, req, MPI_STATUS_IGNORE);
            endtime_1 = MPI_Wtime();
        } else if (rank == 1) {
            starttime_2 = MPI_Wtime();
            MPI_Irecv(&data.front(), N_data, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req[0]);
            MPI_Isend(&data.front(), N_data, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req[1]);
            MPI_Waitall(2, req, MPI_STATUS_IGNORE);
            endtime_2 = MPI_Wtime();
        }

        endtime_tot = MPI_Wtime();

        if(rank==0){
            outputFile << i << ",  " << endtime_tot - starttime_tot << std::endl;
        }

        usleep(200);
    }

    MPI_Finalize();
    outputFile.close();
    return 0;
}