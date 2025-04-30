#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/utils.h" 

int main(int argc, char **argv) {
    int rank, size;
    int kmax, jmax, imax, num_iters;
    double ***my_arr, ***serial_arr1, ***serial_arr2, ***full_arr;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("\nProcess of rank %d and size %d running..", rank, size);

    if (size != 2) {
        if (rank == 0) {
            printf("ERROR: Program requires 2 MPI processes.\n");
        }
        MPI_Finalize();
        exit(1); 
    }

    if (rank == 0) {
        if (argc != 5) {
            printf("ERROR: Intial arguments: %s kmax jmax imax num_iters\n", argv[0]);
        }

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
    printf("\nParameters broadcasted imax: %d, jmax: %d, kmax: %d", imax, jmax, kmax);
    int my_jmax = jmax/2 + 1;
    printf(" and my_jmax: %d", my_jmax);

    // local array and full array
    printf("\nAssigning memory to local array..");
    allocate_array3D(kmax, my_jmax, imax, &my_arr, 0); // final 0 means verbose = 0
    allocate_array3D(kmax, jmax, imax, &full_arr, 0);

    // full arrays for serial test and final global
    if (rank == 0) {
        printf("\nAssigning memory to serial and full array in rank 0..");
        // making to serial arrays to triple check that both serial functions work as they should
        allocate_array3D(kmax, jmax, imax, &serial_arr1, 0);
        allocate_array3D(kmax, jmax, imax, &serial_arr2, 0);
    }

    // splitting cube based on rank
    split_cube(imax, jmax, kmax, full_arr, my_arr, my_jmax, 'j', rank);
  
    printf("\nRunning algorithm for rank %d..", rank);
    for (int iter = 0; iter < num_iters; iter++) {
        // this seems to work
        GS_iteration_2_chunks_mpi(rank, kmax, my_jmax, imax, my_arr);
    } 
    printf("..done with GS for rank %d!", rank);

    // assemble data
    int total_doubles = imax * (my_jmax) * kmax;
    // trouble here now
    if (rank == 0){
        int offset = my_jmax - 2;
        printf("\nAssembling full matrix..");
        MPI_Recv((&full_arr[0][0][0]), total_doubles, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 1; i < imax - 1; i++){
            for (int j = 1; j < my_jmax - 1; j++) {
                for (int k = 0; k < kmax; k++) {
                    full_arr[i][j + offset][k] = my_arr[i][j][k];
                }
            }
        }

        if (euclidean_distance(kmax, jmax, imax, full_arr, serial_arr1) == 0){
            printf("\n\nERROR: no changes have been made to the full array, exiting program\n");
            return 1;
        }

    } else if (rank == 1){
        MPI_Send(&my_arr[0][0][0], total_doubles, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }


    if (rank == 0){
        for (int iter = 0; iter < num_iters; iter++) {
            GS_iteration_2_chunks(kmax, jmax, imax, serial_arr1);
            GS_iteration_normal(kmax, jmax, imax, serial_arr2);
        }

        // euclidian distance
        double diff1 = euclidean_distance(kmax, jmax, imax, full_arr, serial_arr1);
        double diff2 = euclidean_distance(kmax, jmax, imax, serial_arr2, serial_arr1);
        printf("\nEuclidean distance between two serial implementations: %e\n", diff2);
        printf("\nEuclidean distance between serial and parallel: %e\n", diff1);
    }
    
        
    MPI_Finalize();
    return 0;
}