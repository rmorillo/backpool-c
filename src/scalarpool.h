#ifndef SCALARPOOL_H_INCLUDED
#define SCALARPOOL_H_INCLUDED
#include <stdlib.h>
#include "backpool.h"
#define POOL_TYPE_NAME(x) ScalarPool ## _ ## x
#include "poolcommon.h"

BackPool* POOL_TYPE_NAME(new)(int capacity, POOL_ITEM_TYPE initValue)
{
    BackPool* pool = malloc(sizeof(struct BackPool));

    POOL_ITEM_TYPE* poolItems= malloc(capacity * sizeof(POOL_ITEM_TYPE));

    int i;
    for (i = 0; i < capacity; i++) {
        poolItems[i] = initValue;
    }

    pool->subscribers = NULL;
    pool->subscriberSize = 0;
    pool->defaultValue = initValue;
    pool->items= poolItems;
    pool->lastPosition= -1;
    pool->currentPosition= 0;
    pool->length= 0;
    pool->capacity = capacity;
    pool->offset = 1;
    pool->sequence = 0;

    return pool;
}

void POOL_TYPE_NAME(assign)(BackPool* pool, POOL_ITEM_TYPE value)
{
    pool->items[pool->currentPosition] = value;

    POOL_TYPE_NAME(moveForward)(pool);

    if (pool->subscribers!=NULL)
    {
        int i;
        for(i = 0; i < pool->subscriberSize; i++) {
            pool->subscribers[i](pool->items[pool->lastPosition]);
        }
    }

}

#endif
