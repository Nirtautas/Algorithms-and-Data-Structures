#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

S_PriorityQueue *Create()
{
    S_PriorityQueue *p_queue = (S_PriorityQueue *)malloc(sizeof(S_PriorityQueue));
    p_queue->head = NULL;
    p_queue->size = 0;
    return p_queue;
}

int IsEmpty(S_PriorityQueue *p_queue)
{
    if (p_queue->head == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Insert(S_PriorityQueue *p_queue, int value, int priority)
{
    S_Node *new_node = (S_Node *)malloc(sizeof(S_Node));
    new_node->value = value;
    new_node->priority = priority;
    new_node->next = NULL;

    if (p_queue->head == NULL || p_queue->head->priority < priority)
    {
        new_node->next = p_queue->head;
        p_queue->head = new_node;
    }
    else
    {
        S_Node *pointer = p_queue->head;
        while (pointer->next != NULL && pointer->next->priority > priority)
        {
            pointer = pointer->next;
        }
        new_node->next = pointer->next;
        pointer->next = new_node;
    }
    p_queue->size += 1;
}

int Remove(S_PriorityQueue *p_queue)
{
    S_Node *node = (S_Node *)malloc(sizeof(S_Node));
    p_queue->size -= 1;

    if (p_queue->head != NULL) {
        node = p_queue->head;
        p_queue->head = p_queue->head->next;
        return node->value;
    } else
        return -1;
}

void Print(S_PriorityQueue *p_queue)
{
    S_Node *pointer = p_queue->head;
    while (pointer != NULL)
    {
        printf("Reiksme: %d, Prioritetas: %d\n", pointer->value, pointer->priority);
        pointer = pointer->next;
    }
}

void Join(S_PriorityQueue *p_queue1, S_PriorityQueue *p_queue2)
{
    if (p_queue1 != NULL && p_queue2 != NULL)
    {
        S_Node *pointer = p_queue2->head;
        while (pointer != NULL)
        {
            Insert(p_queue1, pointer->value, pointer->priority);
            p_queue2->head = p_queue2->head->next;
            pointer = pointer->next;
        }
    }
}

void DeletePQueue(S_PriorityQueue *p_queue)
{
    S_Node *pointer = p_queue->head;
    while (pointer != NULL)
    {
        S_Node *temp = pointer->next;
        free(pointer);
        pointer = temp;
    }
    p_queue->head = NULL;
}

int getPQSize(S_PriorityQueue *p_queue)
{
    return p_queue->size;
}

int getPQFrontValue(S_PriorityQueue *p_queue)
{
    if (p_queue->head != NULL)
        return p_queue->head->value;
    return 0;
}

int getPQFrontPriority(S_PriorityQueue *p_queue)
{
    if (p_queue->head != NULL)
        return p_queue->head->priority;
    return 0;
}
