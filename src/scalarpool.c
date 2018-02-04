#define SCALARPOOL_TYPE
#include "scalarpool.h"

POOL_NAME* POOL_TYPE_NAME(new)(int capacity, POOL_ITEM_PTR initValue)
{
    POOL_NAME* pool = malloc(sizeof(struct POOL_NAME));

    POOL_ITEM_PTR* poolItems= malloc(capacity * sizeof(POOL_ITEM_PTR));

    int i;
    for (i = 0; i < capacity; i++) {
        poolItems[i] = initValue;
    }

    pool->subscribers = NULL;
    pool->subscriberSize = 0;
    pool->defaultValue = initValue;
    pool->items= poolItems;
    pool->lastPosition= -1;
    pool->currentPosition= 0;
    pool->length= 0;
    pool->capacity = capacity;
    pool->offset = 1;
    pool->sequence = 0;

    return pool;
}

void POOL_TYPE_NAME(assign)(POOL_NAME* pool, POOL_ITEM_PTR value)
{
    pool->items[pool->currentPosition] = value;

    POOL_TYPE_NAME(moveForward)(pool);

    if (pool->subscribers!=NULL)
    {
        int i;
        for(i = 0; i < pool->subscriberSize; i++) {
            pool->subscribers[i](pool->items[pool->lastPosition]);
        }
    }

}
