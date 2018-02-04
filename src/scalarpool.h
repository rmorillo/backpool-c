#ifndef SCALARPOOL_H_INCLUDED
#define SCALARPOOL_H_INCLUDED

#include <stdlib.h>
#ifndef POOL_TYPE_NAME
#define POOL_TYPE_NAME(x) ScalarPool ## _ ## x
#endif //POOL_TYPE_NAME
#include "poolcommon.c"

POOL_NAME* ScalarPool_new(int capacity, POOL_ITEM_PTR initValue);

void ScalarPool_assign(POOL_NAME* pool, POOL_ITEM_PTR value);

#endif
