#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

const int poizon = -67;
const int none = -1;
const int zero = 0;

//Нумерация элементов начинается с единицы

enum
{
    OK,
    CALL_CONSTRUCTOR_FOR_EXISTING_LIST,
    SEG_FAULT_LIST,
    SEG_FAULT_DATA,
    SEG_FAULT_NEXT,
    SEG_FAULT_PREV,
    LIST_OVERFLOW,
    LOOP_LIST
};

typedef struct
{
    int* data;
    int* next;
    int* prev;
    int head;
    int tail;
    int free_head;
    int capacity;
} List_t;

int CreateList(List_t* lst, int capacity);
int CtorVerify(List_t* lst);
int ListVerify(List_t* lst);
int ListAllocNode(List_t* lst);
void ListFreeNode(List_t* lst, int idx);
int ListPushFront(List_t* lst, int value);
int ListErase(List_t* lst, int idx);
int ListPushBack(List_t* lst, int value);
void ListPopBack(List_t* lst);
void ListPopFront(List_t* lst);
void ListPrint(List_t* lst);
int ListFind(List_t* lst, int value);
int ListFindIndex(List_t* lst, int idx);
int ListInsertAfter(List_t* lst, int idx, int value);
int ListInsertBefore(List_t* lst, int idx, int value);
int ListClear(List_t* lst);

/*int ListClear(List_t* lst)
{
    ListVerify(lst);
    lst->next[0] = zero;
    lst->prev[0] = zero;

    for (int i = 1; i < lst->capacity; ++i)
    {
        lst->next[i] = i + 1;
        lst->prev[i] = none;
    }
    lst->next[lst->capacity] = zero;

    return ListVerify(lst);
}*/

int ListInsertBefore(List_t* lst, int idx, int value)
{
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
    return ListVerify(lst);
}

int ListInsertAfter(List_t* lst, int idx, int value)
{
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
    return ListVerify(lst);
}

int ListFind(List_t* lst, int value)
{
    for (int i = lst->head; i != zero; i = lst->next[i])
    {
        if (lst->data[i] == value) return i;
    }

    return zero;
}

int ListFindIndex(List_t* lst, int idx)
{
    int index = lst->head;
    for (int i = 1; i < idx; ++i)
    {
        index = lst->next[index];
        if (index == 0) return none;
    }

    return index;
}

void ListPrint(List_t* lst)
{
    int state = ListVerify(lst);

    if (state != OK) return;
    int i = lst->head;

    while (i != 0)
    {
        printf("%d ", lst->data[i]);
        i = lst->next[i];
    }
    printf("\n");
}

void ListPopFront(List_t* lst)
{
    ListErase(lst, lst->head);
}

void ListPopBack(List_t* lst)
{
    ListErase(lst, lst->tail);
}

int ListErase(List_t* lst, int idx)
{
    if (lst->prev[idx] != zero) lst->next[lst->prev[idx]] = lst->next[idx];
    else lst->head = lst->next[idx];

    if (lst->next[idx] != zero) lst->prev[lst->next[idx]] = lst->prev[idx];
    else lst->tail = lst->prev[idx];

    ListFreeNode(lst, idx);
    return ListVerify(lst);
}

void ListFreeNode(List_t* lst, int idx)
{
    lst->next[idx] = lst->free_head;
    lst->prev[idx] = none;
    lst->free_head = idx;
}

int ListAllocNode(List_t* lst)
{
    if (lst->free_head == 0) return zero;
    int idx = lst->free_head;
    lst->free_head = lst->next[idx];
    return idx;
}

int CtorVerify(List_t* lst)
{
    if (lst == NULL) return SEG_FAULT_LIST;
    if (lst->data == NULL && lst->head == 0 && lst->next == NULL && lst->tail == 0 && lst->capacity == 0) return OK;
    return CALL_CONSTRUCTOR_FOR_EXISTING_LIST;
}

int ListVerify(List_t* lst)
{
    if (lst == NULL) return SEG_FAULT_LIST;
    if (lst->data == NULL) return SEG_FAULT_DATA;
    if (lst->next == NULL) return SEG_FAULT_NEXT;
    if (lst->prev == NULL) return SEG_FAULT_PREV;

    int i = lst->head;
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
    int idx = ListAllocNode(lst);
    if (idx == zero) return LIST_OVERFLOW;

    lst->data[idx] = value;
    lst->prev[idx] = zero;
    lst->next[idx] = lst->head;

    if (lst->head != 0) lst->prev[lst->head] = idx;
    else lst->tail = idx;

    lst->head = idx;
    return ListVerify(lst);
}

int ListPushBack(List_t* lst, int value)
{
    int idx = ListAllocNode(lst);
    if (idx == zero) return LIST_OVERFLOW;

    lst->data[idx] = value;
    lst->next[idx] = zero;
    lst->prev[idx] = lst->tail;

    if (lst->tail != 0) lst->next[lst->tail] = idx;
    else lst->head = idx;

    lst->tail = idx;
    return ListVerify(lst);
}

int CreateList(List_t* lst, int capacity)
{
    int state = CtorVerify(lst);
    if (state != OK) return state;
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
    return ListVerify(lst);
}

#endif // LIST_H_INCLUDED
