//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

int main()
{
    List_t lst = {};
    List_t* list_ptr = &lst;

    CreateList(list_ptr, 10);

    ListPushBack(list_ptr, 7);
    ListPushBack(list_ptr, 6);

    ListPushFront(list_ptr, 8);
    int idx = ListFind(list_ptr, 7);
    ListInsertAfter(list_ptr, idx, 13);
    ListInsertAfter(list_ptr, idx, 17);

    idx = ListFindIndex(list_ptr, 1);
    ListErase(list_ptr, idx);

    ListDump(list_ptr);
    ListDumpDot(list_ptr, "dot.txt");
    ListFree(list_ptr);

    return 0;
}
