#ifndef FCFS_H
#define FCFS_H

#include "queue.h"
#include "process.h"

//======================================================
// Scheduler Struct
//======================================================
typedef struct fcfs_scheduler {
    queue_t* ready_queue;
    process_t* current_process;
    unsigned long current_time;
} fcfs_scheduler_t;

//======================================================
// Create/Destroy Scheduler
//======================================================
fcfs_scheduler_t* fcfs_create(void);
void fcfs_destroy(fcfs_scheduler_t* scheduler);

//======================================================
// Core Functions
//======================================================
void fcfs_add_process(fcfs_scheduler_t* scheduler, process_t* process);
process_t* fcfs_get_next_process(fcfs_scheduler_t* scheduler);
void fcfs_run_schedule(fcfs_scheduler_t* scheduler);

#endif