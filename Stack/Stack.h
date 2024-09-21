#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

/*
    DOCUMENTATION:

    Creation of the stack:
    You can create a stack by declaring it like a normal POINTER like so:
    Stack *stk;

    You NEED to initialize it using createStack() function like so;
    Stack *stk = createStack();
    OR
    stk = createStack(); IF stk IS DECLARED BEFORE;

    You can use pop(), push(), peek() functions like their name describes.
    You MUST supply a pointer to a stack like so:
    pop(&stk); / peek(&stk)
    If a stack is empty. These functions return -1.
    Otherwise pop() pops a number from the stack and returns it's value.
    Function peek() returns an integer which is at the top of a stack. (no popping)

    push() function additionally takes an integer to be pushed like so:
    push(&stk, num);

    You can use checkEmpty() function to check whether a stack is empty.
    You MUST supply a pointer to a stack like so:
    checkEmpty(&stk);
    If it is the function returns 1. Otherwise it returns 0.

    When the stack is no longer needed, you can delete it by using a function deleteStack();
    You MUST supply a pointer to a stack like so:
    deleteStack(&stk);
    This function returns how many nodes it deleted.
*/
//Declaration of the stack.
//data - holds the value, next - points to the previous ELEMENT (not value) of the stack.

typedef struct Stack {
    int data;
    struct Stack *next;
} Stack;

//Function declaration.

Stack *createStack();
void push(Stack **, int);
int pop(Stack **);
int peek(Stack **);
int checkEmpty(Stack **);
int deleteStack(Stack **);

#endif // STACK_H_INCLUDED
