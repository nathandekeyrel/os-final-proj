#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "queue.h"
#include "process.h"


//Variance = (1/N)SUM 1 to N(Xi - u)^2
//N = num of processes
//Xi is each vruntime value
//u is the mean of the v runtime values

double calculate_vruntime_variance(queue_t* queue) {
    if (queue == NULL || queue_is_empty(queue)) {
        return 0.0;  // No processes in the queue, so variance is 0
    }

    double total_vruntime = 0.0;
    int process_count = 0;
    double sum = 0.0

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
    process_t* current = queue->head;

    while (current != NULL) {
        double runtimeSUBmeanSQAURED = (current->vruntime - u) * (current->vruntime - u);
        sum += runtimeSUBmeanSQAURED;
        current = current->next;
    }

    //DIVIDE BY N THEN RETURN (N is just the total count)
    return sum/process_count;

}


