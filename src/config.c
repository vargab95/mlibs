/*
 * config.c
 *
 *  Created on: 19 Jun 2018
 *      Author: vargab95
 */

#include "config.h"

mLibConfig_t MLibConfig_g = {
		.version = MLIB_VERSION,
		.vectorConfig = {
				.language = MLIB_vector_CONF_LANG_USED,
				.language = MLIB_vector_CONF_MEM_TYPE
		}
};
