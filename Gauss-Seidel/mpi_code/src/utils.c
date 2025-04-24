/*
In this assignment, we assume a 3D array of size kmax×jmax×imax, where kmax denotes the total
number of points in the k-direction (including the two boundary points), and similarly for jmax and
imax. The computation over the 3D array is done by a number of iterations, where the computational
work per iteration can be implemented in the following “normal” manner:

void GS_iteration_normal (int kmax, int jmax, int imax, double ***phi){
    int k,j,i;
    for (k=1; k<kmax-1; k++)
    for (j=1; j<jmax-1; j++)
    for (i=1; i<imax-1; i++)
    phi[k][j][i] = (phi[k-1][j][i] + phi[k][j-1][i]
    +phi[k][j][i-1] + phi[k][j][i+1]
    +phi[k][j+1][i] + phi[k+1][j][i])/6.0;
}

Note: The computation happens only on the interior points, so the boundary points remain
unchanged. In each direction, there is dependency betweeen the loop indices, thus parallelization
can not be directly applied. However, as explained in Section 6.3 of the textbook, there are other
ways of traversing the 3D array that respect the dependency while producing the same computational
result. One idea is to traverse the 3D array as successive wavefronts, where the computational work
on each wavefront can be simultaneously carried out by multiple processes (in MPI) or threads (in
OpenMP).

*/



// The assignment: 

/*

You are requested to parallelize the Guass-Seidel algorithm using two chunks, so that two MPI
processes can divide the work on each wavefront.

*/

#include"include/utils.h"

void GS_iteration_2_chunks (int kmax, int jmax, int imax, double ***phi){
    // first wavefront: only computation on left chunk at level k=1
    // ...
    for (int k = 2; k <= kmax - 2; k++) {
    // computation on left chunk at level k
    // ...
    // computation on right chunk at level k-1
    // ...
    }
    // last wavefront: only computation on right chunk at level k=kmax-2
    // ...
}

void GS_iteration_2_chunks_mpi (int my_rank, int kmax, int my_jmax, int imax, double ***my_phi){
    /*
    void GS_iteration_2_chunks_mpi (int my_rank, int kmax, int my_jmax, int imax, double ***my_phi)
    Different from the serial counterpart GS iteration 2 chunks, this MPI version has one additional
    input argument my rank that has value either 0 or 1. That is, GS iteration 2 chunks mpi is to be
    called by MPI process with rank 0 or 1. Moreover, the input 3D array my phi per MPI process is of
    size kmax×my jmax×imax, with the value of my jmax equal jmax/2+1 (assuming jmax is divisible by
    2). This is because each process is assigned with jmax/2-1 computational points in the j-direction,
    plus one end-point being a boundary point and the other end-point being a ghost point (needed for
    receiving data from the neighbor process). Remember to implement the needed inter-process
    communication after the computation is done on each wavefront.
    You are also requested to implement an MPI version of the main function, which lets process 0 read
    values of num iters, kmax, jmax and imax from the command line. These are then broadcasted to
    process 1. Thereafter, each process allocates a 3D array named my arr of size kmax×my jmax×imax,
    with my jmax equal jmax/2+1. After assigning suitable initial values to my arr, both processes call
    num iters iterations of GS iteration 2 chunks mpi. Thereafter, process 1 sends the content of
    its computed 3D array my arr to process 0, which can thereby construct a global 3D array of size
    kmax×jmax×imax. Finally, process 0 can compare this global 3D array with the computational result
    produced by running the serial function GS iteration 2 chunks (using the same initial values and
    number of iterations) for validating the correctness of the MPI implementation.
    
    */

}

void allocate_array3D (int kmax, int jmax, int imax, double ****array){

}

double euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2){

}