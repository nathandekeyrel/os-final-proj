#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

//======================================================
// Queue Create/Destroy
//======================================================
queue_t *queue_create(void) {
    queue_t *queue = malloc(sizeof(queue_t));
    if (!queue) return NULL;

    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    return queue;
}

void queue_destroy(queue_t *queue) {
    if (!queue) return;

    queue_clear(queue);
    free(queue);
}

//======================================================
// Basic Queue Operations
//======================================================
bool queue_is_empty(const queue_t *queue) {
    return (queue == NULL || queue->size == 0);
}

int queue_size(const queue_t *queue) {
    if (queue) {
        return queue->size;
    }
    return 0;
}

void queue_clear(queue_t *queue) {
    if (!queue) return;
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

//======================================================
// Process Management
//======================================================
bool queue_push(queue_t *queue, process_t *process) {
    if (!queue || !process) return false;

    process->next = NULL;
    process->prev = queue->tail;

    if (queue_is_empty(queue)) {
        queue->head = process;
        queue->tail = process;
    } else {
        queue->tail->next = process;
        process->prev = queue->tail;
        queue->tail = process;
    }

    queue->size++;
    return true;
}

process_t *queue_pop(queue_t *queue) {
    if (!queue || queue_is_empty(queue)) return NULL;

    process_t *process = queue->head;
    queue->head = process->next;

    if (queue->head) {
        queue->head->prev = NULL;
    } else {
        queue->tail = NULL;
    }

    process->next = NULL;
    process->prev = NULL;

    queue->size--;
    return process;
}

process_t *queue_peek(const queue_t *queue) {
    if (!queue) return NULL;
    return queue->head;
}

//======================================================
// Queue Traversal
//======================================================
void queue_foreach(queue_t *queue, queue_process_fn fn, void *data) {
    if (!queue || !fn) return;

    process_t *current = queue->head;
    while (current != NULL) {
        fn(current, data);
        current = current->next;
    }
}
