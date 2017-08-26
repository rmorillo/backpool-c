#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "intpool.h"

POOL_ITEM_TYPE* POOL_ITEM_TYPE_PREFIX(new)(int value)
{
    POOL_ITEM_TYPE* item = malloc(sizeof(struct POOL_ITEM_TYPE));
    item->value = value;

    return item;
}

void POOL_ITEM_TYPE_PREFIX(update)(POOL_ITEM_TYPE* poolItem, int value)
{
    poolItem->value= value;
}

Pool* POOL_TYPE_PREFIX(new)(int capacity)
{
    return Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return POOL_ITEM_TYPE_PREFIX(new)(INT_MIN);}));
}

void POOL_TYPE_PREFIX(update)(Pool* pool, int value)
{
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { POOL_ITEM_TYPE_PREFIX(update)(poolItem, value);}));
}

#include "poolcommon.c"
