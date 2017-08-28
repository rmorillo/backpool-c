#include <stdlib.h>
#include <stdio.h>
#include "backpool.h"
#include "scalarpool.h"

BackPool* ScalarPool_new(int capacity, POOL_ITEM_TYPE* (*initItem)(void))
{
    BackPool* pool = malloc(sizeof(struct BackPool));

    POOL_ITEM_TYPE** poolItems= malloc(capacity * sizeof(struct POOL_ITEM_TYPE *));

    int i;
    for (i = 0; i < capacity; i++) {
        poolItems[i] = initItem();
    }

    pool->subscribers = NULL;

    pool->items= poolItems;
    pool->lastPosition= -1;
    pool->currentPosition= 0;
    pool->length= 0;
    pool->capacity = capacity;
    pool->offset = 1;
    pool->sequence = 0;

    return pool;
}

void ScalarPool_update(BackPool* pool, void (*updateItem)(POOL_ITEM_TYPE* poolItem))
{
    updateItem(pool->items[pool->currentPosition]);

    ScalarPool_moveForward(pool);

    if (pool->subscribers!=NULL)
    {
        ListNode* subscriberNode= pool->subscribers;
        while (subscriberNode!=NULL)
        {
            subscriberNode->item.poolChange(pool->items[pool->lastPosition]);
            subscriberNode= subscriberNode->nextItem;
        }
    }

}

void ScalarPool_moveForward(BackPool* pool)
{
    pool->lastPosition = pool->currentPosition;
    pool->sequence++;

    if (!pool->hasRolledOver)
    {
        pool->length++;
    }
    if (pool->currentPosition < (pool->capacity -1))
    {
        pool->currentPosition += pool->offset;
    }
    else
    {
        pool->currentPosition = 0;
        pool->hasRolledOver = true;
    }
}

POOL_ITEM_TYPE* ScalarPool_item(BackPool* pool, int lookBehindIndex)
{
    int absoluteIndex= ScalarPool_getAbsoluteIndex(pool, lookBehindIndex);
    if (absoluteIndex < 0)
    {
        printf("Pool index is out of range.  You can only use index values from 0 to %i\n", pool->length - 1);
        return NULL;
    }
    else
    {
        return pool->items[absoluteIndex];
    }
}

int ScalarPool_getAbsoluteIndex(BackPool* pool, int lookBehindIndex)
{
    int targetIndex = pool->lastPosition - lookBehindIndex;
    if (targetIndex < 0)
    {
        int absoluteIndex = targetIndex + pool->capacity;
        if (pool->hasRolledOver && absoluteIndex > pool->lastPosition)
        {
            targetIndex = absoluteIndex;
        }
        else
        {
            targetIndex = -1; //Pool index is out of range.
        }
    }

    return targetIndex;
}

void ScalarPool_subscribe(BackPool* pool, void (*poolChange)(POOL_ITEM_TYPE* poolItem))
{
    PoolSubscriber subscriber;
    subscriber.poolChange= poolChange;

    if (pool->subscribers==NULL)
    {
        pool->subscribers= LinkedList_rootNode(subscriber);
    }
    else
    {
        ListNode* lastNode = pool->subscribers;
        while (lastNode->nextItem!=NULL)
        {
            lastNode = lastNode->nextItem;
        }

        LinkedList_attachNode(lastNode, subscriber);
    }
}

POOL_ITEM_TYPE* ScalarPool_current(BackPool* pool)
{
    return pool->items[pool->lastPosition];
}

POOL_ITEM_TYPE* ScalarPool_previous(BackPool* pool)
{
    return pool->items[pool->lastPosition - 1];
}
