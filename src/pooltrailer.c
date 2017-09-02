#include <stdio.h>
#include <stdlib.h>
#include "pooltrailer.h"

PoolTrailer* PoolTrailer_new(BackPool* pool)
{
    PoolTrailer* trailer = malloc(sizeof(struct PoolTrailer));

    trailer->backPool = pool;
    trailer->sequence = pool->sequence;
    trailer->lastPosition = pool->lastPosition;
    trailer->currentPosition = pool->currentPosition;

    return trailer;
}

POOL_ITEM_TYPE PoolTrailer_current(PoolTrailer* trailer)
{
    if (PoolTrailer_hasExpired(trailer, 0)) {
        printf("Trailer has expired!");
        return trailer->backPool->defaultValue;
    }
    else
    {
        return trailer->backPool->items[trailer->lastPosition];
    }
}

POOL_ITEM_TYPE PoolTrailer_previous(PoolTrailer* trailer)
{
    return trailer->backPool->items[PoolTrailer_getAbsoluteIndex(trailer, 1)];
}

bool PoolTrailer_isStale(PoolTrailer* trailer)
{
    return trailer->backPool->sequence > trailer->sequence;
}

int PoolTrailer_trailingIndex(PoolTrailer* trailer)
{
       int diff = trailer->backPool->currentPosition - trailer->sequence;
       if (diff < 0){
            return diff + trailer->backPool->length;
       }
       else {
            return diff;
       }
}

int PoolTrailer_getAbsoluteIndex(PoolTrailer* trailer, int trailingIndex)
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

bool PoolTrailer_hasExpired(PoolTrailer* trailer, int index)
{
    return ((trailer->backPool->sequence / trailer->backPool->length)>=2) || (PoolTrailer_trailingIndex(trailer) + index) > trailer->backPool->length;
}

bool PoolTrailer_keepUp(PoolTrailer* trailer)
{
    if (trailer->sequence<trailer->backPool->sequence)
    {
        PoolTrailer_moveForward(trailer);
        return true;
    }
    else
    {

        return false;
    }
}

void PoolTrailer_moveForward(PoolTrailer* trailer)
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
