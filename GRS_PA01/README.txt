PA01 – Processes and Threads

Course: Graduate Systems (CSE638)
Semester: Winter 2026
Assignment: PA01 – Processes and Threads
Instructor: Dr. Rinku Shah
Roll Number: MT25031

1. Project Overview

This assignment studies the behavior and performance of process-based and thread-based parallel execution under different workload types. Three worker functions—CPU-intensive, memory-intensive, and I/O-intensive—are executed using both processes and threads, and their performance is analyzed using standard Linux system monitoring tools.

The assignment is divided into four parts:

Part A: Demonstration of process and thread creation

Part B: Implementation of reusable worker functions

Part C: Automated performance measurement

Part D: Scalability analysis by increasing parallelism

2. Files Included
Source Code

MT25031_Part_A_Program_A.c — Process-based implementation using fork()

MT25031_Part_A_Program_B.c — Thread-based implementation using POSIX threads

MT25031_Part_B_workers.h — Worker function declarations

MT25031_Part_B_workers.c — Worker function implementations

Automation Scripts

MT25031_Part_C_shell.sh — Automates Part C measurements

MT25031_Part_D_shell.sh — Automates Part D scalability experiments

Results

MT25031_Part_C_CSV.csv

MT25031_Part_D_CSV.csv

Build and Documentation

Makefile

README.md

MT25031_Report.pdf

Note: No binaries or executables are included, as required by the assignment.

3. Part A – Process and Thread Demonstration
Program A: Process-Based Execution

Uses fork() to create worker processes

Default behavior creates 2 child processes

Parent process is not counted as a worker

Demonstration mode:

./partA_process

Program B: Thread-Based Execution

Uses POSIX threads (pthread_create)

Default behavior creates 2 worker threads

Main thread is not counted as a worker

Demonstration mode:

./partA_thread

4. Part B – Worker Functions

Three reusable worker functions are implemented and shared by both programs:

cpu — Performs CPU-intensive floating-point computations

mem — Allocates and repeatedly accesses a large heap array

io — Performs repeated disk writes using fprintf() and fflush()

The loop count is determined by:

(last digit of roll number) × 10³


For MT25031, the loop count is 1000 iterations.

Worker selection is handled using a function-pointer based mechanism (get_worker()), enabling modular reuse across processes and threads.

5. Compilation Instructions
Manual Compilation

Process-based program:

gcc MT25031_Part_A_Program_A.c MT25031_Part_B_workers.c -o partA_process -lm


Thread-based program:

gcc MT25031_Part_A_Program_B.c MT25031_Part_B_workers.c -o partA_thread -pthread -lm

Using Makefile (Recommended)
make


Clean compiled binaries:

make clean

6. System Monitoring Tools Used
CPU Pinning (taskset)

All executions are pinned to CPU core 0 to ensure fair comparison:

taskset -c 0 ./partA_process cpu
taskset -c 0 ./partA_thread mem

CPU and Memory Monitoring (top)

top is used to record CPU utilization (%CPU) and resident memory usage (RES).

Sampling is performed periodically while the program is running.

Example:

top

Disk I/O Monitoring (iostat)

iostat is used to observe disk activity during program execution.

Example:

iostat -dx 1

Execution Time Measurement (time)

Execution time is measured using /usr/bin/time.

Example:

/usr/bin/time ./partA_process cpu

7. Part C – Automated Performance Measurement

Part C evaluates all six combinations:

A+cpu, A+mem, A+io

B+cpu, B+mem, B+io

The automation script:

Pins programs to CPU 0

Samples top for CPU and memory

Records disk activity using iostat

Measures execution time using time

Run Part C
chmod +x MT25031_Part_C_shell.sh
./MT25031_Part_C_shell.sh


Output:

MT25031_Part_C_CSV.csv

8. Part D – Scalability Analysis

Part D extends Part C by increasing parallelism:

Processes: 2–5

Threads: 2–8

Workers: cpu, mem, io

Run Part D
chmod +x MT25031_Part_D_shell.sh
./MT25031_Part_D_shell.sh


Output:

MT25031_Part_D_CSV.csv


These results are used to generate scaling graphs.

9. Submission Notes

No executables or binary files are included.

All file naming conventions are followed.

Screenshots, plots, analysis, AI declaration, and GitHub URL are included in the report.

Submission consists of a single ZIP file with no subfolders.