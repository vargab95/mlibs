/*
 * list.h
 *
 *  Created on: 19 Jun 2018
 *      Author: vargab95
 */

#ifndef LIB_LIST_H_
#define LIB_LIST_H_

typedef struct mLibListElement_t {
	void * data;
	size_t size;
} mLibListElement_t;

typedef struct {
	mLibListElement_t * items;
	size_t elementCount;
} mLibList_t;

extern void mLibList_Add (mLibList_t * const list, void const * const data, size_t const size);

#endif /* LIB_LIST_H_ */
