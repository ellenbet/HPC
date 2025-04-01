**HPC Course IN4200**

**1) PageRank**
The PageRank-project contains the following structure: 
.
```bash
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
```


**Instructions:**
*Please note: the suggested terminal inputs will only work if you have the proper file setup as suggested above.*

To link, compile and run ```main.c``` file, run the following command in your terminal within the ```PageRank``` directory: 

- ```make run ARGS="data/small_example.txt data/medium_example.txt 0.85, 0.1, 5"```

To link, compile and run ```main_omp.c``` file, run the following command in your terminal within the ```PageRank``` directory: 

- ```make run MAIN_SRC=main_omp.c ARGS="data/medium_example.txt data/large_example.txt 0.85 0.1 5"```


