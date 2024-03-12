# Group 7 Project 3 Write-up

## Part 1: Blocking Ping-Pong

Your task is to implement the ping-pong problem using MPI in C or C++ and analyze the behavior and performance of your code. Specifically, you should:

1. Implement the ping-pong problem using MPI in C or C++. Use blocking `MPI_Send()` and `MPI_Recv()` calls. You should define the number of iterations and the size of the message to be exchanged.



2. Measure the time taken to complete the ping-pong exchange for different message sizes. You should use the `MPI_Wtime()` function to obtain the time before and after the exchange and calculate the elapsed time. Vary the message size from 2 bytes to 4 kilobytes in powers of 2 (i.e., 2 bytes, 4 bytes, 8 bytes,..., 2048 bytes, 4096 bytes). For each message size, perform 100 iterations of the ping-pong to build up statistical significance.

![Picture1](https://github.com/cmse822/project-3-mpi-p2p-seven-c-s/assets/143351616/84f71015-06bc-4951-aec6-a6110e865802)


3. Record the total amount of data sent and received during the ping-pong exchange for each configuration.

| Byte Size | Time (seconds) | Data Sent and Received (bytes) |
| --------- | -------------- | ------------------------------ |
| 2         | 1.88E-06       | 400                            |
| 4         | 8.21E-07       | 8000                           |
| 8         | 8.14E-07       | 16000                          |
| 16        | 9.12E-07       | 32000                          |
| 32        | 9.27E-07       | 64000                          |
| 64        | 1.01E-06       | 128000                         |
| 128       | 9.80E-07       | 256000                         |
| 256       | 1.01E-06       | 512000                         |
| 512       | 1.24E-06       | 1024000                        |
| 1024      | 2.16E-06       | 2048000                        |
| 2048      | 2.65E-06       | 4096000                        |
| 4096      | 4.79E-06       | 81920000                       |



4. Repeat steps 2 and 3 but ensure that the 2 processes that are communicating reside on different physical hardware nodes on HPCC.

![Picture2](https://github.com/cmse822/project-3-mpi-p2p-seven-c-s/assets/143351616/730b8ca6-fb17-4e2e-a192-0ec487da3625)

| Byte Size | Time (seconds) | Data Sent and Received (bytes) |
| --------- | -------------- | ------------------------------ |
| 2         | 0.00016257     | 400                            |
| 4         | 3.05E-05       | 8000                           |
| 8         | 3.09E-05       | 16000                          |
| 16        | 3.04E-05       | 32000                          |
| 32        | 3.05E-05       | 64000                          |
| 64        | 3.10E-05       | 128000                         |
| 128       | 3.37E-05       | 256000                         |
| 256       | 3.47E-05       | 512000                         |
| 512       | 3.54E-05       | 1024000                        |
| 1024      | 3.61E-05       | 2048000                        |
| 2048      | 3.84E-05       | 4096000                        |
| 4096      | 4.51E-05       | 81920000                       |


5. Plot the average communication time of a single exchange (send and receive) as a function of message size for the two cases. Using this plot, estimate the _latency_ and _bandwidth_ for each case. Are they different? Explain your results.

<img width="1160" alt="Screenshot 2024-03-11 at 3 32 19 PM" src="https://github.com/cmse822/project-3-mpi-p2p-seven-c-s/assets/143351616/fe3ada39-6c56-466b-bb27-d9addc761f5c">

*Note the following equation:*

$T(n) = \alpha + \beta n$ where $n$ is the number of bytes, $\alpha$ is the latency (measured in seconds), and $\beta ^{-1}$ is the bandwidth (measured in bytes/seconds). 

*The latency for ping-pong on the same nodes is 1E-06 seconds and the bandwidth for ping-pong on the same nodes is around 1.11 GB/s. The latency for ping-pong on different nodes is 5E-05 seconds and the bandwidth for ping-pong on different nodes is around 0.5 GB/s.* 


6. Analyze and discuss your results. Explain the behavior of the resulting curves.

*When going to different nodes, the time is much slower, and the bandwidth is smaller. Hence, the network is better inside the same node. The trend of the lines are similar in the sense that there is a drop between the first byte size and the remaining byte sizes. This may be due to a startup cost at the beginning of running this code. It is interesting to see that right now, the linear line for the graph of the different nodes has a downward trending slope, but this should change if more byte sizes were tested because the time would increase.* 



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
![bandwidth vs  bytes](https://github.com/cmse822/project-3-mpi-p2p-seven-c-s/assets/85922017/5d7c35cb-6747-41b7-a9ed-dfb12628a342)


<p style="color:red;"> we need a table here </p>
<p style="color:red;"> we need a plot here </p>
<p style="color:red;"> we need a written explanation here </p>



