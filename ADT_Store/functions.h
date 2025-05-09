#ifndef HEADER_H

typedef struct S_Node
{
    int value;
    int priority;
    struct S_Node *next;
} S_Node;

typedef struct S_PriorityQueue
{
    int size;
    S_Node *head;
} S_PriorityQueue;

S_PriorityQueue *Create();

int IsEmpty(S_PriorityQueue *p_queue);

void Insert(S_PriorityQueue *p_queue, int value, int priority);

int Remove(S_PriorityQueue *p_queue);

void Print(S_PriorityQueue *p_queue);

void Join(S_PriorityQueue *p_queue1, S_PriorityQueue *p_queue2);

void DeletePQueue(S_PriorityQueue *p_queue);

int getPQSize(S_PriorityQueue *p_queue);

int getPQFrontValue(S_PriorityQueue *p_queue);

int getPQFrontPriority(S_PriorityQueue *p_queue);

#endif
