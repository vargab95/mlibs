/*
 * config.h
 *
 *  Created on: 19 Jun 2018
 *      Author: vargab95
 */

#ifndef LIB_CONFIG_H_
#define LIB_CONFIG_H_

#include <stdint.h>

typedef enum {
	LIST_CONF_LANG_C,
	LIST_CONF_LANG_CPP
} listConfigLanguage_t;

typedef enum {
	LIST_CONF_MEM_STATIC,
	LIST_CONF_MEM_DYNAMIC
} listConfigMemoryType_t;

typedef enum {
	LIST_CONF_ONE_DIRECTIONAL,
	LIST_CONF_BIDIRECTIONAL
} listConfigDirectionType_t;

typedef enum {
    LIST_CONF_HASHING_ENABLED,
    LIST_CONF_HASHING_DISABLED
} listConfigHashingEnabled;

typedef struct {
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
} mLibVersion_t;

typedef struct {
	listConfigLanguage_t language;
	listConfigMemoryType_t memType;
} mLibVectorConfig_t;

typedef struct {
	mLibVersion_t version;
	mLibVectorConfig_t vectorConfig;
} mLibConfig_t;

#define MLIB_VERSION { 1u, 0u, 0u }
#define MLIB_LIST_CONF_LANG_USED LIST_CONF_LANG_C
#define MLIB_LIST_CONF_MEM_TYPE LIST_CONF_MEM_STATIC
#define MLIB_LIST_TYPE LIST_CONF_BIDIRECTIONAL
#define MLIB_LIST_SEARCH_BY_HASH LIST_CONF_HASHING_ENABLED

#endif /* LIB_CONFIG_H_ */
