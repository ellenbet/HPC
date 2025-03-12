/*
main omp.c should contain a main program that accepts, on the command line, the filename
of one web graph file (of large size), the damping constant d, the convergence threshold value
ε, and value of n related to showing the top n webpages. This OpenMP parallelized main
program should call read graph from file2 (serial version) and then the OpenMP version of
5 PageRank iterations2, followed by the OpenMP version of top n webpages (only required
for IN4200 students) or the serial version of top n webpages.

*/