#ifndef OBJECTPOOL_H_INCLUDED
#define OBJECTPOOL_H_INCLUDED

#include <stdlib.h>
#include "backpool.h"
#ifndef POOL_TYPE_NAME
#define POOL_TYPE_NAME(x) ObjectPool ## _ ## x
#endif // POOL_TYPE_NAME
#include "poolcommon.c"

BackPool* ObjectPool_new(int capacity, POOL_ITEM_TYPE (*initItem)(void));

void ObjectPool_update(BackPool* pool, void (*updateItem)(POOL_ITEM_TYPE poolItem));

#endif // OBJECTPOOL_H_INCLUDED
