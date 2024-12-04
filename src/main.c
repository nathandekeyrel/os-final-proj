#include <stdio.h>
#include <stdlib.h>
#include "schedulers/completely-fair-scheduler/cfs.h"
#include "core/process.h"
#include "core/queue.h"
#include "metrics/metrics.h"

void simulate_cfs(cfs_scheduler_t *scheduler, int total_time) {

    for (int t = 0; t < total_time; t++) {
        printf("\n--- Time %d ---\n", t);
        cfs_tick(scheduler);

        process_t *next_process = cfs_schedule(scheduler);

        if (next_process != NULL) {
            printf("Running Process PID: %d, Priority: %d, VRUNTIME: %.2f\n",
                   next_process->pid, next_process->priority, next_process->vruntime);

            unsigned int timeslice = cfs_calc_timeslice(scheduler, next_process);
            printf("Timeslice for PID %d: %u\n", next_process->pid, timeslice);

            if (t % 5 == 0) {
                process_handle_io(next_process);
                printf("Process PID %d moved to WAITING due to I/O.\n", next_process->pid);
            }

            cfs_check_preempt(scheduler, next_process);
        } else {
            printf("No process ready to run.\n");
        }

        double variance = calculate_vruntime_variance(scheduler->wait_queue);
        printf("Variance of VRUNTIME at time %d: %.2f\n", t, variance);

        if (scheduler->wait_queue->head != NULL) {
            double response_time = process_get_response_time(scheduler->wait_queue->head);
            double turnaround_time = process_get_turnaround_time(scheduler->wait_queue->head);

            printf("Response Time of PID %d: %.2f\n", scheduler->wait_queue->head->pid, response_time);
            printf("Turnaround Time of PID %d: %.2f\n", scheduler->wait_queue->head->pid, turnaround_time);
        }
    }
}


int main() {
    // Create a new CFS scheduler
    cfs_scheduler_t *scheduler = cfs_create();

    // Create sample processes with the properties you're tracking
    process_t *p1 = process_create(1, 0, 116, 50, 3);
    process_t *p2 = process_create(2, 0, 122, 43, 2);
    process_t *p3 = process_create(3, 0, 116, 40, 2);
    process_t *p4 = process_create(4, 5, 130, 32, 4);
    process_t *p5 = process_create(5, 10, 131, 26, 3);
    process_t *p6 = process_create(6, 14, 103, 12, 6);
    process_t *p7 = process_create(7, 16, 126, 7, 7);
    process_t *p8 = process_create(8, 17, 104, 39, 2);
    process_t *p9 = process_create(9, 26, 111, 17, 2);
    process_t *p10 = process_create(10, 36, 133, 10, 5);

    // Enqueue processes to the scheduler
    cfs_enqueue_process(scheduler, p1);
    cfs_enqueue_process(scheduler, p2);
    cfs_enqueue_process(scheduler, p3);
    cfs_enqueue_process(scheduler, p4);
    cfs_enqueue_process(scheduler, p5);
    cfs_enqueue_process(scheduler, p6);
    cfs_enqueue_process(scheduler, p7);
    cfs_enqueue_process(scheduler, p8);
    cfs_enqueue_process(scheduler, p9);
    cfs_enqueue_process(scheduler, p10);

    // Simulate the CFS scheduler
    simulate_cfs(scheduler, 50);

    // Clean up
    cfs_destroy(scheduler);

    return 0;
}
