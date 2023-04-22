#include"queue.h"


struct queue* queue_new(unsigned int max_size){
    struct queue* queue = malloc(sizeof(struct queue));
    assert(queue);    
    queue->elements = malloc(sizeof(unsigned int)*max_size);
    assert(queue->elements);
    queue->front = max_size - 1;
    queue->rear = max_size - 1;
    return queue;
}

void queue_push(struct queue* queue, unsigned int data){
    assert(queue->rear);
    queue->elements[queue->rear--] = data;
}

unsigned int queue_pop(struct queue* queue){
    assert(queue->front);
    assert(!queue_is_empty(queue));
    return queue->elements[queue->front--];
}

bool queue_is_empty(const struct queue* queue){
    return queue->rear == queue->front;
}

void queue_free(struct queue* queue){
    assert(queue->elements);
    assert(queue);
    free(queue->elements);
    free(queue);
}
