#ifndef METRICS_H
#define METRICS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../core/queue.h"
#include "../core/process.h"

double calculate_vruntime_variance(queue_t* queue);

double process_get_response_time(const process_t* process);

double process_get_turnaround_time(const process_t* process);


#endif // METRICS_H