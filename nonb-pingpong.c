#include <iostream>
#include<mpi.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv){

    //Initialize the MPI envinroment
    MPI_Init(&argc, &argv);
    int world_rank;
    int world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //We assume 2 processors for this task
    if(world_size != 2){
        printf("World size must be two.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Request request;
    MPI_Status status;
    int message_size;
    int N_iterations = 100;
    char message = 'a';

    double start_time, end_time, total_time = 0;
    double time_iter;
    for(message_size = 2; message_size <= 4096; message_size*=2){
        for(int iter = 0; iter < N_iterations; iter++){
            start_time = MPI_Wtime();

            if(world_rank == 0){
                MPI_Isend(&message, message_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &request);
                MPI_Irecv(&message, message_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &request);
            } else{
                MPI_Irecv(&message, message_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &request);
                MPI_Isend(&message, message_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &request);
                MPI_Wait(&request, &status);
            }
            end_time = MPI_Wtime();
            time_iter = end_time - start_time;
            total_time += time_iter;
        }
        printf("The total time of message size %d is %f", message_size, total_time);
    }

    MPI_Finalize();
    return 0;
}
