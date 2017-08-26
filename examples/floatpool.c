#include "floatpool.h"
#include <stdlib.h>
#include <math.h>

POOL_ITEM_TYPE* POOL_ITEM_TYPE_PREFIX(new)(float value)
{
    POOL_ITEM_TYPE* item = malloc(sizeof(struct POOL_ITEM_TYPE));

    POOL_ITEM_TYPE_PREFIX(update)(item, value);

    return item;
}

void POOL_ITEM_TYPE_PREFIX(update)(POOL_ITEM_TYPE* poolItem, float value)
{
    poolItem->value= value;
}

Pool* POOL_TYPE_PREFIX(new)(int capacity)
{
    return Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return POOL_ITEM_TYPE_PREFIX(new)(NAN);}));
}

void POOL_TYPE_PREFIX(update)(Pool* pool, float value)
{
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { POOL_ITEM_TYPE_PREFIX(update)(poolItem, value);}));
}

#include "poolcommon.c"
