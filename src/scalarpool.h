#ifndef SCALARPOOL_H_INCLUDED
#define SCALARPOOL_H_INCLUDED
#include <stdlib.h>
#include "backpool.h"

BackPool* ScalarPool_new(int capacity, POOL_ITEM_TYPE initValue)
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

void ScalarPool_moveForward(BackPool* pool)
{
    pool->lastPosition = pool->currentPosition;
    pool->sequence++;

    if (!pool->hasRolledOver)
    {
        pool->length++;
    }
    if (pool->currentPosition < (pool->capacity -1))
    {
        pool->currentPosition += pool->offset;
    }
    else
    {
        pool->currentPosition = 0;
        pool->hasRolledOver = true;
    }
}

void ScalarPool_assign(BackPool* pool, POOL_ITEM_TYPE value)
{
    pool->items[pool->currentPosition] = value;

    ScalarPool_moveForward(pool);

    if (pool->subscribers!=NULL)
    {
        int i;
        for(i = 0; i < pool->subscriberSize; i++) {
            pool->subscribers[i](pool->items[pool->lastPosition]);
        }
    }

}

int ScalarPool_getAbsoluteIndex(BackPool* pool, int lookBehindIndex)
{
    int targetIndex = pool->lastPosition - lookBehindIndex;
    if (targetIndex < 0)
    {
        int absoluteIndex = targetIndex + pool->capacity;
        if (pool->hasRolledOver && absoluteIndex > pool->lastPosition)
        {
            targetIndex = absoluteIndex;
        }
        else
        {
            targetIndex = -1; //Pool index is out of range.
        }
    }

    return targetIndex;
}

POOL_ITEM_TYPE ScalarPool_item(BackPool* pool, int lookBehindIndex)
{
    int absoluteIndex= ScalarPool_getAbsoluteIndex(pool, lookBehindIndex);
    if (absoluteIndex < 0)
    {
        printf("Pool index is out of range.  You can only use index values from 0 to %i\n", pool->length - 1);
        return pool->defaultValue;
    }
    else
    {
        return pool->items[absoluteIndex];
    }
}

void ScalarPool_subscribe(BackPool* pool, void (*poolChange)(POOL_ITEM_TYPE poolItem))
{
    pool->subscriberSize++;
    pool->subscribers= realloc(pool->subscribers, pool->subscriberSize * sizeof(void*));
    pool->subscribers[pool->subscriberSize-1] = poolChange;

}

POOL_ITEM_TYPE ScalarPool_current(BackPool* pool)
{
    return pool->items[pool->lastPosition];
}

POOL_ITEM_TYPE ScalarPool_previous(BackPool* pool)
{
    return pool->items[pool->lastPosition - 1];
}

#endif
