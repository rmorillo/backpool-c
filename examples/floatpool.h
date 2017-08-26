#ifndef FLOATPOOL_H_INCLUDED
#define FLOATPOOL_H_INCLUDED

#define POOL_TYPE_PREFIX(x) FloatPool ## _ ## x
#define POOL_ITEM_TYPE FloatPoolItem
#define POOL_ITEM_TYPE_PREFIX(x) FloatPoolItem ## _ ## x

typedef struct POOL_ITEM_TYPE {
    float value;
} POOL_ITEM_TYPE;

#include "lookbehindpool.h"

POOL_ITEM_TYPE* POOL_ITEM_TYPE_PREFIX(new)(float value);
void POOL_ITEM_TYPE_PREFIX(update)(POOL_ITEM_TYPE* poolItem, float value);
void POOL_TYPE_PREFIX(update)(Pool* pool, float value);

#include "poolcommon.h"

#endif // FLOATPOOL_H_INCLUDED
