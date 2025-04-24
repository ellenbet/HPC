#ifndef __utils__
#define __utils__

void GS_iteration_normal (int kmax, int jmax, int imax, double ***phi);
void GS_iteration_2_chunks (int kmax, int jmax, int imax, double ***phi);
void GS_iteration_2_chunks_mpi (int my_rank, int kmax, int my_jmax, int imax, double ***my_phi);
void allocate_array3D (int kmax, int jmax, int imax, double ****array);
void print_cube(double kmax, double jmax, double imax, double ***arr);
double euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2);

#endif