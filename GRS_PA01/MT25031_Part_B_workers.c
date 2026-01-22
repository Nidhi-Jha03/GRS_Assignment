/*
 * MT25031_Part_B_workers.c
 *
 * Implements CPU-intensive, memory-intensive, and I/O-intensive
 * workloads as reusable C worker functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "MT25031_Part_B_workers.h"

/* CPU-intensive worker */
#include <time.h>

void cpu(void) {
    volatile double result = 0.0;
    time_t start = time(NULL);

    while (time(NULL) - start < 5) {   // run ~5 seconds
        for (int i = 0; i < LOOP_COUNT; i++) {
            result += sqrt(i) * sqrt(i + 1);
        }
    }
}


/* Memory-intensive worker */
void mem(void) {
    int size = 1024 * 1024;   /* 1 million integers */
    int *array = malloc(size * sizeof(int));

    if (array == NULL) {
        perror("malloc failed");
        return;
    }

    for (int i = 0; i < LOOP_COUNT; i++) {
        for (int j = 0; j < size; j++) {
            array[j] = j + i;
        }
    }

    free(array);
}

/* I/O-intensive worker */
void io(void) {
    FILE *fp = fopen("io_test.txt", "w");
    if (!fp) return;

    volatile double x = 0;

    for (int i = 0; i < LOOP_COUNT; i++) {
        fprintf(fp, "Writing line %d\n", i);
        fflush(fp);

        for (int k = 0; k < 1000; k++) {  // tiny CPU burst
            x += sqrt(k);
        }

        usleep(1000);
    }
    fclose(fp);
}

/* Worker selector */
void (*get_worker(const char *name))(void) {
    if (strcmp(name, "cpu") == 0)
        return cpu;
    if (strcmp(name, "mem") == 0)
        return mem;
    if (strcmp(name, "io") == 0)
        return io;

    fprintf(stderr, "Invalid worker: %s\n", name);
    exit(1);
}
