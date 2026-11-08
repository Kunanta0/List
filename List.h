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
    LIST_INDEX_OUT_OF_RANGE = 9,
    INVALID_INDEX = 10,
    CONNECTION_FAILED = 11,
    CANARY_DAMAGED = 12,
    ERROR_COMPILING_GRAPHVIZ = 13
};

typedef struct
{
    int* data;
    int* next;
    int* prev;
    int free_head;
    int capacity;
    int size;
    int dump_count;
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
int ListInsertAfter(List_t* lst, int idx, int value);
int ListInsertBefore(List_t* lst, int idx, int value);
void ListClear(List_t* lst);
void ListDump(List_t* lst);
void ListLink(List_t* lst, int idx, int a, int b);
void ListFree(List_t* lst);
int CheckIndex(List_t* lst, int idx);
void ListDumpDot(const List_t* lst, const char *filename, const char* func, int idx, int value);
void ListDumpFinish(List_t* lst);

int CheckIndex(List_t* lst, int idx)
{
    if (idx > lst->capacity || idx < 0) return LIST_INDEX_OUT_OF_RANGE;
    for (int i = lst->free_head; i != none; i = lst->next[i])
    {
        if (idx == i) return INVALID_INDEX;
    }
    return OK;
}

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
    printf("size = %d\n", lst->size);
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
    lst->next[lst->capacity] = none;

    CHECK();
}

int ListInsertBefore(List_t* lst, int idx, int value)
{
    int state = OK;
    CHECK();

    #ifdef DEBUG
    state = CheckIndex(lst, idx);
    if (state != OK)
    {
        printf("ERROR %d, index %d is forbidden\n", state, idx);
        ListDump(lst);
        return state;
    }
    #endif

    int id = ListAllocNode(lst);

    if (id == none)
    {
        #ifdef DEBUG
        printf("ERROR %d, element %d haven't been pushed front, because capacity = %d\nList: ", LIST_OVERFLOW, value, lst->capacity);
        ListPrint(lst);
        printf("\n");
        #endif
        return LIST_OVERFLOW;
    }

    lst->data[id] = value;
    int left = lst->prev[idx];
    int right = idx;
    ListLink(lst, id, left, right);
    CHECK();
    ListDumpDot(lst, "dot.txt", "InsertBefore", idx, value);

    return OK;
}

int ListInsertAfter(List_t* lst, int idx, int value)
{
    int state = OK;
    CHECK();

    #ifdef DEBUG
    state = CheckIndex(lst, idx);
    if (state != OK)
    {
        printf("ERROR %d, index %d is forbidden\n", state, idx);
        ListDump(lst);
        return state;
    }
    #endif

    int id = ListAllocNode(lst);

    if (id == none)
    {
        #ifdef DEBUG
        printf("ERROR %d, element %d haven't been pushed front, because capacity = %d\nList: ", LIST_OVERFLOW, value, lst->capacity);
        ListPrint(lst);
        printf("\n");
        #endif
        return LIST_OVERFLOW;
    }

    lst->data[id] = value;
    int right = lst->next[idx];
    int left = idx;
    ListLink(lst, id, left, right);
    CHECK();
    ListDumpDot(lst, "dot.txt", "InsertAfter", idx, value);

    return OK;
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
            printf("ERROR %d; no index %d\n", INVALID_INDEX, idx);
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
    ListDumpDot(lst, "dot.txt", "Erase", none, idx);
}

void ListFreeNode(List_t* lst, int idx)
{
    int state = OK;
    CHECK();

    lst->next[idx] = lst->free_head;
    lst->prev[idx] = none;
    lst->free_head = idx;

    lst->size -= 1;
    lst->dump_count += 1;

    CHECK();
}

int ListAllocNode(List_t* lst)
{
    int state = OK;
    CHECK();

    if (lst->free_head == none) return none;
    int idx = lst->free_head;
    lst->free_head = lst->next[idx];

    lst->size += 1;
    lst->dump_count += 1;

    CHECK();
    return idx;
}

