#ifndef SEGMENTPOOL_H_INCLUDED
#define SEGMENTPOOL_H_INCLUDED

#include <stdlib.h>
#include "backpool.h"
#ifndef POOL_TYPE_NAME
#define POOL_TYPE_NAME(x) SegmentPool ## _ ## x
#endif //POOL_TYPE_NAME
#include "poolcommon.c"

POOL_NAME* POOL_TYPE_NAME(new)(int capacity, int segmentSize, POOL_ITEM_PTR initValue);
void POOL_TYPE_NAME(assign)(POOL_NAME* pool, POOL_ITEM_PTR values);

#endif // SEGMENTPOOL_H_INCLUDED
