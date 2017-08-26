#ifndef INTPOOL_H_INCLUDED
#define INTPOOL_H_INCLUDED

#define POOL_TYPE_PREFIX(x) IntPool ## _ ## x
#define POOL_ITEM_TYPE IntPoolItem
#define POOL_ITEM_TYPE_PREFIX(x) IntPoolItem ## _ ## x

typedef struct POOL_ITEM_TYPE {
    int value;
} POOL_ITEM_TYPE;

#include "lookbehindpool.h"

POOL_ITEM_TYPE* POOL_ITEM_TYPE_PREFIX(new)(int value);
void POOL_ITEM_TYPE_PREFIX(update)(POOL_ITEM_TYPE* poolItem, int value);
void POOL_TYPE_PREFIX(update)(Pool* pool, int value);

#include "poolcommon.h"


#endif // INTPOOL_H_INCLUDED
