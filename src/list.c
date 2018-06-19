/*
 * list.c
 *
 *  Created on: 19 Jun 2018
 *      Author: vargab95
 */

#include <stdlib.h>

#include "config.h"
#include "list.h"

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
    
    for (uint8_t looper = 0u; looper < size; looper++)
    {
        ((uint8_t *)list->items[list->elementCount - 1u].data)[looper] = ((uint8_t *)data)[looper];
    }
}

void mLibList_Add_Ordered (mLibList_t * const list, void const * const data, size_t const size, uint8_t (*func)(void * const))
{
    
}
