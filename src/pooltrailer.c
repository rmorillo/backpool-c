#include <stdio.h>
#include <stdlib.h>
#include "pooltrailer.h"

PoolTrailer* POOL_TRAILER_TYPE_NAME(new)(POOL_NAME* pool)
{
    PoolTrailer* trailer = malloc(sizeof(struct PoolTrailer));

    trailer->backPool = pool;
    trailer->sequence = pool->sequence;
    trailer->lastPosition = pool->lastPosition;
    trailer->currentPosition = pool->currentPosition;

    return trailer;
}

POOL_ITEM_PTR POOL_TRAILER_TYPE_NAME(current)(PoolTrailer* trailer)
{
    if (POOL_TRAILER_TYPE_NAME(hasExpired)(trailer, 0)) {
        printf("Trailer has expired!");
        return trailer->backPool->defaultValue;
    }
    else
    {
        return trailer->backPool->items[trailer->lastPosition];
    }
}

POOL_ITEM_PTR POOL_TRAILER_TYPE_NAME(previous)(PoolTrailer* trailer)
{
    return trailer->backPool->items[POOL_TRAILER_TYPE_NAME(getAbsoluteIndex)(trailer, 1)];
}

bool POOL_TRAILER_TYPE_NAME(isStale)(PoolTrailer* trailer)
{
    return trailer->backPool->sequence > trailer->sequence;
}

int POOL_TRAILER_TYPE_NAME(trailingIndex)(PoolTrailer* trailer)
{
       int diff = trailer->backPool->currentPosition - trailer->sequence;
       if (diff < 0){
            return diff + trailer->backPool->length;
       }
       else {
            return diff;
       }
}

int POOL_TRAILER_TYPE_NAME(getAbsoluteIndex)(PoolTrailer* trailer, int trailingIndex)
{
    int targetIndex = trailer->lastPosition - (trailingIndex * trailer->backPool->offset);
    if (targetIndex < 0)
    {
        int absoluteIndex = targetIndex + trailer->backPool->capacity;
        if (trailer->backPool->hasRolledOver && absoluteIndex > trailer->lastPosition)
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

bool POOL_TRAILER_TYPE_NAME(hasExpired)(PoolTrailer* trailer, int index)
{
    return ((trailer->backPool->sequence / trailer->backPool->length)>=2) || (POOL_TRAILER_TYPE_NAME(trailingIndex)(trailer) + index) > trailer->backPool->length;
}

bool POOL_TRAILER_TYPE_NAME(keepUp)(PoolTrailer* trailer)
{
    if (trailer->sequence<trailer->backPool->sequence)
    {
        POOL_TRAILER_TYPE_NAME(moveForward)(trailer);
        return true;
    }
    else
    {

        return false;
    }
}

void POOL_TRAILER_TYPE_NAME(moveForward)(PoolTrailer* trailer)
{
    trailer->lastPosition = trailer->currentPosition;
    trailer->sequence++;

    if (trailer->currentPosition < ((trailer->backPool->capacity * trailer->backPool->offset) - trailer->backPool->offset))
    {
        trailer->currentPosition += trailer->backPool->offset;
    }
    else
    {
        trailer->currentPosition = 0;
    }
}

void POOL_TRAILER_TYPE_NAME(refresh)(PoolTrailer* trailer)
{
    trailer->sequence = trailer->backPool->sequence;
    trailer->lastPosition = trailer->backPool->lastPosition;
    trailer->currentPosition = trailer->backPool->currentPosition;
}
