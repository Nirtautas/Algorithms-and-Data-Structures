#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode {
    int data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;


Queue* cloneQueue(Queue* queue);
Queue* createQueue();
int isEmpty(Queue* queue);
void enqueue(Queue* queue, int data);
int dequeue(Queue* queue);
int getFront(Queue* queue);
int getSize(Queue* queue);
void destroyQueue(Queue* queue);
void printQueue(Queue* queue);

#endif
