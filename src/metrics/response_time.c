#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../core/queue.h"
#include "../core/process.h"
//Response Time = first_run_time - arrival_time

double process_get_response_time(const process_t* process) {
    // Assume that the process has `arrival_time` and `first_run_time` fields
    if (process->first_run_time > 0) {
        return process->first_run_timev - process->arrival_time;
    }
    return -1.0;
}