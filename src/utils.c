#include <stdint.h>
#include <inttypes.h>

#include "utils.h"
#include "config.h"

#define MLIB_COPY_UNTIL_LEN_REMAIN(baseType, source, destination, remainingLength) do   \
        {                                                                               \
            while (remainingLength >= sizeof(baseType))                                 \
            {                                                                           \
                *(baseType *)destination = *(baseType *)source;                         \
                (baseType *)destination++;                                              \
                (baseType *)source++;                                                   \
                remainingLength -= sizeof(baseType);                                    \
            }                                                                           \
            if (remainingLength == 0u) return;                                          \
        } while (0u)

//~ #define MLIB_COPY_IF_EQ_LEN_REMAIN(baseType, source, destination, remainingLength) do   \
        //~ {                                                                               \
            //~ while (remainingLength >= sizeof(baseType))                                 \
            //~ {                                                                           \
                //~ if (*(baseType *)destination != *(baseType *)source)                    \
                    //~ *(baseType *)destination = *(baseType *)source;                     \
                //~ (baseType *)destination++;                                              \
                //~ (baseType *)source++;                                                   \
                //~ remainingLength -= sizeof(baseType);                                    \
            //~ }                                                                           \
            //~ if (remainingLength == 0u) return;                                          \
        //~ } while (0u)
//~ 
//~ #define MLIB_CHECK_EQ_UNTIL_LEN_REMAIN(baseType, source, destination, remainingLength) do   \
        //~ {                                                                                   \
            //~ while (remainingLength >= sizeof(baseType))                                     \
            //~ {                                                                               \
                //~ if (*(baseType *)destination != *(baseType *)source) return FALSE;          \
                //~ (baseType *)destination++;                                                  \
                //~ (baseType *)source++;                                                       \
                //~ remainingLength -= sizeof(baseType);                                        \
            //~ }                                                                               \
            //~ if (remainingLength == 0u) return TRUE;                                         \
        //~ } while (0u)

void mLibUtils_Copy(void * src, void * dst, size_t len)
{
    MLIB_COPY_UNTIL_LEN_REMAIN(uint128_t, src, dst, len);
    MLIB_COPY_UNTIL_LEN_REMAIN(uint64_t,  src, dst, len);
    MLIB_COPY_UNTIL_LEN_REMAIN(uint32_t,  src, dst, len);
    MLIB_COPY_UNTIL_LEN_REMAIN(uint16_t,  src, dst, len);
    MLIB_COPY_UNTIL_LEN_REMAIN(uint8_t,   src, dst, len);
}

//~ bool mLibUtils_CopyCondition_Equal (void * first, void * second, size_t size)
//~ {
    //~ MLIB_CHECK_EQ_UNTIL_LEN_REMAIN(uint128_t, src, dst, len);
    //~ MLIB_CHECK_EQ_UNTIL_LEN_REMAIN(uint64_t,  src, dst, len);
    //~ MLIB_CHECK_EQ_UNTIL_LEN_REMAIN(uint32_t,  src, dst, len);
    //~ MLIB_CHECK_EQ_UNTIL_LEN_REMAIN(uint16_t,  src, dst, len);
    //~ MLIB_CHECK_EQ_UNTIL_LEN_REMAIN(uint8_t,   src, dst, len);
//~ }
//~ 
//~ void mLibUtils_CopyWithCondition (void * src, void * dst, size_t len, bool (*condition) (void * first, void * second, size_t size))
//~ {
    //~ 
//~ }
