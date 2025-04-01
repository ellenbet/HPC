#include <stdio.h>
#include <errno.h>
#include"../include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define MAX_LINE_LENGTH 1024

double** create_2d_matrix(int m, int n){
    double *storage = (double*) calloc(m * n, sizeof(double));
    double **matrix = (double**) calloc(m, sizeof(double*));

    for (int i = 0; i < m; i++){
        matrix[i] = &(storage[i*n]);
    }
    return matrix;
}

void read_graph_from_file_1(char *filename, int *N, double ***hyperlink_matrix, bool verbose){
    /*
    This function should read a text file that contains a web graph, so that the number of webpages
    is read (into *N), and the corresponding hyperlink matrix is built up as a N × N 2D array
    
    Note: The reason for hyperlink matrix being a tripple pointer in the argument list is because
    this 2D array needs to be allocated inside the function

    */
    printf("\n\nCreating graph from small file...\n");

    FILE *file = fopen(filename, "r");
    int *edges_out;

    if (file == NULL) {
        perror("\nFailed to open file, file is NULL");
    }

    char line[MAX_LINE_LENGTH];
    // first: loop that counts how many "from's" a webpage has - which makes out it's element contribution
    int from = 0, to = 0;

    while (fgets(line, sizeof(line), file)){
        if(line[0] == '#'){
            if(line[2] == 'N'){
            printf("\n%s", line);
            *N = atoi(&line[9]);
            edges_out = (int*) calloc(*N, sizeof(int));
            *hyperlink_matrix = create_2d_matrix(*N, *N);
        }} 

        else if (line[0] != '#') {
            sscanf(line, "%d %d", &from, &to);
            if (to != from && to < *N && from < *N && to > -1 && from > -1){
                edges_out[from] += 1;
            } else {
                printf("\n\nExcluded edge from %d to %d\n", from, to);
            }
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

    if (verbose){
        printf("\n\nResulting hyperlink matrix:\n");
        for (int i = 0; i < *N; i++){
            printf("\n %d:   " , i);
            for (int j = 0; j < *N; j++){
                printf("  %f  " , (*hyperlink_matrix)[i][j]);
            }
        }
    }  

    free(edges_out);
    fclose(file);

}

void read_graph_from_file_2(char *filename, int *N, int **row_ptr, int **col_idx, double **val, bool verbose){
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
    int *edges_out, *edges_in;
    int from, to;
    int total_edges = 0;


    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') {
            if (line[2] == 'N') {
                *N = atoi(&line[9]);
                total_edges = 0;
                printf("\ntotal nodes: %d", *N);
                edges_out = (int*) calloc(*N, sizeof(int)); 
                edges_in = (int*) calloc(*N, sizeof(int)); 
            }
        } else if (line[0] != '#') {
            sscanf(line, "%d %d", &from, &to);
            if (to != from && to < *N && from < *N && to > -1 && from > -1){
                edges_out[from] += 1; 
                edges_in[to] += 1;
                total_edges += 1;
            } else {
                printf("\n\nExcluded edge from %d to %d\n", from, to);
            }
        } 
    }
    printf("\ntotal edges: %d", total_edges);

    *row_ptr = (int*) malloc((*N + 1) * sizeof(int)); 
    *col_idx = (int*) malloc(total_edges * sizeof(int)); 
    *val = (double*) malloc(total_edges * sizeof(double));
 
    (*row_ptr)[0] = 0;
    rewind(file);

    for (int i = 1; i < *N + 1; i++) {
        (*row_ptr)[i] = (*row_ptr)[i - 1] + edges_in[i - 1];
    }

    int *current_pos = (int*) calloc(*N, sizeof(int));
    memcpy(current_pos, *row_ptr, *N * sizeof(int));

    rewind(file);
    fscanf(file, "%*[^\n]\n");
    fscanf(file, "%*[^\n]\n");  
    fscanf(file, "%*[^\n]\n");  
    fscanf(file, "%*[^\n]\n");  

    while (fscanf(file, "%d %d", &from, &to) == 2) {
        int index = current_pos[to];
        (*col_idx)[index] = from;
        if (edges_out[from] != 0){
            (*val)[index] = 1.0 / edges_out[from];
        } else {
            printf("\n\nDangling webpage with id: %d", from);
        }
        current_pos[to]++; 
    }

    free(edges_out);
    free(edges_in);

    if(verbose){
        printf("\n\nResulting CRS format:\n");
        for (int i = 0; i < *N + 1; i++) {
            printf("row_ptr[%d] = %d\n", i, (*row_ptr)[i]);
        }

        printf("\n\ncol_idx\tval");
        for (int i = 0; i < total_edges; i++) {
            printf("\n%d\t%f", (*col_idx)[i], (*val)[i]);
        }
        printf("\n");
    }

    fclose(file); 
}


