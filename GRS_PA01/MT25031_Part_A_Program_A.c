/*
 * MT25031_Part_A_Program_A.c
 *
 * Process-based implementation.
 *
 * Behavior:
 * 1. Part A (Demonstration):
 *    - Command: ./partA_process
 *    - Creates exactly 2 child processes
 *    - Prints process creation details on terminal
 *
 * 2. Part C / Part D (Evaluation):
 *    - Command: ./partA_process <cpu|mem|io> [num_processes]
 *    - Runs silently (no printf)
 *    - Number of processes scalable from 2 to 5 (Part D)
 *
 * Parent process is NOT counted as a worker.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "MT25031_Part_B_workers.h"

int main(int argc, char *argv[]) {

    char *worker = "mem";   /* default worker */
    int n = 2;              /* default = Part A */
    int demo = 0;           /* demo mode flag */

    /* ---------------- Part A Demo Mode ---------------- */
    if (argc == 1) {
        demo = 1;           /* enable demo output */
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
    if (n < 2 || n > 5) {
        fprintf(stderr,
                "Number of processes must be between 2 and 5\n");
        exit(1);
    }

    void (*work_func)(void) = get_worker(worker);

    /* ---------------- Process Creation ---------------- */
    if (demo) {
        printf("Parent PID = %d\n", getpid());
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            /* Child process */
            if (demo) {
                printf("Child process %d created, PID = %d\n",
                       i + 1, getpid());
            }
            work_func();
            exit(0);
        }
    }

    /* Parent waits for all children */
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}