int CtorVerify(List_t* lst)
{
    if (lst == NULL) return SEG_FAULT_LIST;
    if (lst->data == NULL && lst->next == NULL && lst->capacity == 0 && lst->size == 0 && lst->dump_count == 0) return OK;
    return CALL_CONSTRUCTOR_FOR_EXISTING_LIST;
}

int ListVerify(List_t* lst)
{
    if (lst == NULL) return SEG_FAULT_LIST;
    if (lst->data == NULL) return SEG_FAULT_DATA;
    if (lst->next == NULL) return SEG_FAULT_NEXT;
    if (lst->prev == NULL) return SEG_FAULT_PREV;

    if (lst->data[0] != poizon) return CANARY_DAMAGED;

    int i = lst->next[0];
    int counter = 0;
    while (i != 0)
    {
        if (i != lst->prev[lst->next[i]]) return CONNECTION_FAILED;
        i = lst->next[i];
        if (++counter > lst->capacity) return LOOP_LIST;
    }

    return OK;
}

int ListPushFront(List_t* lst, int value)
{
    int state = OK;
    CHECK();
    #ifndef DEBUG
    return ListInsertBefore(lst, lst->next[0], value);
    #else
    #undef DEBUG
    state = ListInsertBefore(lst, lst->next[0], value);
    #define DEBUG
    return state;
    #endif
}

int ListPushBack(List_t* lst, int value)
{
    int state = OK;
    CHECK();
    #ifndef DEBUG
    return ListInsertAfter(lst, lst->prev[0], value);
    #else
    #undef DEBUG
    state = ListInsertAfter(lst, lst->prev[0], value);
    #define DEBUG
    return state;
    #endif
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
    if (lst->data == NULL)
    {
        printf("ERROR %d\n", SEG_FAULT_DATA);
        exit(-1);
    }

    lst->next = calloc(capacity + 1, sizeof(int));
    if (lst->next == NULL)
    {
        printf("ERROR %d\n", SEG_FAULT_NEXT);
        exit(-1);
    }
    lst->prev = calloc(capacity + 1, sizeof(int));
    if (lst->prev == NULL)
    {
        printf("ERROR %d\n", SEG_FAULT_PREV);
        exit(-1);
    }

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
    lst->next[capacity] = none;

    CHECK();

    ListDumpDot(lst, "dot.txt", "Create", none, capacity);
    return state;
}

