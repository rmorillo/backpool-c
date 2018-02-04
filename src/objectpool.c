#include "objectpool.h"

POOL_NAME* POOL_TYPE_NAME(new)(int capacity, POOL_ITEM_PTR (*initItem)(void))
{
    POOL_NAME* pool = malloc(sizeof(struct POOL_NAME));

    POOL_ITEM_PTR* poolItems= malloc(capacity * sizeof(struct POOL_ITEM_PTR));

    int i;
    for (i = 0; i < capacity; i++) {
        poolItems[i] = initItem();
    }

    pool->subscribers = NULL;
    pool->subscriberSize = 0;
    pool->defaultValue = NULL;
    pool->items= poolItems;
    pool->lastPosition= -1;
    pool->currentPosition= 0;
    pool->length= 0;
    pool->capacity = capacity;
    pool->offset = 1;
    pool->sequence = 0;

    return pool;
}

void POOL_TYPE_NAME(update)(POOL_NAME* pool, void (*updateItem)(POOL_ITEM_PTR poolItem))
{
    updateItem(pool->items[pool->currentPosition]);

    POOL_TYPE_NAME(moveForward)(pool);

    if (pool->subscribers!=NULL)
    {
        int i;
        for(i = 0; i < pool->subscriberSize; i++) {
            pool->subscribers[i](pool->items[pool->lastPosition]);
        }
    }

}
