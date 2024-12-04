#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "metrics.h"
#include "../core/queue.h"
#include "../core/process.h"


double process_get_response_time(const process_t* process) {
    // Assume that the process has `arrival_time` and `first_run_time` fields
    if (process->first_run_time > 0) {
        return process->first_run_time - process->arrival_time;
    }
    return -1.0;
}

double process_get_turnaround_time(const process_t* process) {
    if (process->completion_time > 0) {
        return process->completion_time - process->arrival_time;
    }
    return -1.0;
}


double calculate_vruntime_variance(queue_t* queue) {
    if (queue == NULL || queue_is_empty(queue)) {
        return 0.0;  // No processes in the queue, so variance is 0
    }

    double total_vruntime = 0.0;
    int process_count = 0;
    double sum = 0.0;

    process_t* current = queue->head;

    //THIS IS THE LOOP TO GET THE N, AND TOTAL VRUNTIME
    while (current != NULL) {
        total_vruntime += current->vruntime;
        process_count++;

        current = current->next;
    }

    //MEAN
    double u = total_vruntime/process_count;

    //DO EQUATION
    process_t* current2 = queue->head;

    while (current2 != NULL) {
        double runtimeSUBmeanSQAURED = (current2->vruntime - u) * (current2->vruntime - u);
        sum += runtimeSUBmeanSQAURED;
        current2 = current2->next;
    }

    //DIVIDE BY N THEN RETURN (N is just the total count)
    return sum/process_count;

}
