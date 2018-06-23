#ifndef __LIST_H__
#define __LIST_H__

#include "config.h"

#include <stdlib.h>

typedef struct mLibListElement_t {
    void * data;
    size_t size;
    
#if MLIB_LIST_SEARCH_BY_HASH == LIST_CONF_HASHING_ENABLED
    uint32_t hash;
#endif
    
#if MLIB_LIST_TYPE == LIST_CONF_BIDIRECTIONAL
    mLibListElement_t * prev;
#endif
    mLibListElement_t * next;
} mLibListElement_t;

typedef struct {
    mLibListElement_t * begin;
    mLibListElement_t * end;
    size_t elementCount;
} mLibList_t;

mLibList_t mLibList_GetDefault(void);
void mLibList_Add(mLibList_t * const list, void const * const data, size_t size);
void mLibList_PopByValue(mLibList_t * const list, void const * const data, size_t size);
void mLibList_PopOneById(mLibList_t * const list, size_t id);
void mLibList_PopById(mLibList_t * const list, size_t id_start, size_t id_stop);

#endif
