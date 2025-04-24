#include "./include/utils.h"
#include<stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int nargs, char **args){
    // setting up initial parameters
    int n, k, j, i, kmax, jmax, imax, num_iters;
    kmax = atoi(args[1]);
    jmax = atoi(args[2]);
    imax = atoi(args[3]);
    num_iters = atoi(args[4]);
    double ***arr1, ***arr2;
    int verbose = 0;

    // allocating arrays with random values
    printf("\nSetting up 3D array nr 1: ");
    allocate_array3D(kmax, jmax, imax, &arr1, verbose);
    printf("\nSetting up 3D array nr 2: ");
    allocate_array3D(kmax, jmax, imax, &arr2, verbose);

    // running the gs algorithm 
    printf("\nRunning Gauss-Streidel algorithms...");
    for (n = 0; n < num_iters; n++) {
        GS_iteration_normal(kmax, jmax, imax, arr1);
        GS_iteration_2_chunks(kmax, jmax, imax, arr2);
    }

    // checking results
    //printf("\n\nConfirming new cube:");
    //print_cube(kmax, jmax, imax, arr1);
    printf("\nEuclidian distance between cubes: %0.6f", euclidean_distance(kmax, jmax, imax, arr1, arr2));
    return 0;
}