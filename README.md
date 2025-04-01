**HPC Course IN4200**

**1) PageRank**
The PageRank-project contains the following structure: 
.
├── main.c                                  # main program with small and medium examples
├── main_omp.c                              # main program with large example
│
├── main.c 
│
├── Makefile                                # Build instructions, handles compilation, linking, and cleaning
│
├── bin/                                    # Output directory for compiled binaries and object files
│   └── (compiled binaries and object files go here, e.g., main.o, main executable)
│
├── src/                                    # Source files for the project's implementation
│   └── utils.c                             # utility functions
│                       
├── include/                                # Header files 
│   └── utils.h                             # Header file corresponding to utils.cpp            
│
└── data/                                   # In .gitignore
    ├── small_example.txt
    ├── medium_example.txt
    └── large_example.txt
