#include <stdlib.h>
#include <stdio.h>
#include "cfs.h"


//======================================================
// Scheduler Stuff
//======================================================
cfs_scheduler_t *cfs_create(void) {
    cfs_scheduler_t *scheduler = malloc(sizeof(cfs_scheduler_t));
    if (!scheduler) return NULL;

    scheduler->run_queue = (struct rb_root) RB_ROOT;

    scheduler->wait_queue = queue_create();
    if (!scheduler->wait_queue) {
        free(scheduler);
        return NULL;
    }

    scheduler->runtime = 0;
    scheduler->number_of_runnable_processes = 0;
    scheduler->current = NULL;

    return scheduler;
}

void cfs_destroy(cfs_scheduler_t *scheduler) {
    if (!scheduler) return;
    if (scheduler->wait_queue) {
        queue_destroy(scheduler->wait_queue);
    }
    free(scheduler);
}

//======================================================
// Process Management
//======================================================
static bool compare_vruntime(struct rb_node *a, struct rb_node *b) {
    // this is the compare function used in the compare callback in rbtree insert
    process_t *process_a = rb_entry(a, process_t, rb_node);
    process_t *process_b = rb_entry(b, process_t, rb_node);
    return process_a->vruntime < process_b->vruntime;
}

void cfs_enqueue_process(cfs_scheduler_t *scheduler, process_t *process) {
    // add process to rbtree based on vruntime
    if (!scheduler || !process) return;

    rb_insert(&process->rb_node, &scheduler->run_queue, compare_vruntime);
    scheduler->number_of_runnable_processes++;

    process_set_state(process, PROCESS_READY);
}

process_t *cfs_pick_next_task(cfs_scheduler_t *scheduler) {
    // return leftmost node -- ndoe with lowest vruntime
    if (!scheduler || !scheduler->number_of_runnable_processes) return NULL;

    struct rb_node *node = rb_first(&scheduler->run_queue);
    if (!node) return NULL;

    return rb_entry(node, process_t, rb_node);
}

void cfs_put_prev_task(cfs_scheduler_t *scheduler, process_t *process) {
    if (!scheduler || !process) return;

    if (!process_is_finished(process)) {
        cfs_enqueue_process(scheduler, process);
    }
}

void cfs_remove_process(cfs_scheduler_t *scheduler, process_t *process) {
    if (!scheduler || !process) return;

    rb_erase(&process->rb_node, &scheduler->run_queue);
    scheduler->number_of_runnable_processes--;
}

//======================================================
// Scheduling Operations
//======================================================
void cfs_tick(cfs_scheduler_t *scheduler) {
    /*
     * on each scheduler tick:
     *   1. update scheduler runtime
     *   2. calc and update process' vruntime
     *   3. check for preemption at timeslice
     */
    if (!scheduler || !scheduler->current) return;

    scheduler->runtime++;

    double delta = cfs_calc_delta_fair(scheduler, scheduler->current);
    cfs_update_vruntime(scheduler->current, delta);
    process_update_runtime(scheduler->current, PROCESS_RUNNING);

    unsigned int timeslice = cfs_calc_timeslice(scheduler, scheduler->current);
    if (scheduler->runtime % timeslice == 0) {
        cfs_check_preempt(scheduler, scheduler->current);
    }
}

process_t *cfs_schedule(cfs_scheduler_t *scheduler) {
    if (!scheduler) return NULL;

    if (scheduler->current) {  // handle current process if it exists
        process_t *prev = scheduler->current;
        cfs_put_prev_task(scheduler, prev);
        scheduler->current = NULL;
    }

    process_t *next = cfs_pick_next_task(scheduler);
    // picks next process, if there is one update schedulers current pointer
    if (next) {
        cfs_remove_process(scheduler, next);
        scheduler->current = next;
        process_set_state(next, PROCESS_RUNNING);
    }

    return next;
}

void cfs_check_preempt(cfs_scheduler_t *scheduler, process_t *process) {
    if (!scheduler || !process) return;

    process_t *next = cfs_pick_next_task(scheduler);
    if (!next) return;

    if (cfs_should_preempt(process, next)) {
        process_set_state(process, PROCESS_READY);
        cfs_schedule(scheduler);
    }
}

//======================================================
// Virtual Runtime Management
//======================================================
double cfs_calc_delta_fair(cfs_scheduler_t *scheduler, process_t *process) {
    if (!scheduler || !process) return 0.0;

    unsigned int weight = cfs_calc_load_weight(process->priority);

    /*
     * calculate teh fair delta
     * delta_fair = delta * (NICE_0_LOAD) / weight
     * this makes sure lower priority (higher nice value) prcesses
     * accumulate vruntime more quickly
     */
    double delta = 1.0 * NICE_0_LOAD / weight;
    return delta;
}

void cfs_update_vruntime(process_t *process, double delta) {
    if (!process) return;
    process->vruntime += delta;
}

unsigned int cfs_calc_timeslice(cfs_scheduler_t *scheduler, process_t *process) {
    if (!scheduler || !process) return MIN_SLICE;

    unsigned int weight = cfs_calc_load_weight(process->priority);

    if (scheduler->number_of_runnable_processes <= 1) {
        return LATENCY_TARGET;
    }

    // timeslice = (weight / total_weight) * latency target
    // higher weight means higher timeslice
    unsigned int timeslice = (weight * LATENCY_TARGET) /
                             (scheduler->number_of_runnable_processes * NICE_0_LOAD);

    // minimum timeslice to reduce amount of switching
    return (timeslice < MIN_SLICE) ? MIN_SLICE : timeslice;
}

//======================================================
// Utility Functions
//======================================================
unsigned int cfs_calc_load_weight(int priority) {
    unsigned int weight = NICE_0_LOAD;

    if (priority > 0) {
        while (priority > 0) {
            weight = (weight * 4) / 5;
            priority--;
        }
    } else if (priority < 0) {
        while (priority < 0) {
            weight = (weight * 5) / 4;
            priority++;
        }
    }

    return weight;
}

bool cfs_should_preempt(process_t *current, process_t *next) {
    if (!current || !next) return false;

    /*
     * preemption based on:
     *   1. virtual runtime diff - allows process with lower vruntime to preempt
     *   2. min granularity check - prevents too many context switches (min runtime diff)
     */
    double delta = current->vruntime - next->vruntime;

    return (delta > MIN_GRANULARITY && next->vruntime < current->vruntime);
}

