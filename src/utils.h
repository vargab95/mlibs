/*
 * utils.h
 *
 *  Created on: 19 Jun 2018
 *      Author: vargab95
 */

#ifndef LIB_UTILS_H_
#define LIB_UTILS_H_

#include <stdlib.h>

typedef __uint128_t uint128_t;
typedef unsigned char bool;

extern void mLibUtils_Copy(void * src, void * dst, size_t len);
extern void mLibUtils_CopyWithCondition(void * src, void * dst, size_t len, bool (*condition) (void * first, void * second, size_t size));
extern bool mLibUtils_CopyCondition_Equal (void * first, void * second, size_t size);

#endif /* LIB_LIST_H_ */