void ListDumpDot(const List_t* lst, const char *filename, const char* func, int idx, int value)
{
    FILE* dump = fopen(filename, "w");
    if (dump == NULL) return;

    fprintf(dump, "digraph G {\n");
    fprintf(dump, " rankdir=LR;\n");
    fprintf(dump, " splines=true;\n");
    fprintf(dump, " nodesep=0.6;\n");
    fprintf(dump, " ranksep=1.5;\n");
    fprintf(dump, " node [shape=none, margin=0, fontname=\"Helvetica\"];\n");
    fprintf(dump, " edge [fontname=\"Helvetica\", fontsize=12];\n");

    fprintf(dump, " anchor [shape=point, style=invis, width=0];\n");
    fprintf(dump, " { rank=same; anchor; ");
    fprintf(dump, " }\n");

    for (int curr = 0; curr <= lst->capacity; ++curr)
    {
        int free_cell = (curr != 0 && lst->prev[curr] == none);

        const char *color = "#add8e6";//light blue
        if (curr == 0) color = "gold";
        else if (free_cell) color = "green";//grey
        else if (curr == lst->next[0]) color = "orange";
        else if (curr == lst->prev[0]) color = "violet";

        fprintf(dump, " node%d [label=<\n", curr);
        fprintf(dump, " <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\" bgcolor=\"%s\">\n", color);
        fprintf(dump, " <tr><td colspan=\"2\"><b>INDEX = %d</b></td></tr>\n", curr);

        if (curr == 0) fprintf(dump, " <tr><td colspan=\"2\">SENTINEL</td></tr>\n");
        else if (free_cell) fprintf(dump, " <tr><td colspan=\"2\">FREE</td></tr>\n");
        else fprintf(dump, " <tr><td colspan=\"2\">DATA = %d</td></tr>\n", lst->data[curr]);

        if (!free_cell) fprintf(dump, " <tr><td>NEXT = %d</td><td>PREV = %d</td></tr>\n", lst->next[curr], lst->prev[curr]);
        else fprintf(dump, " <tr><td colspan=\"2\">NEXT = %d</td></tr>\n", lst->next[curr]);

        fprintf(dump, " </table>\n");
        fprintf(dump, " >];\n");
    }


    for (int i = 0; i <= lst->capacity; i++)
    {
        fprintf(dump, " anchor -> node%d [style=invis];\n", i);
        if (i > 0) fprintf(dump, " node%d -> node%d [style=invis, weight=100];\n", i - 1, i);
    }

    fprintf(dump, " node%d:n -> node%d:n [color=blue, constraint=false, penwidth=1.5, label=\"HEAD\", fontcolor=blue, fontsize=17];\n", 0, lst->next[0]);
    fprintf(dump, " node%d:s -> node%d:s [color=red, constraint=false, penwidth=1.5, label=\"TAIL\", fontcolor=red, fontsize=17];\n", 0, lst->prev[0]);

    for (int i = 1; i <= lst->capacity; i++)
    {
        if (lst->prev[i] != none)
        {
            if (lst->next[i] != zero) fprintf(dump, " node%d:n -> node%d:n [color=blue, constraint=false, penwidth=1.5];\n", i, lst->next[i]);
            if (lst->prev[i] != zero) fprintf(dump, " node%d:s -> node%d:s [color=red, constraint=false, penwidth=1.5];\n", i, lst->prev[i]);
        }
        else if (lst->next[i] != none) fprintf(dump, " node%d -> node%d [color=green, style=dashed, constraint=false];\n", i, lst->next[i]);
    }

    fprintf(dump, "}\n");
    fclose(dump);

    char cmd[256];

    const char* dot_path = "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\"";
    snprintf(cmd, sizeof(cmd), "%s -Tpng dot.txt -o Dump_%d.png", dot_path, lst->dump_count);

    int result = system(cmd);

    if (result == 0) printf("Graph has been compiled into Dump_%d.png correctly\n", lst->dump_count);
    else printf("ERROR: %d; Graph hasn't been compiled into Dump.png correctly\n", ERROR_COMPILING_GRAPHVIZ);

    FILE *html = fopen("dumps.html", "a");
    if (html != NULL)
    {
        if (lst->dump_count == 1)
        {
            fprintf(html, "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n");
            fprintf(html, "<title>Dump story</title>\n");
            fprintf(html, "<style>\n");
            fprintf(html, " body { font-family: Arial, sans-serif; background: #f4f4f4; padding: 20px; }\n");
            fprintf(html, " .dump { background: white; padding: 20px; margin-bottom: 30px; border-radius: 8px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }\n");
            fprintf(html, " h2 { color: #333; border-bottom: 2px solid #ccc; padding-bottom: 5px; }\n");
            fprintf(html, " img { max-width: 100%%; height: auto; }\n");
            fprintf(html, "</style>\n</head>\n<body>\n");
        }

        fprintf(html, "<div class=\"dump\">\n");
        if (idx != none) fprintf(html, " <h2>Call #%d\n%s(%d, %d)</h2>\n", lst->dump_count, func, idx, value);
        else fprintf(html, " <h2>Call #%d\n%s(%d)</h2>\n", lst->dump_count, func, value);

        fprintf(html, " <img src=\"Dump_%d.png\" alt=\"Dump_%d\">\n", lst->dump_count, lst->dump_count);
        fprintf(html, "</div>\n");

        fclose(html);
    }
}

#endif
