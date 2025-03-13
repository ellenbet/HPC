#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../include/utils.h"

float** create_2d_matrix(int m, int n){
    float *storage = (float*) calloc(m * n * sizeof(float));
    float **matrix = (float**) malloc(m * sizeof(float*));

    for (int i = 0; i < m; i++){
        matrix[i] = &(storage[i*n]);
    }
    return matrix;
}

void read_graph_from_file_1(char *filename, int *N, double ***hyperlink_matrix){
    /*
    This function should read a text file that contains a web graph, so that the number of webpages
    is read (into *N), and the corresponding hyperlink matrix is built up as a N × N 2D array
    
    Note: The reason for hyperlink matrix being a tripple pointer in the argument list is because
    this 2D array needs to be allocated inside the function

    */
   N = 0

    printf("Creating graph from small file...\n")

    FILE *file fopen(filename, "r");

    if (file == NULL) {
        perror("Failed to open file, file is NULL");
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)){
        N++;
    }

    printf("Number of webpages in small file: %d\n", N);
    rewind(file);

    // first: loop that counts how many "from's" a webpage has - which makes out it's element contribution
    // second: nested loop what uses hyperlink_matrix[to][from] = 1 / webpage_outs[from] to set the elements in the matri

}

void read_graph_from_file_2(char *filename, int *N, int **row_ptr, int **col_idx, double **val){
    /*
    This function should read a text file that contains a web graph, so that the
    corresponding hyperlink matrix is built up in the CRS format (see Section 3.6.1 of the textbook). 
    
    Note: The reason for row ptr, col idx and val being double pointers in the argument
    list is because these 1D arrays need to be allocated inside the function. Moreover, the length
    of row ptr should be N + 1, and the length of col idx and val should be the number of web
    links.

    */
}

void PageRank_iterations_1(int N, double **hyperlink_matrix, double d, double epsilon, double *scores){
    /*
    This function should implement the iterative procedure of the PageRank algorithm. 
    The input arguments include the N × N hyperlink matrix, the damping constant
    (d), and the convergence threshold value (epsilon). 
    
    The computed PageRank scores are to be
    contained in the pre-allocated 1D array scores.
    
    */
}

/*

The two functions below must be parallelized with OpenMP!

*/

void PageRank_iterations_2(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores){
    /*
    This function should implement the iterative procedure of the PageRank algorithm. 
    The difference from PageRank iterations1 is that the
    hyperlink matrix is now provided in the CRS format.
    
    */
}


void top_n_webpages(int N, double *scores, int n){
    /*
    This function should go through the computed PageRank score vector scores and print out the top n webpages, 
    with both their scores and webpage indices.

    */
}