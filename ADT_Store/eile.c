#include "eile.h"

// Funkcija, sukurianti eile
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// Funkcija, patikrinanti, ar eile yra tuscia
int isEmpty(Queue* queue) {
    return (queue->front == NULL);
}

// Funkcija, idedanti nauja elementa i eile
void enqueue(Queue* queue, int data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = data;
    newNode->next = NULL;

    // Jei eile tuscia, naujas elementas yra pradinis ir galinis elementas
    if (isEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    }
    // Kitu atveju, naujas elementas turi buti pridedamas prie galo
    else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->size++;
}

// Funkcija, isimimanti elementa is eiles
int dequeue(Queue* queue) {
    // Patikriname, ar eile tuscia
    if (isEmpty(queue)) {
        //printf("Trying to dequeue error: Queue is empty.\n");
        return -1;
    }

    // Isimame elementa is eiles ir atnaujiname pradini elementa
    int data = queue->front->data;
    QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    queue->size--;
    return data;
}

// Funkcija, gaunanti pirmo eiles elemento duomenis, neisimant jo is eiles
int getFront(Queue* queue) {
    if (isEmpty(queue)) {
        //printf("Trying to get front error: Queue is empty.\n");
        return -1;
    }

    return queue->front->data;
}

// Funkcija, gaunanti eiles elementu skaiciu
int getSize(Queue* queue) {
    return queue->size;
}

// Funkcija, sunaikinanti eile
void destroyQueue(Queue* queue) {
    QueueNode* current = queue->front;
    while (current != NULL) {
        QueueNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}
// Funkcija, kuri klonuoja eile
Queue* cloneQueue(Queue* queue) {
    Queue* clone = createQueue();
    QueueNode* current = queue->front;
    while (current != NULL) {
        enqueue(clone, current->data);
        current = current->next;
    }
    return clone;
}
void printQueue(Queue* queue) {
    QueueNode* currentNode = queue->front;
    printf("Queue contents: ");
    while (currentNode != NULL) {
        printf("%d ", currentNode->data);
        currentNode = currentNode->next;
    }
    printf("\n");
}


