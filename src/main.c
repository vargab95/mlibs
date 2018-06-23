/*
 * main.c
 *
 *  Created on: 18 Jun 2018
 *      Author: vargab95
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "vector.h"
#include "utils.h"

static int print_u128_u(uint128_t u128)
{
    uint128_t divider = 1;
    char asd[1000] = {0};
    uint16_t looper = 0u;

    puts("AaaA");

    char asdasd;
    uint128_t divided_val = 0;
    uint128_t moduloead_val = 0;

    while (u128 / divider > 0)
    {
    	divided_val = u128 / divider;
    	moduloead_val = divided_val % 10;
    	asdasd = asd[999 - looper] = moduloead_val + '0';
        divider *= 10;
        looper++;
    }

    while(looper)
    {
        --looper;
        putchar(asd[999 - looper]);
    }

    putchar('\n');
    
    return 0;
}

uint128_t get_prd_128()
{
    uint128_t asd = 1u;
    uint32_t looper;

    for (looper = 0u; looper < 100u; looper++)
    {
        asd += looper;
    }

    return (uint128_t) 1.45234523e38;
}

int main(void)
{
    mLibVector_t myvector;
    
    myvector.elementCount = 0u;
    
    uint8_t data = 53u;
    
    for (uint16_t asd = 0u; asd < 10u; asd++)
    {
        mLibVector_Add (&myvector, &data, sizeof(uint8_t));
        data += 100u;
    }
    
    for (uint16_t looper = 0u; looper < myvector.elementCount; looper++)
    {
        printf("%03d\t%03d\t%x\n", looper, *(uint8_t *)(myvector.items[looper].data), mLibUtils_32bitHash((uint8_t *)(myvector.items[looper].data), 4u));
    }
    
    puts("\n\n");
    myvector = mLibVector_GetSubList(&myvector, 0, 9);
    
    for (uint16_t looper = 0u; looper < myvector.elementCount; looper++)
    {
        printf("%03d\t%03d\t%x\n", looper, *(uint8_t *)(myvector.items[looper].data), mLibUtils_32bitHash((uint32_t *)(myvector.items[looper].data), 4u));
    }
    
    //~ for (uint8_t elemLooper = 0u; elemLooper < 4u; elemLooper++)
    //~ {
        //~ mLibVector_PopFirst(&myvector);
        //~ puts("\n\n");
        //~ printf("%d\n", myvector.elementCount);
        //~ 
        //~ for (uint16_t looper = 0u; looper < myvector.elementCount; looper++)
        //~ {
            //~ printf("%03d\t%03d\t%x\n", looper, *(uint32_t *)(myvector.items[looper].data), mLibUtils_32bitHash((uint32_t *)(myvector.items[looper].data), 4u));
        //~ }
    //~ }
}

