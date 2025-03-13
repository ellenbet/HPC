/*
main.c should contain a main program that accepts, on the command line, the filenames of
two web graph files (one small, one large), the damping constant d, the convergence threshold value ε, 
and value of n related to showing the top n webpages. This serial main program should first call read 
graph from file1 to read the small web graph file, followed by
PageRank iterations1 and top n webpages. Then, the main program should call read graph from file2
to read the large web graph file, followed by PageRank iterations2 and top n webpages.
Proper comments and output info (using printf) should be provided

*/

int main(int argc, char *argv[]){
    if(argc != 5){
        perror("Number of arguments provided does not match number of arguments needed: small_filename, large_filename, d, eta, n");
        return 1;
    }

    // all command line inputs in order: small_filename, large_filename, d, eta, n
    char s_file[] = argv[0], l_file[] = argv[1];
    double d = atof(argv[2]), eta = atof(argv[3]);
    int n = atoi(argv[4]);

    printf("pagerank algorithm starting..\nconfirming parameters:\nsmall filname: %c\nlarge filename: %c\ndampening constand d: %f\neta threshold: %f\nn webpages: %d\n", s_file, l_file, d, eta, n);

    // read graph from file here for small file 

    // pagerank iterations 1 here for small file

    // top n webpages here for small file 

    // repeat above here for large file

}