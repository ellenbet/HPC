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


int main(int argc, char *argv[]){
    printf("\n\nNUMBER OF ARGUMENTS PROVIDED:\n argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    if(argc != 6){
        perror("Number of arguments provided does not match number of arguments needed: small_filename, large_filename, d, eta, n");
        return 1;
    }

    // all command line inputs in order: small_filename, large_filename, d, eta, n
    char *s_file = argv[1];
    char *l_file = argv[2];
    float d = atof(argv[3]);
    float eta = atof(argv[4]);
    int n = atoi(argv[5]);

    printf("\nPageRank algorithm starting..\nconfirming parameters:\n\nsmall filname: %s\nlarge  filename: %s\ndampening constand d: %f\neta threshold: %f\nn webpages: %d\n", s_file, l_file, d, eta, n);

    double** hyperlink_mat;
    
    int N = 0;
    char *small_filename = "data/small_example.txt";
    // read graph from file here for small file 
    read_graph_from_file_1(small_filename, &N, &hyperlink_mat);

    int *col_idx, *row_ptr;
    double *val;
    read_graph_from_file_2(small_filename, &N, &row_ptr, &col_idx, &val);
    // pagerank iterations 1 here for small file
    double *scores = calloc(N, sizeof(double));
    PageRank_iterations_1(N, hyperlink_mat, d, eta, scores);

    // top n webpages here for small file 
    top_n_webpages(N, scores, 3);

    // repeat above here for large file

}