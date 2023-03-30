#include <stdio.h>

#include "m_hash.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#else
#include "../../m_common/api/m_common.h"
#endif

int main(void)
{
    m_com_sized_data_t data = {"asd", 3};

    printf("0x%x\n", m_hash_djb(&data));
    printf("0x%x\n", m_hash_fnv1a(&data));
    return 0;
}
