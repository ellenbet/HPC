#ifndef __utils__
#define __utils__
#include <stdbool.h>

void read_graph_from_file_1(char *filename, int *N, double ***hyperlink_matrix, bool verbose);
void read_graph_from_file_2(char *filename, int *N, int **row_ptr, int **col_idx, double **val, bool verbose);
void PageRank_iterations_1(int N, double **hyperlink_matrix, double d, double epsilon, double *scores, bool verbose);
double** create_2d_matrix(int m, int n);

// The two functions below must be parallelized
void PageRank_iterations_2(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores, bool verbose);
void top_n_webpages(int N, double *scores, int n);

#endif