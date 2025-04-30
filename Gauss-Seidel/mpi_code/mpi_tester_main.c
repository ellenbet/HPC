#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/utils.h" 

// testing main to check if functions work as they should, not meant to be displayed.

int main(int argc, char **argv) {
    int rank, size;
    int kmax, jmax, imax, num_iters;
    int my_jmax;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //double ***my_arr, ***serial_arr, ***full_arr;

    if (rank == 0) {
        kmax = atoi(argv[1]);
        jmax = atoi(argv[2]);
        imax = atoi(argv[3]);
        num_iters = atoi(argv[4]);
    }

    printf("\nBroadcasting parameters..");
    MPI_Bcast(&kmax, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&jmax, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&imax, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num_iters, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("\nParameters broadcasted i: %i, j: %d, k: %d..", imax, jmax, kmax);


    my_jmax = jmax/2 + 1;
    double ***old, ***new_left, ***new_left2, ***new_right, ***new_right2;;
    int verbose = 0;

    allocate_array3D(kmax, jmax, imax, &old, verbose);
    allocate_array3D(kmax, my_jmax, imax, &new_right, verbose);
    allocate_array3D(kmax, my_jmax, imax, &new_left, verbose);
    allocate_array3D(kmax, my_jmax, imax, &new_right2, verbose);

    int left_split = 0; // not correct anymore
    int right_split = 1;
    split_cube(imax, jmax, kmax, old, new_right, my_jmax, 'j', right_split);
    split_cube(imax, jmax, kmax, old, new_right2, my_jmax, 'j', right_split);
    split_cube(imax, jmax, kmax, old, new_left, my_jmax, 'j', left_split);
    split_cube(imax, jmax, kmax, old, new_left2, my_jmax, 'j', left_split);

    //print_cube(imax, my_jmax, kmax, new_right);
    //printf("\nSuccess!");

    //GS_iteration_2_chunks_mpi(rank, kmax, my_jmax, imax, new_right);
    //GS_iteration_2_chunks(kmax, my_jmax, imax, new_right2);

    printf("euclidian: %f", euclidean_distance(kmax, my_jmax, imax, new_right, new_right2));

    MPI_Finalize();
    return 0;
}