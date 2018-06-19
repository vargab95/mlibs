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

typedef struct {
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
} mLibVersion_t;

typedef struct {
	listConfigLanguage_t language;
	listConfigMemoryType_t memType;
} mLibListConfig_t;

typedef struct {
	mLibVersion_t version;
	mLibListConfig_t listConfig;
} mLibConfig_t;

#define MLIB_VERSION { 1u, 0u, 0u }
#define MLIB_LIST_CONF_LANG_USED LIST_CONF_LANG_C
#define MLIB_LIST_CONF_MEM_TYPE LIST_CONF_MEM_STATIC
#define MLIB_LIST_TYPE LIST_CONF_BIDIRECTIONAL

#endif /* LIB_CONFIG_H_ */
