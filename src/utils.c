#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "utils.h"
#include "config.h"

#define MLIB_COPY_UNTIL_LEN_REMAIN(baseType, source, destination, remainingLength)      \
        do {                                                                            \
            while (remainingLength >= sizeof(baseType))                                 \
            {                                                                           \
                *(baseType *)destination = *(baseType *)source;                         \
                (baseType *)destination++;                                              \
                (baseType *)source++;                                                   \
                remainingLength -= sizeof(baseType);                                    \
            }                                                                           \
            if (remainingLength == 0u) return;                                          \
        } while (0u)

void mLibUtils_Copy(void const * src, void * dst, size_t len)
{
    MLIB_COPY_UNTIL_LEN_REMAIN(uint128_t, src, dst, len);
    MLIB_COPY_UNTIL_LEN_REMAIN(uint64_t,  src, dst, len);
    MLIB_COPY_UNTIL_LEN_REMAIN(uint32_t,  src, dst, len);
    MLIB_COPY_UNTIL_LEN_REMAIN(uint16_t,  src, dst, len);
    MLIB_COPY_UNTIL_LEN_REMAIN(uint8_t,   src, dst, len);
}

uint32_t mLibUtils_32bitHash(void const * const data, size_t size)
{
    size_t i = 0u;
    uint32_t hash = 0u;
    
    while (i != size)
    {
        hash += ((uint8_t *)data)[i++];
        hash += hash << 10u;
        hash ^= hash >> 6u;
    }
    
    hash += hash << 3u;
    hash ^= hash >> 11u;
    hash += hash << 15u;
    return hash;
}

void mLibUtils_Print_u128(uint128_t u128)
{
    return;
}
