#include <stdlib.h>
#include "process.h"

//======================================================
// Process Stuff
//======================================================
process_t *process_create(int pid, int priority, unsigned long burst_time, unsigned int io_frequency) {
    process_t *process = malloc(sizeof(process_t));
    if (!process) return NULL;

    // basic
    process->pid = pid;
    process->priority = priority;

    // timing
    process->arrival_time = 0;
    process->first_run_time = 0;
    process->completion_time = 0;
    process->total_time = 0;
    process->burst_time = burst_time;
    process->io_frequency = io_frequency;



    // cfs
    process->vruntime = 0;

    // initial state
    process->state = PROCESS_NEW;

    // queue management
    process->next = NULL;
    process->prev = NULL;

    return process;

}

void destroy_process(process_t *process) {
    if (!process) return;
    free(process);
}

//======================================================
// State Management
//======================================================
void process_set_state(process_t *process, process_state_t new_state) {
    if (!process) return;

    // valid state transitions
    switch (process->state) {
        case PROCESS_NEW:
            if (new_state != PROCESS_READY) return;
            break;
        case PROCESS_READY:
            if (new_state != PROCESS_RUNNING) return;
            break;
        case PROCESS_RUNNING:
            if (new_state != PROCESS_WAITING &&
                new_state != PROCESS_READY &&
                new_state != PROCESS_COMPLETED)
                return;
            break;
        case PROCESS_WAITING:
            if (new_state != PROCESS_READY) return;
            break;
        case PROCESS_COMPLETED:
            return;
    }

    process->state = new_state;
}

bool process_is_finished(const process_t *process) {
    if (!process) return true;
    return process->state == PROCESS_COMPLETED;
}

//======================================================
// Runtime Management
//======================================================
void process_update_runtime(process_t *process, process_state_t new_state) {
    if (!process) return;

    // first time running
    if (process->first_run_time == 0 && new_state == PROCESS_RUNNING) {
        process->first_run_time = process->total_time;
    }

    // runtime tracking
    switch (new_state) {
        case PROCESS_RUNNING:
            process->total_time++;
            break;
        case PROCESS_COMPLETED:
            process->completion_time = process->total_time;
            break;
        case PROCESS_READY:
        case PROCESS_WAITING:
        case PROCESS_NEW:
            // these cases don't affect runtime
            break;
    }

    process_set_state(process, new_state);
}

void process_handle_io(process_t *process) {
    // actual I/O is managed by the scheduler, this just sets up the state change
    if (!process) return;
    if (process->state != PROCESS_RUNNING) return;
    process_set_state(process, PROCESS_WAITING);
}

//======================================================
// Metric Helpers
//======================================================
double process_get_response_time(const process_t *process) {
    if (!process || process->first_run_time == 0) return 0;
    // time until first cpu allocation
    return (double) (process->first_run_time - process->arrival_time);
}

double process_get_turnaround_time(const process_t *process) {
    if (!process || !process_is_finished(process)) return 0;
    // total time in system
    return (double) (process->completion_time - process->arrival_time);
}

double process_get_waiting_time(const process_t *process) {
    if (!process) return 0;
    // total time - cpu time --> total time not running
    return (double) ((process->completion_time - process->arrival_time) - process->total_time);
}