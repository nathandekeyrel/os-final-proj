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
// Queue Traversal
//======================================================
typedef void (*queue_process_fn)(process_t* process, void* data);
void queue_foreach(queue_t* queue, queue_process_fn fn, void* data);

#endif