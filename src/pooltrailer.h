#ifndef POOLTRAILER_H_INCLUDED
#define POOLTRAILER_H_INCLUDED

#include "backpool.h"

typedef struct PoolTrailer
{
    BackPool* backPool;
    int lastPosition;
    int currentPosition;
    int sequence;
} PoolTrailer;

PoolTrailer* PoolTrailer_new(BackPool* pool);
POOL_ITEM_TYPE PoolTrailer_current(PoolTrailer* trailer);
POOL_ITEM_TYPE PoolTrailer_previous(PoolTrailer* trailer);
bool PoolTrailer_isStale(PoolTrailer* trailer);
bool PoolTrailer_hasExpired(PoolTrailer* trailer, int index);
int PoolTrailer_trailingIndex(PoolTrailer* trailer);
int PoolTrailer_getAbsoluteIndex(PoolTrailer* trailer, int trailingIndex);
bool PoolTrailer_keepUp(PoolTrailer* trailer);
void PoolTrailer_moveForward(PoolTrailer* trailer);
#endif // POOLTRAILER_H_INCLUDED
