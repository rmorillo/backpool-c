#ifndef OBJECTPOOL_H_INCLUDED
#define OBJECTPOOL_H_INCLUDED

#include <stdlib.h>
#include "backpool.h"
#ifndef POOL_TYPE_NAME
#define POOL_TYPE_NAME(x) ObjectPool ## _ ## x
#endif // POOL_TYPE_NAME
#include "poolcommon.c"

POOL_NAME* ObjectPool_new(int capacity, POOL_ITEM_PTR (*initItem)(void));

void ObjectPool_update(POOL_NAME* pool, void (*updateItem)(POOL_ITEM_PTR poolItem));

#endif // OBJECTPOOL_H_INCLUDED
