#include <stdio.h>
#include <stdlib.h>
#include "List.h"

int main()
{
    List_t lst = {};
    List_t* list_ptr = &lst;

    CreateList(list_ptr, 10);

    ListPushBack(list_ptr, 8);
    ListPushFront(list_ptr, 9);
    ListPushBack(list_ptr, 10);
    ListPushBack(list_ptr, 7);
    ListPushFront(list_ptr, 13);
    ListPushBack(list_ptr, 15);

    int idx = ListFindIndex(list_ptr, 5);
    ListErase(list_ptr, idx);

    idx = ListFind(list_ptr, 8);
    ListInsertAfter(list_ptr, idx, 7);

    idx = ListFindIndex(list_ptr, 6);
    ListInsertAfter(list_ptr, idx, 20);

    ListPopBack(list_ptr);

    idx = ListFind(list_ptr, 15);
    ListInsertBefore(list_ptr, idx, 12);

    idx = ListFindIndex(list_ptr, 1);
    ListInsertBefore(list_ptr, idx, 5);

    ListPopBack(list_ptr);
    ListPopFront(list_ptr);

    for (int i = 0; i < 10; ++i)
    {
        ListPushBack(list_ptr, 67);
    }
    //printf("%d\n", lst.data[lst.head]);

    ListPrint(list_ptr);

    return 0;
}
