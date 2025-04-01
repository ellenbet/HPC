/*
main omp.c should contain a main program that accepts, on the command line, the filename
of one web graph file (of large size), the damping constant d, the convergence threshold value
ε, and value of n related to showing the top n webpages. This OpenMP parallelized main
program should call read graph from file2 (serial version) and then the OpenMP version of
5 PageRank iterations2, followed by the OpenMP version of top n webpages (only required
for IN4200 students) or the serial version of top n webpages.

*/

#include<stdio.h>
#include "include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[]){
    printf("\n\nNUMBER OF ARGUMENTS PROVIDED:\n argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    if(argc != 6){
        perror("Number of arguments provided does not match number of arguments needed: s_file, large_filename, d, eta, n");
        return 1;
    }

    // all command line inputs in order: s_file, large_filename, d, eta, n
    char *s_file = argv[1];
    char *l_file = argv[2];
    float d = atof(argv[3]);
    float eta = atof(argv[4]);
    int n = atoi(argv[5]);
    double** hyperlink_mat;
    bool verbose = false;
    int N = 0;
    int *col_idx, *row_ptr; //, *col_idx2, *row_ptr2;
    double *scores; //, *scores2;
    double *val; //, *val2;

    printf("\nStarting CRS PageRank on large file...");
    clock_t begin = clock();
    // crs
    read_graph_from_file_2(l_file, &N, &row_ptr, &col_idx, &val, verbose);
    // pagerank with crs for small file

    scores = calloc(N, sizeof(double));
    PageRank_iterations_2(N, row_ptr, col_idx, val, d, eta, scores, verbose);
    // top n webpages here for small file with crs system
    top_n_webpages(N, scores, n);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTotal time spent on large file with crs algorithm: %f", time_spent);


    /*
    clock_t begin2 = clock();
    // crs
    read_graph_from_file_2(l_file, &N, &row_ptr2, &col_idx2, &val2, verbose);
    // pagerank with crs for small file
    PageRank_iterations_2(N, row_ptr2, col_idx2, val2, d, eta, scores2, verbose);
    // top n webpages here for small file with crs system
    top_n_webpages(N, scores, n);

    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("\nTotal time spent on large file with crs algorithm: %f", time_spent2);
    */
}