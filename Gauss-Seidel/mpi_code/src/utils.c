#include"../include/utils.h"
#include <stdio.h>
#include<stdlib.h>
#include <mpi.h>
#include<math.h>

void GS_iteration_normal (int kmax, int jmax, int imax, double ***phi){
    int k,j,i;
    for (i = 2; i<imax-1; i++){
        for (j=1; j<jmax-1; j++){
            for (k=1; k<kmax-1; k++){
                phi[i][j][k] = (phi[i-1][j][k] + phi[i][j-1][k]
                +phi[i][j][k-1] + phi[i][j][k+1]
                +phi[i][j+1][k] + phi[i+1][j][k])/6.0;
            }
        }
    }
}

void GS_iteration_2_chunks (int kmax, int jmax, int imax, double ***phi){
    int jmid;
    jmid = jmax/2 - 1;

    for (int i = 2; i <= imax - 2; i++){
        for (int j = 1; j < jmid; j++){
            for (int k = 1; k < kmax - 1; k++){
                phi[i][j][k] = (phi[i-1][j][k] + phi[i][j-1][k]
                +phi[i][j][k-1] + phi[i][j][k+1]
                +phi[i][j+1][k] + phi[i+1][j][k])/6.0;
            }
        }

        for (int j = jmid; j < jmax - 1; j++){
            for (int k = 1; k < kmax - 1; k++){
                phi[i][j][k] = (phi[i-1][j][k] + phi[i][j-1][k]
                +phi[i][j][k-1] + phi[i][j][k+1]
                +phi[i][j+1][k] + phi[i+1][j][k])/6.0;
            }
        }
    }
}

void GS_iteration_2_chunks_mpi(int my_rank, int kmax, int my_jmax, int imax, double ***my_phi){
    for (int i = 2; i <= imax - 2; i++){
        if (my_rank == 0){
            for (int j = 1; j < my_jmax - 1; j++){
                for (int k = 1; k < kmax - 1; k++){
                    my_phi[i][j][k] = (my_phi[i-1][j][k] + my_phi[i][j-1][k]
                    +my_phi[i][j][k-1] + my_phi[i][j][k+1]
                    +my_phi[i][j+1][k] + my_phi[i+1][j][k])/6.0;
                }
            }
        }
    }
    /*
            MPI_Send(my_phi[i][my_jmax - 2], kmax, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
        }
        else {
            double left;
            double *buffer = malloc(kmax * sizeof(double));
            MPI_Recv(buffer, kmax, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < my_jmax - 1; j++){
                for (int k = 1; k < kmax - 1; k++){
                    if (j == 0){
                        left = buffer[k];
                    } else {
                        left = my_phi[i][j-1][k];
                    }
                    my_phi[i][j][k] = (my_phi[i-1][j][k] + left
                        +my_phi[i][j][k-1] + my_phi[i][j][k+1]
                        +my_phi[i][j+1][k] + my_phi[i+1][j][k])/6.0;
                }
            }
        }
    }
    */
}

void allocate_array3D (int kmax, int jmax, int imax, double ****array, int verbose){

    double ***outer      =       (double***)    malloc(imax * sizeof(double**));          //  rows
    double **inner       =       (double**)     malloc(imax * jmax * sizeof(double*));       //  columns
    double *inner_inner  =       (double*)      malloc(imax * jmax * kmax * sizeof(double));    //  depth
    
    int i, j, k, col_pointer;
    if (!outer || !inner || !inner_inner) {
        printf("\nMemory allocation failed!");
    } else {
        printf("\nMemory assigned..");
    }

    for (int i = 0; i < imax; i++){
        col_pointer = jmax * i;
        outer[i] = &inner[col_pointer];
    }
    for (i = 0; i < jmax * imax; i++){
        inner[i] = &inner_inner[i * kmax];
    }
    for (i = 0; i < imax; i++) {
        for (j = 0; j < jmax; j++) {
            for (k = 0; k < kmax; k++) {
                outer[i][j][k] = k * (k+1) * (i+1) * (j+1);
            }
        }
    }
    printf("\nValues assigned..\n");
    if (verbose) {
        print_cube(kmax, jmax, imax, outer);
    }
    // saving pointer as outer cube layer
    *array = outer;
}


void print_cube(double kmax, double jmax, double imax, double ***arr){
    printf("\nPrinting cube below, slice by slice:");
    for (int k = 0; k < kmax; k++) {
        printf("\n-----------------------------------------------\nslice %d\n-----------------------------------------------", k);
        for (int i = 0; i < imax; i++) {
            printf("\n|");
            for (int j = 0; j < jmax; j++) {
                printf("\t%0.3f\t", arr[i][j][k]);
            }
            printf("\t|");
        }
    }
}

double parallel_euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2){
    double total_diff = 0.;

    #pragma omp parallel
    {
        double diff = 0.; 
        #pragma omp for
        for (int i = 0; i < imax; i++) {
            for (int j = 0; j < jmax; j++) {
                for (int k = 0; k < kmax; k++) {
                    diff += (arr1[i][j][k] - arr2[i][j][k]) * (arr1[i][j][k] - arr2[i][j][k]);
                }
            }
        }
        #pragma omp critical
        {
            total_diff += diff;
        }
    }
    return total_diff;
}

double euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2){
    double diff = 0.; 
    for (int i = 0; i < imax; i++) {
        for (int j = 0; j < jmax; j++) {
            for (int k = 0; k < kmax; k++) {
                diff += sqrt((arr1[i][j][k] - arr2[i][j][k]) * (arr1[i][j][k] - arr2[i][j][k]));
            }
        }
    }
    return diff;
}

void split_cube(int imax, int jmax, int kmax, double ***cube, double ***new_cube, int split_on_index, char split_on_dimension, int return_split){
    printf("\nSplitting cube on dimension ");
    int i, j, k;
    if (split_on_dimension == 'i'){
        printf("\nERROR: Not implemented yet");
    } else if (split_on_dimension == 'j') {
        if (return_split) {
            printf("j, with splitting index %d", split_on_index - 2);
            int j_start =  split_on_index - 2;
            printf(", retrieving right side of cube ..");
            // returning right hand side
            for (i = 0; i < imax; i++){
                for (j = j_start; j < jmax; j++){
                    for (k = 0; k < kmax; k++){
                        new_cube[i][j - j_start][k] = cube[i][j][k];
                    }
                }
            }

        } else { 
            printf("j, with splitting index %d", split_on_index);
            printf(", retrieving left side of cube ..");
            // returning left hand side
            for (i = 0; i < imax; i++){
                for (j = 0; j < split_on_index; j++){
                    for (k = 0; k < kmax; k++){
                        new_cube[i][j][k] = cube[i][j][k];
                    }
                }
            }
        }
    } else if (split_on_dimension == 'k'){
        printf("\nERROR: Not implemented yet");
    } else {
        printf("ERROR: Dimension not recognized, has to be i, j or k");
    }

}
