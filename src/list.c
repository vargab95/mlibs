/*
 * list.c
 *
 *  Created on: 19 Jun 2018
 *      Author: vargab95
 */

#include <stdlib.h>

#include "config.h"
#include "list.h"
#include "utils.h"

void mLibList_Add (mLibList_t * const list, void const * const data, size_t const size)
{
    ++(list->elementCount);

    if (list->elementCount == 1u)
    {
        list->items = malloc(sizeof(mLibListElement_t));
    }
    else
    {
        list->items = realloc(list->items, sizeof(mLibListElement_t) * list->elementCount);
    }
    
    list->items[list->elementCount - 1u].data = malloc(size);
    list->items[list->elementCount - 1u].size = size;
    
    mLibUtils_Copy(data, list->items[list->elementCount - 1u].data, size);
}

void mLibList_Add_Ordered (mLibList_t * const list, void const * const data, size_t const size, uint8_t (*func)(void * const))
{
    
}

void mLibList_Pop (mLibList_t * const list, size_t const id_start, size_t const id_stop)
{
    mLibListElement_t * oldItemList = list->items;
    size_t oldSize = list->elementCount;
    size_t looper;
    
    list->elementCount = list->elementCount - (id_stop - id_start);
    list->items = malloc(sizeof(mLibListElement_t) * list->elementCount);
    
    for (looper = id_start; looper <= id_stop; looper++)
    {
        free(oldItemList[looper].data);
    }
    
    for (looper = 0u; looper < id_start; looper++)
    {
        list->items[looper] = oldItemList[looper];
    }
    
    if (id_stop < oldSize)
    {
        for (looper = id_stop + 1u; looper < oldSize; looper++)
        {
            list->items[(looper - id_stop) + id_start] = oldItemList[looper];
        }
    }
    
    free(oldItemList);
}

void mLibList_PopOne (mLibList_t * const list, size_t const id)
{
    
}
