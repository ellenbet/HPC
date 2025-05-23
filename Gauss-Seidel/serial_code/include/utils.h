#ifndef __utils__
#define __utils__

void GS_iteration_normal (int kmax, int jmax, int imax, double ***phi);
void GS_iteration_2_chunks (int kmax, int jmax, int imax, double ***phi);
void allocate_array3D (int kmax, int jmax, int imax, double ****array, int verbose);
void print_cube(double kmax, double jmax, double imax, double ***arr);
double euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2);
double parallel_euclidean_distance (int kmax, int jmax, int imax, double ***arr1, double ***arr2);

#endif