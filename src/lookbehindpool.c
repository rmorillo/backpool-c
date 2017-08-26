#include <stdlib.h>
#include <stdio.h>
#include "lookbehindpool.h"

Pool* Pool_new(int capacity, POOL_ITEM_TYPE* (*initItem)(void))
{
    Pool* pool = malloc(sizeof(struct Pool));

    POOL_ITEM_TYPE** poolItems= malloc(capacity * sizeof(struct POOL_ITEM_TYPE *));

    for (int i = 0; i < capacity; i++) {
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

void Pool_update(Pool* pool, void (*updateItem)(POOL_ITEM_TYPE* poolItem))
{
    updateItem(pool->items[pool->currentPosition]);

    Pool_moveForward(pool);

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

void Pool_moveForward(Pool* pool)
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

POOL_ITEM_TYPE* Pool_item(Pool* pool, int lookBehindIndex)
{
    int absoluteIndex= Pool_getAbsoluteIndex(pool, lookBehindIndex);
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

int Pool_getAbsoluteIndex(Pool* pool, int lookBehindIndex)
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

void Pool_subscribe(Pool* pool, void (*poolChange)(POOL_ITEM_TYPE* poolItem))
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

POOL_ITEM_TYPE* Pool_current(Pool* pool)
{
    return pool->items[pool->lastPosition];
}

POOL_ITEM_TYPE* Pool_previous(Pool* pool)
{
    return pool->items[pool->lastPosition - 1];
}
