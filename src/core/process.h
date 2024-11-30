#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rbtree.h"

//======================================================
// State Enum
//======================================================
typedef enum process_state {
    PROCESS_NEW,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_WAITING,
    PROCESS_COMPLETED
} process_state_t;

//======================================================
// Process Structure
//======================================================
typedef struct process {

    // basic for all processes
    int pid;
    int priority;

    // timing info
    unsigned long arrival_time;
    unsigned long first_run_time;
    unsigned long completion_time;
    unsigned long total_time;
    unsigned long burst_time;
    unsigned int io_frequency;

    // cfs
    double vruntime;
    struct rb_node rb_node;

    // state stuff
    process_state_t state;

    // queue management
    struct process *next;
    struct process *prev;
} process_t;

//======================================================
// Process Stuff
//======================================================
process_t *process_create(int pid, int arrival_time, int priority, unsigned long burst_time, unsigned int io_frequency);
void destroy_process(process_t *process);

//======================================================
// State Management
//======================================================
void process_set_state(process_t *process, process_state_t new_state);
bool process_is_finished(const process_t *process);

//======================================================
// Runtime Management
//======================================================
void process_update_runtime(process_t *process, process_state_t new_state);
void process_handle_io(process_t *process);

//======================================================
// Metric Helpers
//======================================================
double process_get_response_time(const process_t *process);
double process_get_turnaround_time(const process_t *process);
double process_get_waiting_time(const process_t *process);

#endif