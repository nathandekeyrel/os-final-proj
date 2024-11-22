#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include "process.h"

typedef struct queue {
    process_t* head;
    process_t* tail;
    int size;
} queue_t;

//======================================================
// Queue Create/Destroy
//======================================================

queue_t* queue_create(void);
void queue_destroy(queue_t* queue);

//======================================================
// Basic Queue Operations
//======================================================

bool queue_is_empty(const queue_t* queue);
int queue_size(const queue_t* queue);
void queue_clear(queue_t* queue);

//======================================================
// Process Management
//======================================================

bool queue_push(queue_t* queue, process_t* process);
process_t* queue_pop(queue_t* queue);
process_t* queue_peek(const queue_t* queue);

//======================================================
// Round Robin Specific
//======================================================

void queue_push_back(queue_t* queue, process_t* process);
process_t* queue_rotate(queue_t* queue);

//======================================================
// Priority Queue for SPN/SRT
//======================================================

process_t* queue_get_shortest_burst(const queue_t* queue);
process_t* queue_get_shortest_remaining(const queue_t* queue);
void queue_remove_process(queue_t* queue, process_t* process);

//======================================================
// Queue Traversal
//======================================================

typedef void (*queue_process_fn)(process_t* process, void* data);
void queue_foreach(queue_t* queue, queue_process_fn fn, void* data);

#endif