# Operating Systems Project 5  
**CS3113 – Introduction to Operating Systems – Spring 2025**

## Project Description  
This project simulates parallel array summation using a structured hierarchy of POSIX threads organized as a full binary tree. Each thread represents a node in the tree and is responsible for computing partial sums from an input array. The project emphasizes thread creation, synchronization using mutexes, and controlled top-down termination.

Processes are coordinated to ensure orderly computation and cleanup through the use of mutexes and thread joining, providing practical exposure to multithreaded programming concepts.

## Key Features  
- **Thread Hierarchy**: Threads are arranged in a full binary tree structure based on user-specified height.  
- **Chunk Division**: The input array is divided into equal-sized chunks assigned to leaf threads.  
- **Synchronization**: Parent threads wait for child threads to finish before computing their sum.  
- **Controlled Termination**: Tree cleanup and thread termination follow a top-down order after computation completion.  
- **Structured Output**: Each thread logs its computation and termination details, including its index, level, position, and thread ID.

## Files Included  
- `CS3113_Project5.cpp`: Full C++ implementation of thread-based array summation using a binary tree.  
- `project5details.pdf`: Official project specification detailing computation and threading structure.  
- `README.md`: Documentation for project overview and usage instructions.

## Input Format  
The program reads from standard input (via redirection). The input must include:
1. An integer specifying the height of the binary tree (`H`)  
2. An integer specifying the number of elements in the array (`M`)  
3. `M` integers representing the elements of the array

If the number of input elements is not divisible by the number of leaf threads, the program pads the array with zeros to achieve divisibility.

## Computation and Thread Structure  
- **Number of Leaf Threads**: \( N = 2^{H-1} \)  
- **Total Threads**: \( 2^H - 1 \)  
- **Thread Tree Behavior**:
  - **Leaf Threads**: Compute the sum of their assigned chunk.
  - **Internal Threads**: Wait for left and right child results, then compute the combined sum.
  - **Root Thread**: Computes the final sum of the array and initiates thread cleanup.

## Output Behavior  
During execution:
- Each thread prints its computation results.
- Each internal thread prints the values received from its child threads.
- After computation, each thread prints a termination message.

### Example Output Excerpts  
