#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Stack.h"

#define CLEAR system("cls")
#define SEPARATOR printf("==================================================\n")
#define MAX_STACK 10
#define LIST_OPTIONS1 "Pasirinkimai:\n\n1. Sukurti steka\n2. Pasirinkti dirbamaji steka\n3. Iseiti"
#define LIST_OPTIONS2 "Pasirinkimai:\n\n1. Istumti reiksme\n2. Isimti reiksme\n3. Paziureti reiksme\n4. Patikrinti ar tuscias\n5. Istrinti steka\n6. Eiti atgal"
#define ERROR1 "Klaida 1: Pasiektas steku limitas"
#define ERROR2 "Klaida 2: Steku sarasas tuscias!"
#define ERROR3 "Klaida 3: Netinkami duomenys!"
#define ERROR4 "Klaida 4: Iveskite teisinga pasirinkimo numeri!"
#define STACK_LIST_EMPTY "Steku sarasas tuscias!"
#define STACK_CREATED "Sukurtas naujas stekas!"
#define STACK_ENTER_NUM "Iveskite dirbamojo steko numeri: "
#define STACK_CURRENT "Esate steke: "
#define ENTER_NUM "Iveskite numeri: "
#define POP_MESSAGE "Isimta reiksme: "
#define PEEK_MESSAGE "Paziureta reiksme: "
#define STACK_EMPTY "Stekas yra tuscias!"
#define STACK_NOT_EMPTY "Stekas nera tuscias!"
#define STACK_FREE "Atlaisvinti nariai: "
#define EXIT_MESSAGE "Iseinama is programos...\nAtlaisvinti nariai: "

typedef struct stackData{
    int index; //Index is -1 if stack is not initialized or is deleted.
    Stack *sptr;
} stackData;

int workIndex;
int workStack;
int curListPos = 0;
stackData stackList[MAX_STACK];

void initializeListIndex();
int printListData();
int deleteList();
int fetchIndex(int);

int main()
{
    //Variables
    int choice, n, asrt = 0;

    //Console interface
    initializeListIndex();
    while(asrt == 0){
        SEPARATOR;
        if(printListData() == 0)
            printf("%s\n", STACK_LIST_EMPTY);
        SEPARATOR;
        printf("%s\n", LIST_OPTIONS1);
        SEPARATOR;
        if (scanf("%d", &choice) == 1 && getchar() == '\n'){
            switch(choice){
            case 1:
                CLEAR;
                if(curListPos < MAX_STACK){
                    stackList[curListPos].sptr = createStack();
                    stackList[curListPos].index = curListPos;
                    printf("\n%s\n\n", STACK_CREATED);
                    ++curListPos;
                } else {
                    printf("\n%s\n\n", ERROR1);
                }
                break;
            case 2:
                CLEAR;
                SEPARATOR;
                if (printListData() == 0){
                    CLEAR;
                    printf("\n%s\n\n", ERROR2);
                    break;
                }
                SEPARATOR;
                printf("\n%s", STACK_ENTER_NUM);
                if (scanf("%d", &workStack) == 1 && getchar() == '\n' && fetchIndex(workStack) != -1){
                    CLEAR;
                    workIndex = fetchIndex(workStack);
                    while(choice != 5 && choice != 6){
                        SEPARATOR;
                        printf("%s%d\n\n",STACK_CURRENT, workIndex + 1);
                        printf("%s\n\n", LIST_OPTIONS2);
                        SEPARATOR;
                        if (scanf("%d", &choice) == 1 && getchar() == '\n'){
                            switch(choice){
                            case 1:
                                printf("%s", ENTER_NUM);
                                if (scanf("%d", &n) == 1 && getchar() == '\n'){
                                    push(&stackList[workIndex].sptr, n);
                                    CLEAR;
                                    printf("\n%s%d\n\n", POP_MESSAGE, n);
                                } else {
                                    while(getchar() != '\n') continue;
                                    CLEAR;
                                    printf("\n%s\n\n", ERROR3);
                                }
                                break;
                            case 2:
                                CLEAR;
                                printf("\n%s%d\n\n", POP_MESSAGE, pop(&stackList[workIndex].sptr));
                                break;
                            case 3:
                                CLEAR;
                                printf("\n%s%d\n\n", PEEK_MESSAGE, peek(&stackList[workIndex].sptr));
                                break;
                            case 4:
                                CLEAR;
                                if (checkEmpty(&stackList[workIndex].sptr) == 1)
                                    printf("\n%s\n\n", STACK_EMPTY);
                                else
                                    printf("\n%s\n\n", STACK_NOT_EMPTY);
                                break;
                            case 5:
                                CLEAR;
                                printf("\n%s%d\n\n", STACK_FREE, deleteStack(&stackList[workIndex].sptr));
                                stackList[workIndex].index = -1;
                                break;
                            case 6:
                                CLEAR;
                                break;
                            default:
                                CLEAR;
                                printf("\n%s\n\n", ERROR4);
                            }
                        } else {
                            CLEAR;
                            printf("\n%s\n\n", ERROR4);
                            while(getchar() != '\n');
                        }
                    }
                } else {
                    CLEAR;
                    printf("\n%s\n\n", ERROR4);
                }
            break;
            case 3:
                CLEAR;
                SEPARATOR;
                printf("%s%d\n", EXIT_MESSAGE, deleteList());
                SEPARATOR;
                return 0;
            default:
                CLEAR;
                printf("\n%s\n\n", ERROR4);
            }
        } else {
            CLEAR;
            printf("\n%s\n\n", ERROR4);
            while(getchar() != '\n');
        }
    }

    //Assert tests

    Stack *test = createStack();
    push(&test, 10);
    assert(test->data == 10);

    push(&test, 12);
    assert(test->data == 12);
    assert(test->next->data == 10);

    assert(peek(&test) == 12);
    assert(checkEmpty(&test) == 0);

    assert(pop(&test) == 12);
    assert(pop(&test) == 10);

    assert(pop(&test) == -1);
    assert(peek(&test) == -1);
    assert(checkEmpty(&test) == 1);

    assert(deleteStack(&test) == 0);
    test = createStack();
    push(&test, 9999);
    push(&test, 8888);
    assert(deleteStack(&test) == 2);
    return 0;
}

void initializeListIndex(){
    for (int i = 0 ; i < MAX_STACK ; ++i)
        stackList[i].index = -1;
}

int printListData(){
    int listNum = 1;

    for (int i = 0 ; i < MAX_STACK ; ++i)
        if(stackList[i].index != -1){
            printf("%d - Stekas %d\n", listNum, stackList[i].index + 1);
            ++listNum;
        }
    return listNum - 1;
}

int deleteList(){
    int deletedElem = 0;
    for (int i = 0 ; i < MAX_STACK ; ++i)
        if(stackList[i].index != -1)
            deletedElem = deletedElem + deleteStack(&stackList[i].sptr);
    return deletedElem;
}

int fetchIndex(int workStack){
    int count = 0;
    for (int i = 0 ; i < MAX_STACK ; ++i){
        if(stackList[i].index != -1)
            ++count;
        if(count == workStack)
            return stackList[i].index;
    }
    return -1;
}
