#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

//Exit code -1 - Cannot initialize a stack due to memory shortage.

//Definitions of Stack library:
Stack *elem;  //helper elements for stacks.

//Initializes an array by assigning NULL (empty) to the stack head.

Stack *createStack(){
    return NULL;
}

//Pushes a value to the stack depending on a condition: Empty/Not empty.

void push(Stack ** sptr, int n){
    elem = (Stack *) malloc (sizeof(Stack));
    if (elem == NULL)
        exit(-1);
    elem->data = n;
    if (*sptr == NULL){
        elem->next = NULL;
    } else
        elem->next = *sptr;
    *sptr = elem;
}

//Pops a value to the stack.
//If stack is empty returns -1. Otherwise returns the popped value.

int pop(Stack ** sptr){
    if (!checkEmpty(sptr)){
        int val = (*sptr)->data;

        elem = *sptr;
        *sptr = elem->next;
        free(elem);
        return val;
    } else
        return -1;
}

//Peeks at the head element of the stack.
//If stack is empty returns -1. Otherwise returns the number at the head of the stack.

int peek(Stack ** sptr){
    if (checkEmpty(sptr))
        return -1;
    else
        return (*sptr)->data;
}

//Checks whether a stack is empty.
//Returns 1 if stack is empty and 0 otherwise.

int checkEmpty(Stack ** sptr){
    if (*sptr == NULL)
        return 1;
    else
        return 0;
}

//Deletes all stack elements.
//Returns how much elements were deleted.

int deleteStack(Stack ** sptr){
    int n = 0;
    while(1){
        if (!checkEmpty(sptr)){
            elem = *sptr;
            *sptr = elem->next;
            free(elem);
            ++n;
        } else
            break;
    }
    return n;
}
