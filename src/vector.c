/*
 * vector.c
 *
 *  Created on: 19 Jun 2018
 *      Author: vargab95
 */

#include <stdlib.h>

#include "config.h"
#include "vector.h"
#include "utils.h"

void mLibVector_Add (mLibVector_t * const vector, void const * const data, size_t const size)
{
    ++(vector->elementCount);

    if (vector->elementCount == 1u)
    {
        vector->items = malloc(sizeof(mLibVectorElement_t));
    }
    else
    {
        vector->items = realloc(vector->items, sizeof(mLibVectorElement_t) * vector->elementCount);
    }
    
    vector->items[vector->elementCount - 1u].data = malloc(size);
    vector->items[vector->elementCount - 1u].size = size;
    
    mLibUtils_Copy(data, (void *)(vector->items[vector->elementCount - 1u].data), size);
}

void mLibVector_AddFront (mLibVector_t * const vector, void const * const data, size_t const size)
{
    
}

void mLibVector_AddBack (mLibVector_t * const vector, void const * const data, size_t const size)
{
    ++(vector->elementCount);
    
    vector->items = realloc(vector->items, sizeof(mLibVectorElement_t) * vector->elementCount);
    
    vector->items[vector->elementCount - 1u].data = malloc(size);
    vector->items[vector->elementCount - 1u].size = size;
    
    mLibUtils_Copy(data, vector->items[vector->elementCount - 1u].data, size);
}

void mLibVector_Pop (mLibVector_t * const vector, size_t const id_start, size_t const id_stop)
{
    mLibVectorElement_t * oldItemvector = vector->items;
    size_t oldSize = vector->elementCount;
    size_t looper;
    
    vector->elementCount = vector->elementCount - (id_stop - id_start) - 1u;
    vector->items = malloc(sizeof(mLibVectorElement_t) * vector->elementCount);
    
    for (looper = id_start; looper <= id_stop; looper++)
        free(oldItemvector[looper].data);
    
    for (looper = 0u; looper < id_start; looper++)
        vector->items[looper] = oldItemvector[looper];
    
    if (id_stop < oldSize)
        for (looper = id_stop + 1u; looper < oldSize; looper++)
            vector->items[(looper - id_stop - 1u) + id_start] = oldItemvector[looper];
    
    free(oldItemvector);
}

void mLibVector_PopOne (mLibVector_t * const vector, size_t const id)
{
    mLibVectorElement_t * oldItemvector = vector->items;
    size_t oldSize = vector->elementCount;
    size_t looper;
    
    --(vector->elementCount);
    vector->items = malloc(sizeof(mLibVectorElement_t) * vector->elementCount);
    
    for (looper = 0u; looper < id; looper++)
        vector->items[looper] = oldItemvector[looper];
    
    while(looper < vector->elementCount)
    {
        vector->items[looper] = oldItemvector[looper + 1u];
        ++looper;
    }
    
    free(oldItemvector[id].data);
    free(oldItemvector);
}

void mLibVector_PopFirst (mLibVector_t * const vector)
{
    mLibVectorElement_t * oldItemvector = vector->items;
    
    --(vector->elementCount);
    vector->items = malloc(sizeof(mLibVectorElement_t) * vector->elementCount);
    
    for (size_t looper = 0u; looper < vector->elementCount; looper++)
        vector->items[looper] = oldItemvector[looper + 1u];
    
    free(oldItemvector[0u].data);
    free(oldItemvector);
}

void mLibVector_PopLast (mLibVector_t * const vector)
{
    --(vector->elementCount);
    vector->items = realloc(vector->items, sizeof(mLibVectorElement_t) * vector->elementCount);
}

mLibVector_t mLibVector_GetSubList (mLibVector_t * const vector, size_t const id_start, size_t const id_stop)
{
    return (mLibVector_t) {
        .items = &(vector->items[id_start]),
        .elementCount = vector->elementCount - (vector->elementCount - (id_stop - id_start)) + 1u
    };
}
