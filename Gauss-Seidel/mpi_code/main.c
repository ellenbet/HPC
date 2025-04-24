int main (int nargs, char **args){
    double ***arr1, ***arr2;
    int n,k,j,i, num_iters, kmax, jmax, imax;
    // read from command line the values of num_iters, kmax, jmax, and imax


    
    // ...

    allocate_array3D(kmax, jmax, imax, &arr1);
    allocate_array3D(kmax, jmax, imax, &arr2);

    // assign same initial values to arr1 & arr2 (don’t give a constant to all entries of arr1 & arr2)
    // ....

    for (n = 0; n < num_iters; n++) {
        GS_iteration_normal(kmax, jmax, imax, arr1);
        GS_iteration_2_chunks(kmax, jmax, imax, arr2);
    }

    printf("num iters = %d, kmax = %d, jmax = %d, imax = %d, diff = %g\n",
    num_iters, kmax, jmax, imax, euclidean_distance(kmax, jmax, imax, arr1, arr2));
    return 0;
}