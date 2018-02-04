#ifndef POOLTRAILER_H_INCLUDED
#define POOLTRAILER_H_INCLUDED

#include "backpool.h"
#ifndef POOL_TRAILER_TYPE_NAME
#define POOL_TRAILER_TYPE_NAME(x) BasicPoolTrailer ## _ ## x
#endif
typedef struct PoolTrailer
{
    POOL_NAME* backPool;
    int lastPosition;
    int currentPosition;
    int sequence;
} PoolTrailer;

PoolTrailer* POOL_TRAILER_TYPE_NAME(new)(POOL_NAME* pool);
POOL_ITEM_PTR POOL_TRAILER_TYPE_NAME(current)(PoolTrailer* trailer);
POOL_ITEM_PTR POOL_TRAILER_TYPE_NAME(previous)(PoolTrailer* trailer);
bool POOL_TRAILER_TYPE_NAME(isStale)(PoolTrailer* trailer);
bool POOL_TRAILER_TYPE_NAME(hasExpired)(PoolTrailer* trailer, int index);
int POOL_TRAILER_TYPE_NAME(trailingIndex)(PoolTrailer* trailer);
int POOL_TRAILER_TYPE_NAME(getAbsoluteIndex)(PoolTrailer* trailer, int trailingIndex);
bool POOL_TRAILER_TYPE_NAME(keepUp)(PoolTrailer* trailer);
void POOL_TRAILER_TYPE_NAME(moveForward)(PoolTrailer* trailer);
void POOL_TRAILER_TYPE_NAME(refresh)(PoolTrailer* trailer);
#endif // POOLTRAILER_H_INCLUDED
