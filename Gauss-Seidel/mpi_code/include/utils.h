#ifndef __utils__
#define __utils__

void GS_iteration_normal (int kmax, int jmax, int imax, double ***phi);
void GS_iteration_2_chunks (int kmax, int jmax, int imax, double ***phi);
void GS_iteration_2_chunks_mpi (int my_rank, int kmax, int my_jmax, int imax, double ***my_phi);
void allocate_array3D (int kmax, int jmax, int imax, double ****array, int verbose);
void print_cube(double kmax, double jmax, double imax, double ***arr);
void split_cube(int imax, int jmax, int kmax, double ***cube, double ***new_cube, int split_on_index, char split_on_dimension, int return_split);
double euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2);
double parallel_euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2);

#endif