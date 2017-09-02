#include "objectpool.h"

BackPool* POOL_TYPE_NAME(new)(int capacity, POOL_ITEM_TYPE (*initItem)(void))
{
    BackPool* pool = malloc(sizeof(struct BackPool));

    POOL_ITEM_TYPE* poolItems= malloc(capacity * sizeof(struct POOL_ITEM_TYPE));

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

void POOL_TYPE_NAME(update)(BackPool* pool, void (*updateItem)(POOL_ITEM_TYPE poolItem))
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
