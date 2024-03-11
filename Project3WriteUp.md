# Group 7 Project 3 Write-up

## Part 1: Blocking Ping-Pong

Your task is to implement the ping-pong problem using MPI in C or C++ and analyze the behavior and performance of your code. Specifically, you should:

1. Implement the ping-pong problem using MPI in C or C++. Use blocking `MPI_Send()` and `MPI_Recv()` calls. You should define the number of iterations and the size of the message to be exchanged.



2. Measure the time taken to complete the ping-pong exchange for different message sizes. You should use the `MPI_Wtime()` function to obtain the time before and after the exchange and calculate the elapsed time. Vary the message size from 2 bytes to 4 kilobytes in powers of 2 (i.e., 2 bytes, 4 bytes, 8 bytes,..., 2048 bytes, 4096 bytes). For each message size, perform 100 iterations of the ping-pong to build up statistical significance.

![Picture1](https://github.com/cmse822/project-3-mpi-p2p-seven-c-s/assets/143351616/84f71015-06bc-4951-aec6-a6110e865802)


3. Record the total amount of data sent and received during the ping-pong exchange for each configuration.
The data sent and received (bytes) was calculated as such - for 2 byte size, there were 100 iterations, so 2 * 100 * 4 = 800.
4 comes from the following steps:

Processor 0 sends to Processor 1 (1)

Processor 1 receives from Processor 0 (2)

Processor 1 sends to Processor 0 (3)

Processor 0 receives from Processor 1 (4)

| Byte Size | Time (seconds) | Data Sent and Received (bytes) |
| --------- | -------------- | ------------------------------ |
| 2         | 1.88E-06       | 800                            |
| 4         | 8.21E-07       | 16000                          |
| 8         | 8.14E-07       | 32000                          |
| 16        | 9.12E-07       | 64000                          |
| 32        | 9.27E-07       | 128000                         |
| 64        | 1.01E-06       | 256000                         |
| 128       | 9.80E-07       | 512000                         |
| 256       | 1.01E-06       | 1024000                        |
| 512       | 1.24E-06       | 2048000                        |
| 1024      | 2.16E-06       | 4096000                        |
| 2048      | 2.65E-06       | 8192000                        |
| 4096      | 4.79E-06       | 16384000                       |



4. Repeat steps 2 and 3 but ensure that the 2 processes that are communicating reside on different physical hardware nodes on HPCC.

![Picture2](https://github.com/cmse822/project-3-mpi-p2p-seven-c-s/assets/143351616/730b8ca6-fb17-4e2e-a192-0ec487da3625)

| Byte Size | Time (seconds) | Data Sent and Received (bytes) |
| --------- | -------------- | ------------------------------ |
| 2         | 0.00016257     | 800                            |
| 4         | 3.05E-05       | 16000                          |
| 8         | 3.09E-05       | 32000                          |
| 16        | 3.04E-05       | 64000                          |
| 32        | 3.05E-05       | 128000                         |
| 64        | 3.10E-05       | 256000                         |
| 128       | 3.37E-05       | 512000                         |
| 256       | 3.47E-05       | 1024000                        |
| 512       | 3.54E-05       | 2048000                        |
| 1024      | 3.61E-05       | 4096000                        |
| 2048      | 3.84E-05       | 8192000                        |
| 4096      | 4.51E-05       | 16384000                       |


5. Plot the average communication time of a single exchange (send and receive) as a function of message size for the two cases. Using this plot, estimate the _latency_ and _bandwidth_ for each case. Are they different? Explain your results.

<p style="color:red;"> we need a plot here </p>


6. Analyze and discuss your results. Explain the behavior of the resulting curves.

<p style="color:red;"> we need a written explanation here </p>



## Part 2: Non-block Ping-Pong

Repeat Part 1 using non-blocking MPI communication, i.e., using `MPI_Isend()` and `MPI_Irecv()`. You will need to include explicit process synchronization using, e.g., `MPI_Wait()` calls. Compare the results to the blocking case.

<p style="color:red;"> we need a table here </p>
<p style="color:red;"> we need a plot here </p>
<p style="color:red;"> we need a written explanation here </p>


## Part 3: MPI Ring Shift

1. Implement the MPI ring shift in C or C++ for an arbitrary number of processes in the ring and arbitrary message size (i.e., number of elements per process). In your implementation, use `MPI_Sendrecv()` instead of separate `MPI_Send()` and `MPI_Recv()` calls.



2. As in Parts 1 and 2, vary the message size from 2 bytes to 4 kb, in powers of 2. Also vary the number of processes used from 2 to `N`, in powers of 2, where `N` is sufficiently large that rank 0 and rank `N-1` are guaranteed to reside on separate nodes (`N` will depend on which cluster you are using on HPCC).

<p style="color:red;"> we need a table here </p>


3. Compute the bandwidth and latency, as above. Plot the bandwidth as a function of message size. Include separate lines for each number of processes used. 

<p style="color:red;"> we need a plot here </p>


4. Analyze and discuss your results. Explain the behavior of the resulting curves.

<p style="color:red;"> we need a written explanation here </p>


## Part 4: Non-blocking MPI Ring Shift

Repeat Part 3 but using non-blocking communication via `MPI_Isendrecv()`. Compare the results to the blocking case.

<p style="color:red;"> we need a table here </p>
<p style="color:red;"> we need a plot here </p>
<p style="color:red;"> we need a written explanation here </p>



