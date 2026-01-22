# Makefile for PA01 - Processes and Threads
# Course: Graduate Systems (CSE638)
# Roll Number: MT25031

CC = gcc
CFLAGS = -Wall -O2
LIBS = -lm
PTHREAD = -pthread

# Default target
all: partA_process partA_thread

# ---------------- Program A: Process-based ----------------
partA_process: MT25031_Part_A_Program_A.c MT25031_Part_B_workers.c MT25031_Part_B_workers.h
	$(CC) $(CFLAGS) MT25031_Part_A_Program_A.c MT25031_Part_B_workers.c -o partA_process $(LIBS)

# ---------------- Program B: Thread-based ----------------
partA_thread: MT25031_Part_A_Program_B.c MT25031_Part_B_workers.c MT25031_Part_B_workers.h
	$(CC) $(CFLAGS) MT25031_Part_A_Program_B.c MT25031_Part_B_workers.c -o partA_thread $(PTHREAD) $(LIBS)

# ---------------- Clean ----------------
clean:
	rm -f partA_process partA_thread *.o
