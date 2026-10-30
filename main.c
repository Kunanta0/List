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

    size_t idx = ListFindIndex(list_ptr, 6);
    ListErase(list_ptr, idx);
    //printf("%d\n", lst.data[lst.head]);

    ListPrint(list_ptr);

    return 0;
}
