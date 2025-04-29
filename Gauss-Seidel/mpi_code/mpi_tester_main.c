#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/utils.h" 

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

    if (size != 2) {
        if (rank == 0) {
            printf("ERROR: This program requires 2 MPI processes.\n");
        }
        MPI_Finalize();
        exit(1); 
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


    MPI_Finalize();
    return 0;
}