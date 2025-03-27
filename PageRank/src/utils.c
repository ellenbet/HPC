#include <stdio.h>
#include <errno.h>
#include"../include/utils.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

double** create_2d_matrix(int m, int n){
    double *storage = (double*) calloc(m * n, sizeof(double));
    double **matrix = (double**) calloc(m, sizeof(double*));

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
    printf("\n\nCreating graph from small file...\n");

    FILE *file = fopen(filename, "r");
    int nodes;
    int *edges_out;

    if (file == NULL) {
        perror("\nFailed to open file, file is NULL");
    }

    char line[MAX_LINE_LENGTH];
    // first: loop that counts how many "from's" a webpage has - which makes out it's element contribution
    // Read data from file -> fromNode, toNode
    int from = 0, to = 0;

    while (fgets(line, sizeof(line), file)){
        if(line[0] == '#'){
            if(line[2] == 'N'){
            printf("\n%s", line);
            *N = atoi(&line[9]);
            edges_out = (int*) calloc(*N, sizeof(int));
            nodes = atoi(&line[19]);
            *hyperlink_matrix = create_2d_matrix(*N, *N);
        }} 

        else if (line[0] != '#') {
            sscanf(line, "%d %d", &from, &to);
            edges_out[from] += 1;
            printf("\nFrom node: %d to node: %d - node %d has a total of %d edges so far", from, to, from, edges_out[from]);
        }
    }

    rewind(file);
    // Skipping the first four lines
    fscanf(file, "%*[^\n]\n");
    fscanf(file, "%*[^\n]\n");
    fscanf(file, "%*[^\n]\n");
    fscanf(file, "%*[^\n]\n");
 
    while (fscanf(file, "%d %d", &from, &to) == 2){
        if (edges_out[from] != 0){
            (*hyperlink_matrix)[to][from] = 1./edges_out[from];
        } else {
            (*hyperlink_matrix)[to][from] = 0.;
        }
    }

    printf("\n\nResulting hyperlink matrix:\n");
    for (int i = 0; i < *N; i++){
        printf("\n %d:   " , i);
        for (int j = 0; j < *N; j++){
            printf("  %f  " , (*hyperlink_matrix)[i][j]);
        }
    }

    free(edges_out);
    fclose(file);  // Close the file

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

    printf("\n\nCreating CRS graph...\n");

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("\nFailed to open file, file is NULL");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int *edges_out;
    int nodes = 0;
    int from, to;
    int total_edges = 0;


    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') {
            if (line[2] == 'N') {
                *N = atoi(&line[9]);
                printf("Number of nodes: %d\n", *N);
                edges_out = (int*) calloc(*N, sizeof(int)); 
            }
        } else if (line[0] != '#') {
            sscanf(line, "%d %d", &from, &to);
            edges_out[from] += 1;  
            total_edges += from;
        }
    }

    *row_ptr = (int*) malloc((*N + 1) * sizeof(int)); 
    *col_idx = (int*) malloc(total_edges * sizeof(int)); 
    *val = (double*) malloc(total_edges * sizeof(double));

    (*row_ptr)[0] = 0;

    int current_edge = 0;
    rewind(file);
    fscanf(file, "%*[^\n]\n");  // Skip first line
    fscanf(file, "%*[^\n]\n");  // Skip second line
    fscanf(file, "%*[^\n]\n");  // Skip third line
    fscanf(file, "%*[^\n]\n");  // Skip fourth line

    while (fscanf(file, "%d %d", &from, &to) == 2) {
        // Fill col_idx and val for each non-zero element
        (*col_idx)[current_edge] = to;
        (*val)[current_edge] = 1.0 / edges_out[from];  // Assuming it's the same as in your hyperlink matrix logic
        current_edge++;
    }

    for (int i = 1; i <= *N; i++) {
        (*row_ptr)[i] = (*row_ptr)[i - 1] + edges_out[i - 1];
    }

    free(edges_out);

    printf("\n\nResulting CRS format:\n");
    for (int i = 0; i < *N + 1; i++) {
        printf("row_ptr[%d] = %d\n", i, (*row_ptr)[i]);
    }

    printf("\ncol_idx: ");
    for (int i = 0; i < total_edges; i++) {
        printf("%d ", (*col_idx)[i]);
    }

    printf("\nval: ");
    for (int i = 0; i < total_edges; i++) {
        printf("%f ", (*val)[i]);
    }
    printf("\n");

    fclose(file); 
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