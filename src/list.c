#include "list.h"

mLibList_t mLibList_GetDefault(void)
{
    return (mLibList_t) {
        .begin = 0u,
        .end = 0u,
        .elementCount = 0u
    };
}
    
void mLibList_AddToEnd(mLibList_t * const list, void const * const data, size_t size)
{
    void * newDataInList;
    mLibListElement_t * newListElement;
    
    ++(list->elementCount);
    
    newListElement = malloc(sizeof(mLibListElement_t));
    newDataInList = malloc(size);
    mLibUtils_Copy(data, newDataInList, size);
    
    if ((list->begin == 0u) && (list->end == 0u))
    {
        list->begin = list->end = newListElement;
        list->begin->next = 0u;

#if MLIB_LIST_TYPE == LIST_CONF_BIDIRECTIONAL
        list->begin->prev = 0u;
#endif
    }
    else
    {
        list->end = newListElement;
        list->begin->next = 0u;

#if MLIB_LIST_TYPE == LIST_CONF_BIDIRECTIONAL
        list->begin->prev = 0u;
#endif
    }
}

void mLibList_PopByValue(mLibList_t * const list, void const * const data, size_t size)
{
    
}

void mLibList_PopOneById(mLibList_t * const list, size_t id)
{
    
}

void mLibList_PopById(mLibList_t * const list, size_t id_start, size_t id_stop)
{
    
}
