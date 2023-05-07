#ifndef QUEUE_H
#define QUEUE_H

/// \cond
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
/// \endcond
struct queue{
    unsigned int* elements;
    unsigned int front;
    unsigned int rear;
};

struct queue* queue_new(unsigned int max_size);

void queue_push(struct queue* q, unsigned int data);

unsigned int queue_pop(struct queue* q);

bool queue_is_empty(const struct queue* q);

void queue_free(struct queue* q);

#endif
