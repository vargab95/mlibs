#ifndef __M_COMMON_H__
#define __M_COMMON_H__

#include <stdlib.h>

/**
 * @brief Common structure for sized objects
 * 
 */
typedef struct
{
    void *data;
    size_t size;
} m_com_sized_data_t;

#endif