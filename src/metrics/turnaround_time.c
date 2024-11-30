#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../core/queue.h"
#include "../core/process.h"

//Turnaround Time = completion_time - arrival_time

double process_get_turnaround_time(const process_t* process) {
    if (process->completion_time > 0) {
        return process->completion_time - process->arrival_time;
    }
    return -1.0;
}