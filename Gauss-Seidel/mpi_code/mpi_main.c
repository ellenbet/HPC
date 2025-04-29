#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/utils.h" 

int main(int argc, char **argv) {
    int rank, size;
    int kmax, jmax, imax, num_iters;
    int my_jmax;
    double ***my_arr, ***serial_arr, ***full_arr;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("\nProcess of rank %d and size %d running..", rank, size);

    if (size != 2) {
        if (rank == 0) {
            printf("ERROR: This program requires 2 MPI processes.\n");
        }
        MPI_Finalize();
        exit(1); 
    }

    if (argc != 5) {
        printf("ERROR with intial arguments: %s kmax jmax imax num_iters\n", argv[0]);
    }

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

    if (rank == 0){
        my_jmax = jmax / 2;
        printf("\nProcess 0, my_jmax: %d", my_jmax);
    } else {
        my_jmax = jmax;
        printf("\nProcess 1, my_jmax: %d", my_jmax);
    }

    // local array
    printf("\nAssigning memory to local array..");
    allocate_array3D(kmax, jmax, imax, &my_arr, 0);

    // full arrays for serial test and final global
    if (rank == 0) {
        printf("\nAssigning memory to serial and full array in rank 0..");
        allocate_array3D(kmax, jmax, imax, &serial_arr, 0);
        allocate_array3D(kmax, jmax, imax, &full_arr, 0);
        printf("\t..done!");
    }
  
    printf("\nRunning algorithm for rank %d..", rank);
    for (int iter = 0; iter < num_iters; iter++) {
        GS_iteration_2_chunks_mpi(rank, kmax, my_jmax, imax, my_arr);
    } 
    
    printf("\t..done!");
    // assemble data
    for (int i = 0; i < imax; i++) {
        if (rank == 0) {
            for (int j = 0; j < my_jmax - 1; j++) {
                for (int k = 0; k < kmax; k++) {
                    full_arr[i][j][k] = my_arr[i][j][k];
                }
            }
            MPI_Recv((&full_arr[i][my_jmax][0]), kmax, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (rank == 1) {
            MPI_Send((&my_arr[i][my_jmax/2][0]), kmax, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }
        

    // serial comparison 
    if (rank == 0) {
        for (int iter = 0; iter < num_iters; iter++) {
            GS_iteration_2_chunks(kmax, jmax, imax, serial_arr);
        }
        //print_cube(kmax, jmax, imax, serial_arr);
        //print_cube(kmax, jmax, imax, full_arr);

        // euclidian distance
        double diff = euclidean_distance(kmax, jmax, imax, full_arr, serial_arr);
        printf("\nEuclidean distance between serial and parallel: %e\n", diff);
    }

    MPI_Finalize();
    return 0;
}