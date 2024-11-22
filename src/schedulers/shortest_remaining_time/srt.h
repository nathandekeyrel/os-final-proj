#ifndef SRT_H
#define SRT_H

#include "queue.h"
#include "process.h"

//======================================================
// Scheduler Struct
//======================================================

typedef struct srt_scheduler {
    queue_t* ready_queue;
    process_t* current_process;
    unsigned long current_time;
    unsigned long time_slice;
} srt_scheduler_t;

//======================================================
// Create/Destroy Scheduler
//======================================================

srt_scheduler_t srt_create(unsigned long time_slice);
void srt_destroy(srt_scheduler_t* scheduler);

//======================================================
// Core Functions
//======================================================

void srt_add_process(srt_scheduler_t* scheduler, process_t* process);
process_t* srt_get_next_process(srt_scheduler_t* scheduler);
void srt_run_schedule(srt_scheduler_t* scheduler);

//======================================================
// Preempt
//======================================================

bool srt_should_preempt(srt_scheduler_t scheduler, process_t* new_process);

#endif
