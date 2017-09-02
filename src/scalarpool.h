#ifndef SCALARPOOL_H_INCLUDED
#define SCALARPOOL_H_INCLUDED

#include <stdlib.h>
#ifndef POOL_TYPE_NAME
#define POOL_TYPE_NAME(x) ScalarPool ## _ ## x
#endif //POOL_TYPE_NAME
#include "poolcommon.c"

BackPool* ScalarPool_new(int capacity, POOL_ITEM_TYPE initValue);

void ScalarPool_assign(BackPool* pool, POOL_ITEM_TYPE value);

#endif
