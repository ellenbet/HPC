#include "./include/utils.h"
#include<stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int nargs, char **args){
    // setting up initial parameters
    int n, kmax, jmax, imax, num_iters;
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

    printf("\narr1 address: %p", (void*)arr1);
    printf("\narr2 address: %p", (void*)arr2);


    if(0 == euclidean_distance(kmax, jmax, imax, arr1, arr2)){
        printf("\nCube distance prior to GS is zero");
        // running the gs algorithm 
        printf("\nRunning Gauss-Seidel algorithms...");
        for (n = 0; n < num_iters; n++) {
            //GS_iteration_normal(kmax, jmax, imax, arr1);
            GS_iteration_2_chunks(kmax, jmax, imax, arr2);
        }

        printf("\nParallel function distance between cubes after GS: %0.6f", parallel_euclidean_distance(kmax, jmax, imax, arr1, arr2)); //possible to just use euclidean_distance instead 
        printf("\nDistance after GS: %f", euclidean_distance(kmax, jmax, imax, arr1, arr2)); 
        return 0;
    } else {
        printf("ERROR: the two cubes to be compared have different values prior to algorithm changes");
        return 1;
    }
}