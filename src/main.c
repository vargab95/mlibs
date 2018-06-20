/*
 * main.c
 *
 *  Created on: 18 Jun 2018
 *      Author: vargab95
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "list.h"
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
    mLibList_t myList;
    
    myList.elementCount = 0u;
    
    uint32_t data = 53u;
    
    for (uint16_t asd = 0u; asd < 10u; asd++)
    {
        data += 100u;
        mLibList_Add (&myList, &data, sizeof(uint32_t));
    }
    
    for (uint16_t looper = 0u; looper < myList.elementCount; looper++)
    {
        printf("%d\t%d\n", looper, *(uint32_t *)(myList.items[looper].data));
    }
    
    mLibList_Pop(&myList, 3, 6);
    
    puts("\n\n");
    printf("%d\n", myList.elementCount);
    
    for (uint16_t looper = 0u; looper < myList.elementCount; looper++)
    {
        printf("%d\t%d\n", looper, *(uint32_t *)(myList.items[looper].data));
    }
}

