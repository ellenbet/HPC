/*
main.c should contain a main program that accepts, on the command line, the filenames of
two web graph files (one small, one large), the damping constant d, the convergence threshold value ε, 
and value of n related to showing the top n webpages. This serial main program should first call read 
graph from file1 to read the small web graph file, followed by
PageRank iterations1 and top n webpages. Then, the main program should call read graph from file2
to read the large web graph file, followed by PageRank iterations2 and top n webpages.
Proper comments and output info (using printf) should be provided

*/

#include<stdio.h>
#include "include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[]){
    printf("\n\nNUMBER OF ARGUMENTS PROVIDED: %d", argc);
    for (int i = 0; i < argc; i++) {
        printf("\nArgument %d: %s", i, argv[i]);
    }
    if(argc != 6){
        perror("\nNumber of arguments provided does not match number of arguments needed: s_file, large_filename, d, eta, n");
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
    int *col_idx, *row_ptr;
    double *val;

    printf("\nPageRank algorithm starting for small file..\nconfirming parameters:\n\nsmall filname: %s\nlarge  filename: %s\ndampening constand d: %f\neta threshold: %f\nn webpages: %d\n", s_file, l_file, d, eta, n);
    // read graph from file here for small file 
    read_graph_from_file_1(s_file, &N, &hyperlink_mat, verbose);

    clock_t begin = clock();
    // pagerank iterations 1 here for small file
    double *scores = calloc(N, sizeof(double));
    PageRank_iterations_1(N, hyperlink_mat, d, eta, scores, verbose);

    // top n webpages here for small file with matrix system
    top_n_webpages(N, scores, n);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTotal time spent on hyperlink matrix algorithm: %f", time_spent);

    clock_t begin2 = clock();
    // crs
    read_graph_from_file_2(s_file, &N, &row_ptr, &col_idx, &val, verbose);
    // pagerank with crs for small file
    PageRank_iterations_2(N, row_ptr, col_idx, val, d, eta, scores, verbose);
    // top n webpages here for small file with crs system
    top_n_webpages(N, scores, n);

    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("\nTotal time spent on crs matrix algorithm: %f", time_spent2);

    // repeat above here for large file
    printf("\nPageRank algorithm starting for medium file...");
    double** hyperlink_mat2;
    // read graph from file here for small file 
    read_graph_from_file_1(l_file, &N, &hyperlink_mat2, verbose);

    int *col_idx2, *row_ptr2;
    double *val2;

    clock_t begin3 = clock();
    // pagerank iterations 1 here for small file
    double *scores2 = calloc(N, sizeof(double));
    PageRank_iterations_1(N, hyperlink_mat2, d, eta, scores2, verbose);

    // top n webpages here for small file with matrix system
    top_n_webpages(N, scores2, n);

    clock_t end3 = clock();
    double time_spent3 = (double)(end3 - begin3) / CLOCKS_PER_SEC;
    printf("\nTotal time spent on hyperlink matrix algorithm: %f", time_spent3);

    clock_t begin4 = clock();
    // crs
    read_graph_from_file_2(l_file, &N, &row_ptr2, &col_idx2, &val2, verbose);
    // pagerank with crs for small file
    PageRank_iterations_2(N, row_ptr2, col_idx2, val2, d, eta, scores2, verbose);
    // top n webpages here for small file with crs system
    top_n_webpages(N, scores2, n);

    clock_t end4 = clock();
    double time_spent4 = (double)(end4 - begin4) / CLOCKS_PER_SEC;
    printf("\nTotal time spent on crs matrix algorithm: %f", time_spent4);
}