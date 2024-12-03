#ifndef CFS_H
#define CFS_H

#include "../../utils//rbtree.h"
#include "../../core/process.h"
#include "../../core/queue.h"

//======================================================
// Scheduler Constants
//======================================================
#define MIN_GRANULARITY 4  // this is the minimum runtime guarantee
#define LATENCY_TARGET 20  // this is the scheduling period length
#define MIN_SLICE 4  // this is the minimum timeslice
#define NICE_0_LOAD 1024  // this is the base load weight

//======================================================
// Scheduler Structure
//======================================================
typedef struct cfs_scheduler {
    struct rb_root run_queue;
    queue_t *wait_queue;
    unsigned long runtime;
    unsigned int number_of_runnable_processes;
    process_t *current;
} cfs_scheduler_t;

//======================================================
// Scheduler Stuff
//======================================================
cfs_scheduler_t *cfs_create(void);
void cfs_destroy(cfs_scheduler_t *scheduler);

//======================================================
// Process Management
//======================================================
void cfs_enqueue_process(cfs_scheduler_t *scheduler, process_t *process);
process_t *cfs_pick_next_task(cfs_scheduler_t *scheduler);
void cfs_put_prev_task(cfs_scheduler_t *scheduler, process_t *process);
void cfs_remove_process(cfs_scheduler_t *scheduler, process_t *process);

//======================================================
// Scheduling Operations
//======================================================
void cfs_tick(cfs_scheduler_t *scheduler);
process_t *cfs_schedule(cfs_scheduler_t *scheduler);
void cfs_check_preempt(cfs_scheduler_t *scheduler, process_t *process);

//======================================================
// Virtual Runtime Management
//======================================================
double cfs_calc_delta_fair(cfs_scheduler_t *scheduler, process_t *process);
void cfs_update_vruntime(process_t *process, double delta);
unsigned int cfs_calc_timeslice(cfs_scheduler_t *scheduler, process_t *process);

//======================================================
// Utility Functions
//======================================================
unsigned int cfs_calc_load_weight(int priority);
bool cfs_should_preempt(process_t *current, process_t *next);



#endif