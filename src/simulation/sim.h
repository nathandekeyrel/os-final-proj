#ifndef SIMULATION_H
#define SIMULATION_H

#include "../schedulers/completely-fair-scheduler/cfs.h"
#include "../core/queue.h"
#include "../metrics/metrics.h"
#include <stdbool.h>

//======================================================
// Simulation Structure
//======================================================
typedef struct simulation {
    cfs_scheduler_t *scheduler;
    queue_t *initial_queue;
    const char *input_file;
    metrics_t *metrics;
    const char *output_file;
    unsigned long current_time;
    bool simulation_complete;
} simulation_t;

//======================================================
// Core Functions
//======================================================
simulation_t *simulation_create(const char *input_file, const char *output_file);
void simulation_destroy(simulation_t *sim);
void simulation_run(simulation_t *sim);

//======================================================
// Helper Functions
//======================================================
bool simulation_load_processes(simulation_t *sim);
void simulation_step(simulation_t *sim);
void simulation_check_arrivals(simulation_t *sim);

#endif