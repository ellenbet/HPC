#include "./include/utils.h"
#include<stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int nargs, char **args){
    // run with make run ARGS="1 2 3 4" to check (NB main has to be named main.c)

    /*
    TODO
    printf("num iters = %d, kmax = %d, jmax = %d, imax = %d, diff = %g\n",
    num_iters, kmax, jmax, imax, euclidean_distance(kmax, jmax, imax, arr1, arr2));
    return 0;
    */

    // setting up initial parameters
    int n, k, j, i, kmax, jmax, imax, num_iters;
    kmax = atoi(args[1]);
    jmax = atoi(args[2]);
    imax = atoi(args[3]);
    num_iters = atoi(args[4]);
    double ***arr1, ***arr2;

    // allocating arrays with random values
    printf("\n\nSetting up 3D arrays: ");
    allocate_array3D(kmax, jmax, imax, &arr1);
    allocate_array3D(kmax, jmax, imax, &arr2);

    // running the gs algorithm 
    printf("\n\nRunning Gauss-Streidel algorithms:");
    for (n = 0; n < num_iters; n++) {
        GS_iteration_normal(kmax, jmax, imax, arr1);
        GS_iteration_normal(kmax, jmax, imax, arr2);
        //GS_iteration_2_chunks(kmax, jmax, imax, arr2);
    }

    // checking results
    printf("\n\nConfirming new cube:");
    print_cube(kmax, jmax, imax, arr1);
    printf("\n\ndistance: %f", euclidean_distance(kmax, jmax, imax, arr1, arr2));
}