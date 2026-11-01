#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <windows.h>
#define BLUE "\033[34m"
#define RESET "\033[0m"

const int poizon = -67;
const int none = -1;
const int zero = 0;

//Нумерация элементов начинается с единицы

enum
{
    OK = 0,
    CALL_CONSTRUCTOR_FOR_EXISTING_LIST = 1,
    SEG_FAULT_LIST = 2,
    SEG_FAULT_DATA = 3,
    SEG_FAULT_NEXT = 4,
    SEG_FAULT_PREV = 5,
    LIST_OVERFLOW = 6,
    LOOP_LIST = 7,
    ELEMENT_DOES_NOT_EXIST = 8,
    LIST_INDEX_OUT_OF_RANGE = 9
};

typedef struct
{
    int* data;
    int* next;
    int* prev;
    int free_head;
    int capacity;
} List_t;

int CreateList(List_t* lst, int capacity);
int CtorVerify(List_t* lst);
int ListVerify(List_t* lst);
int ListAllocNode(List_t* lst);
void ListFreeNode(List_t* lst, int idx);
int ListPushFront(List_t* lst, int value);
void ListErase(List_t* lst, int idx);
int ListPushBack(List_t* lst, int value);
void ListPopBack(List_t* lst);
void ListPopFront(List_t* lst);
void ListPrint(List_t* lst);
int ListFind(List_t* lst, int value);
int ListFindIndex(List_t* lst, int idx);
void ListInsertAfter(List_t* lst, int idx, int value);
void ListInsertBefore(List_t* lst, int idx, int value);
void ListClear(List_t* lst);
void ListDump(List_t* lst);
void ListLink(List_t* lst, int idx, int a, int b);
void ListFree(List_t* lst);

#ifndef DEBUG
#define CHECK()
#else
#define CHECK()\
{\
    state = ListVerify(lst);\
    if (state != OK)\
    {\
        printf("ERROR %d\n", state);\
        exit(-1);\
    }\
}
#endif

void ListFree(List_t* lst)
{
    free(lst->data);
    free(lst->next);
    free(lst->prev);
}

void ListLink(List_t* lst, int idx, int left, int right)
{
    int state = OK;
    CHECK();

    lst->next[left] = idx;
    lst->prev[right] = idx;
    lst->prev[idx] = left;
    lst->next[idx] = right;

    CHECK();
}

