#ifndef POOLCOMMON_H_INCLUDED
#define POOLCOMMON_H_INCLUDED
#ifndef POOL_TYPE_NAME
#define POOL_TYPE_NAME(x) BackPool ## _ ## x
#endif // POOL_TYPE_NAME

void POOL_TYPE_NAME(moveForward)(BackPool* pool)
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

int POOL_TYPE_NAME(getAbsoluteIndex)(BackPool* pool, int lookBehindIndex)
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

POOL_ITEM_TYPE POOL_TYPE_NAME(item)(BackPool* pool, int lookBehindIndex)
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

void POOL_TYPE_NAME(subscribe)(BackPool* pool, void (*poolChange)(POOL_ITEM_TYPE poolItem))
{
    pool->subscriberSize++;
    pool->subscribers= realloc(pool->subscribers, pool->subscriberSize * sizeof(void*));
    pool->subscribers[pool->subscriberSize-1] = poolChange;

}

POOL_ITEM_TYPE POOL_TYPE_NAME(current)(BackPool* pool)
{
    return pool->items[pool->lastPosition];
}

POOL_ITEM_TYPE POOL_TYPE_NAME(previous)(BackPool* pool)
{
    return pool->items[pool->lastPosition - 1];
}

#endif // POOLCOMMON_H_INCLUDED
