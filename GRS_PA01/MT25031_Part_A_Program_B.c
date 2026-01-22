/*
 * MT25031_Part_A_Program_B.c
 *
 * Thread-based implementation.
 *
 * Behavior:
 * 1. Part A (Demonstration):
 *    - Command: ./partA_thread
 *    - Creates exactly 2 threads
 *    - Prints thread creation details on terminal
 *
 * 2. Part C / Part D (Evaluation):
 *    - Command: ./partA_thread <cpu|mem|io> [num_threads]
 *    - Runs silently (no printf)
 *    - Number of threads scalable from 2 to 8 (Part D)
 *
 * Main thread is NOT counted as a worker.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "MT25031_Part_B_workers.h"

/* Thread wrapper to execute worker function */
void *thread_runner(void *arg) {
    void (*work_func)(void) = arg;
    work_func();
    return NULL;
}

int main(int argc, char *argv[]) {

    char *worker = "mem";   /* default worker */
    int n = 2;              /* default = Part A */
    int demo = 0;           /* demo mode flag */

    /* ---------------- Part A Demo Mode ---------------- */
    if (argc == 1) {
        demo = 1;
    }

    /* ---------------- Argument Parsing ---------------- */
    if (argc >= 2) {
        if (strcmp(argv[1], "cpu") == 0 ||
            strcmp(argv[1], "mem") == 0 ||
            strcmp(argv[1], "io")  == 0) {
            worker = argv[1];
        }
    }

    if (argc >= 3) {
        n = atoi(argv[2]);
    }

    /* Enforce Part D limits */
    if (n < 2 || n > 8) {
        fprintf(stderr,
                "Number of threads must be between 2 and 8\n");
        exit(1);
    }

    pthread_t threads[n];
    void (*work_func)(void) = get_worker(worker);

    /* ---------------- Thread Creation ---------------- */
    if (demo) {
        printf("Main thread PID = %d\n", getpid());
    }

    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL,
                       thread_runner, work_func);

        if (demo) {
            printf("Thread %d created, PID = %d\n", i + 1, getpid());
        }
    }

    /* Wait for all threads */
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