#define PrintArray(arr)\
{\
    printf(#arr" (%p): ", lst->arr);\
    printf(BLUE "%4d " RESET, lst->arr[0]);\
    for(int i = 1; i <= lst->capacity; ++i)\
    {\
        printf("%4d ", lst->arr[i]);\
    }\
    printf("\n");\
}

void ListDump(List_t* lst)
{
    printf("indexes data:            ");
    for(int i = 0; i <= lst->capacity; ++i)
    {
        printf("%4d ", i);
    }
    printf("\n");
    PrintArray(data);
    PrintArray(next);
    PrintArray(prev);
    printf("info: ");
    ListPrint(lst);
    printf("\n");
}

void ListClear(List_t* lst)
{
    int state = OK;
    CHECK();

    lst->next[0] = zero;
    lst->prev[0] = zero;
    lst->free_head = 1;

    for (int i = 1; i <= lst->capacity; ++i)
    {
        lst->data[i] = poizon;
        lst->next[i] = i + 1;
        lst->prev[i] = none;
    }
    lst->next[lst->capacity] = zero;

    CHECK();
}

void ListInsertBefore(List_t* lst, int idx, int value)
{
    int state = OK;
    CHECK();

    if (lst->prev[idx] != zero)
    {
        int id = ListAllocNode(lst);
        lst->data[id] = value;
        lst->prev[id] = lst->prev[idx];
        lst->next[id] = idx;
        lst->next[lst->prev[id]] = id;
        lst->prev[idx] = id;
        //printf("IM here!\n");
    }

    else ListPushFront(lst, value);
    CHECK();
}

void ListInsertAfter(List_t* lst, int idx, int value)
{
    int state = OK;
    CHECK();

    if (lst->next[idx] != zero)
    {
        int id = ListAllocNode(lst);
        lst->data[id] = value;
        lst->next[id] = lst->next[idx];
        lst->prev[id] = idx;
        lst->prev[lst->next[id]] = id;
        lst->next[idx] = id;
        //printf("IM here!\n");
    }
    else ListPushBack(lst, value);

    CHECK();
}

int ListFind(List_t* lst, int value)
{
    int state = OK;
    CHECK();

    for (int i = lst->next[0]; i != zero; i = lst->next[i])
    {
        if (lst->data[i] == value) return i;
    }

    #ifdef DEBUG
    printf("ERROR %d\n", ELEMENT_DOES_NOT_EXIST);
    ListDump(lst);
    printf("\a");
    #endif
    return zero;
}

int ListFindIndex(List_t* lst, int idx)
{
    int state = OK;
    CHECK();

    int index = lst->next[0];
    for (int i = 1; i < idx; ++i)
    {
        index = lst->next[index];
        if (index == 0)
        {
            #ifdef DEBUG
            printf("ERROR %d; no index %d\n", LIST_INDEX_OUT_OF_RANGE, idx);
            ListDump(lst);
            printf("\a");
            #endif
            return zero;
        }
    }

    return index;
}

void ListPrint(List_t* lst)
{
    int state = OK;
    CHECK();

    int i = lst->next[0];

    while (i != 0)
    {
        printf("%d ", lst->data[i]);
        i = lst->next[i];
    }
    printf("\n");
}

void ListPopFront(List_t* lst)
{
    ListErase(lst, lst->next[0]);
}

void ListPopBack(List_t* lst)
{
    ListErase(lst, lst->prev[0]);
}

void ListErase(List_t* lst, int idx)
{
    int state = OK;
    CHECK();

    int left = lst->prev[idx];
    int right = lst->next[idx];

    lst->next[left] = right;
    lst->prev[right] = left;

    ListFreeNode(lst, idx);

    CHECK();
}

void ListFreeNode(List_t* lst, int idx)
{
    int state = OK;
    CHECK();

    lst->next[idx] = lst->free_head;
    lst->prev[idx] = none;
    lst->free_head = idx;

    CHECK();
}

int ListAllocNode(List_t* lst)
{
    int state = OK;
    CHECK();

    if (lst->free_head == 0) return zero;
    int idx = lst->free_head;
    lst->free_head = lst->next[idx];

    CHECK();
    return idx;
}

int CtorVerify(List_t* lst)
{
    if (lst == NULL) return SEG_FAULT_LIST;
    if (lst->data == NULL && lst->next == NULL && lst->capacity == 0) return OK;
    return CALL_CONSTRUCTOR_FOR_EXISTING_LIST;
}

int ListVerify(List_t* lst)
{
    if (lst == NULL) return SEG_FAULT_LIST;
    if (lst->data == NULL) return SEG_FAULT_DATA;
    if (lst->next == NULL) return SEG_FAULT_NEXT;
    if (lst->prev == NULL) return SEG_FAULT_PREV;

    int i = lst->next[0];
    int counter = 0;
    while (i != 0)
    {
        i = lst->next[i];
        if (++counter > lst->capacity) return LOOP_LIST;
    }

    return OK;
}

int ListPushFront(List_t* lst, int value)
{
    int state = OK;
    CHECK();

    int idx = ListAllocNode(lst);
    if (idx == zero)
    {
        #ifdef DEBUG
        printf("ERROR %d, element %d haven't been pushed front, because capacity = %d\nList: ", LIST_OVERFLOW, value, lst->capacity);
        ListPrint(lst);
        printf("\n");
        #endif
        return LIST_OVERFLOW;
    }

    lst->data[idx] = value;

    int head = lst->next[0];
    ListLink(lst, idx, 0, head);

    CHECK();
    return OK;
}

int ListPushBack(List_t* lst, int value)
{
    int state = OK;
    CHECK();

    int idx = ListAllocNode(lst);
    if (idx == zero)
    {
        #ifdef DEBUG
        printf("ERROR %d, element %d haven't been pushed back, because capacity = %d\nList: ", LIST_OVERFLOW, value, lst->capacity);
        ListPrint(lst);
        printf("\n");
        #endif
        return LIST_OVERFLOW;
    }

    lst->data[idx] = value;

    int tail = lst->prev[0];
    ListLink(lst, idx, tail, 0);

    CHECK();
    return OK;
}

int CreateList(List_t* lst, int capacity)
{
    int state = CtorVerify(lst);
    #ifdef DEBUG
    if (state != OK)
    {
        printf("ERROR %d\n", state);
        exit(-1);
    }
    #endif

    lst->data = calloc(capacity + 1, sizeof(int));
    lst->next = calloc(capacity + 1, sizeof(int));
    lst->prev = calloc(capacity + 1, sizeof(int));
    lst->capacity = capacity;
    lst->free_head = 1;

    lst->data[0] = poizon;
    lst->next[0] = zero;
    lst->prev[0] = zero;

    for (int i = 1; i <= capacity; ++i)
    {
        lst->data[i] = poizon;
        lst->next[i] = i + 1;
        lst->prev[i] = none;
    }
    lst->next[capacity] = zero;

    CHECK();
    return state;
}

#endif // LIST_H_INCLUDED