void PageRank_iterations_1(int N, double **hyperlink_matrix, double d, double epsilon, double *scores, bool verbose){
    /*
    This function should implement the iterative procedure of the PageRank algorithm. 
    The input arguments include the N × N hyperlink matrix, the damping constant
    (d), and the convergence threshold value (epsilon). 
    
    The computed PageRank scores are to be
    contained in the pre-allocated 1D array scores.
    */
   printf("\n\nStarting PageRank algorithm for matrix format...");

   double N_inv = 1./N; //  N is the number of pages 
   double one_minus_d = (1 - d);
   double temp_xi = N_inv, temp_term = one_minus_d * N_inv, term;
   double score_delta = 1.;
   double max_score_delta = 0.5;
   double *new_scores = calloc(N, sizeof(double));

   for (int i = 0; i < N; i++){
    scores[i] = N_inv;
    }

    // assuming there are no dangling pages and W is zero
    while (max_score_delta > epsilon){
        max_score_delta = 0;

        for (int i = 0; i < N; i++){
            temp_xi =  scores[i];
            term = 0;

            for (int j = 0; j < N; j++){
                term +=  scores[j] * hyperlink_matrix[i][j];
            }

            new_scores[i] = d * term + temp_term;
            score_delta = fabs(temp_xi - new_scores[i]);
            //printf("\nx_i: %f", scores[i]);
            //printf("\nprevious x_i: %f", temp_xi);
            if (score_delta > max_score_delta){
                max_score_delta = score_delta;
            }
        }
        printf("\nmax delta score: %f", max_score_delta);
    }

    if (verbose){
        printf("\n\nPageRank algorithm for matrix is finished!\nscore delta is %f, deemed less than eta threshold: %f\n\nResulting scores:", score_delta, epsilon);
        for (int i = 0; i < N; i++){
            scores[i] = new_scores[i];
            printf("\nWebpage: %d with score: %f", i, scores[i]);
        }
    } else {
        for (int i = 0; i < N; i++){
            scores[i] = new_scores[i];
        } 
    }
}


void PageRank_iterations_2(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores, bool verbose){
    /*
    This function should implement the iterative procedure of the PageRank algorithm. 
    The difference from PageRank iterations1 is that the
    hyperlink matrix is now provided in the CRS format.
    
    */
   printf("\n\nStarting parallelized PageRank algorithm for CRS format...");
   double N_inv = 1. / N;  
   double one_minus_d = (1. - d);
   double temp_xi, temp_term;
   double score_delta = 2.;
   double max_score_delta = 1.;
   double *new_scores = calloc(N, sizeof(double));
   int threads = 5; //default 8 threads, no need to change this
   omp_set_dynamic(0); 
   omp_set_num_threads(threads); 

   for (int i = 0;  i < N; i++){
       scores[i] = N_inv;
   }
   printf("\nAvailable threads: %d\n", omp_get_max_threads());

   while (max_score_delta > epsilon){
       max_score_delta = 0;

       for (int i = 0; i < N; i++){
           temp_xi = scores[i];
           temp_term = one_minus_d * N_inv;

           #pragma omp parallel for
           for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++){
               new_scores[i] += scores[col_idx[j]] * val[j];
           }
           new_scores[i] = d * new_scores[i] + temp_term;
           score_delta = fabs(temp_xi - new_scores[i]);

           #pragma omp critical
           if (score_delta > max_score_delta){
            max_score_delta = score_delta;
        }
       }

       printf("\nmax delta score: %f", max_score_delta);

   }

   if (verbose){
        printf("\n\nPageRank algorithm with CRS finished!\nscore delta is %f, deemed less than eta threshold: %f\n\nResulting scores:", score_delta, epsilon);
        for (int i = 0; i < N; i++){
                scores[i] = new_scores[i];
                printf("\nWebpage: %d with score: %f", i, scores[i]);
        }
    } else {
        for (int i = 0; i < N; i++){
            scores[i] = new_scores[i];
        }
    }
}


int comp (void *scores, const void * elem1, const void * elem2){
    // found here: https://stackoverflow.com/questions/1787996/c-library-function-to-perform-sort
    double *score_array = (double *) scores;
    int index1 = *(int *)elem1;
    int index2 = *(int *)elem2;

    if (score_array[index1] < score_array[index2]) return 1;  
    if (score_array[index1] > score_array[index2]) return -1;
    return 0;
}

void top_n_webpages(int N, double *scores, int n) {
    int *indices = malloc(N * sizeof(int));
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        indices[i] = i;
    }

    // qsort_r will only work with gnu compiler! 
    qsort_r(indices, N, sizeof(int), scores, comp);

    printf("\n\nTop %d webpages based on PageRank scores:\n", n);
    for (int i = 0; i < n; i++) {
        #pragma omp ordered
        printf("Ranked %d: Webpage %d with score: %.6f\n", i + 1, indices[i], scores[indices[i]]);
    }

    free(indices);
}