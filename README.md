# Operating Systems Project 5  
**CS3113 – Introduction to Operating Systems – Spring 2025**

## Project Description  
This project simulates parallel summation of an integer array using a hierarchy of POSIX threads structured as a full binary tree. Each thread represents a node in the tree and is responsible for computing partial sums based on assigned chunks of the input array. Synchronization between threads is handled using mutexes, and a controlled top-down termination sequence is enforced after computation is complete.

The project provides practical experience with:
- Thread creation and synchronization
- Tree-based hierarchical computation
- Controlled and orderly thread termination
- Shared memory management in multithreaded programs

## Features  
- **Thread Tree Construction**: Threads are organized as nodes in a full binary tree.  
- **Chunk Assignment**: Input array elements are divided equally among leaf threads.  
- **Hierarchical Summation**: Internal threads wait for and sum the results of their children.  
- **Top-Down Termination**: Root thread initiates orderly termination after final computation.  
- **Logging**: Each thread prints its computation and termination details.

## Files Included  
- `CS3113_Project5.cpp`: Full source code implementing the thread-based summation.  
- `project5details.pdf`: Project specification document.  
- `README.md`: Documentation for project overview and usage instructions.

## Input Format  
The program expects input from standard input (e.g., redirected from a file) in the following format:
1. Integer: Tree height \( H \) (height of the binary tree)  
2. Integer: Array size \( M \) (number of integers in the array)  
3. \( M \) integers: The elements of the array

If the number of elements \( M \) is not divisible by the number of leaf threads \( N = 2^{H-1} \), the program pads the array with zeros.

## Compilation and Execution  
To compile and run the program:
```bash
g++ CS3113_Project5.cpp -o os_project5 -lpthread
./os_project5 < input.txt
