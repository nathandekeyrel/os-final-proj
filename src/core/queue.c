#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

//======================================================
// Queue Create/Destroy
//======================================================

queue_t* queue_create(void) {
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    if (queue) {
        queue->head = NULL;
        queue->tail = NULL;
        queue->size = 0;
    }
    return queue;
}

void queue_destroy(queue_t* queue) {
    if (queue) {
        queue_clear(queue);
        free(queue);
    }
}

//======================================================
// Basic Queue Operations
//======================================================

bool queue_is_empty(const queue_t* queue) {
    if(queue == NULL || queue->size == 0){
        return true;
    }
    else{
        return false;
    }
}

int queue_size(const queue_t* queue) {
    if(queue){
        return queue->size;
    }
    return 0;
}

void queue_clear(queue_t* queue) {
    if (queue) {
        process_node_t* current = queue->head;
        while (current != NULL) {
            process_node_t* next = current->next;
            free(current->process);
            free(current);
            current = next;
        }
        queue->head = queue->tail = NULL;
        queue->size = 0;
    }
}

//======================================================
// Process Management
//======================================================
bool queue_push(queue_t* queue, process_t* process) {
    if (queue == NULL || process == NULL) {
        return false;
    }

    process->next = NULL;
    process->prev = queue->tail;

    if (queue_is_empty(queue)) {
        queue->head = process;
    } else {
        queue->tail->next = process;
    }

    queue->tail = process;
    queue->size++;
    return true;
}

process_t* queue_pop(queue_t* queue) {
    if (queue_is_empty(queue)) {
        return NULL;
    }

    process_t* process = queue->head;
    queue->head = process->next;
    if (queue->head) {
        queue->head->prev = NULL;
    } else {
        queue->tail = NULL;
    }

    process->next = NULL;
    queue->size--;
    return process;
}


process_t* queue_peek(const queue_t* queue) {
    if (queue_is_empty(queue)) {
        return NULL;
    }
    return queue->head->process;
}

//======================================================
// Queue Traversal
//======================================================

void queue_foreach(queue_t* queue, queue_process_fn fn, void* data) {
    if (queue && fn) {
        process_node_t* current = queue->head;
        while (current != NULL) {
            fn(current->process, data);
            current = current->next;
        }
    }
}
