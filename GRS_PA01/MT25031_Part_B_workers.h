/*
 * MT25031_Part_B_workers.h
 *
 * Declares reusable CPU, memory, and I/O intensive worker functions.
 */

#ifndef MT25031_PART_B_WORKERS_H
#define MT25031_PART_B_WORKERS_H

/* Last digit of roll number = 1 */
#define LAST_DIGIT 1
#define LOOP_COUNT (LAST_DIGIT * 1000)

/* Worker function declarations */
void cpu(void);
void mem(void);
void io(void);

/* Worker selector */
void (*get_worker(const char *name))(void);

#endif
