#ifndef LOOKBEHINDPOOL_H_INCLUDED
#define LOOKBEHINDPOOL_H_INCLUDED

#ifndef POOL_ITEM_TYPE
#define POOL_ITEM_TYPE PoolItemType
typedef struct POOL_ITEM_TYPE {
    int value;
} POOL_ITEM_TYPE;
#endif

#include <stdbool.h>
#define LAMBDA(c_) ({ c_ _;})

#ifndef NULL
#define NULL ( (void *) 0)
#endif // NULL

typedef struct PoolSubscriber
{
    void (*poolChange)(POOL_ITEM_TYPE* poolItem);
}PoolSubscriber;


#define LINKED_LIST_ITEM_TYPE PoolSubscriber
#include "linkedlist.h"

typedef struct Pool
{
    POOL_ITEM_TYPE** items;
    ListNode* subscribers;
    int capacity;
    int lastPosition;
    int currentPosition;
    int sequence;
    int length;
    bool hasRolledOver;
    int offset;

} Pool;

Pool* Pool_new(int capacity, POOL_ITEM_TYPE* (*initItem)(void));
void Pool_update(Pool* pool, void (*updateItem)(POOL_ITEM_TYPE* poolItem));
POOL_ITEM_TYPE* Pool_current(Pool* pool);
POOL_ITEM_TYPE* Pool_previous(Pool* pool);
POOL_ITEM_TYPE* Pool_item(Pool* pool, int index);
void Pool_moveForward(Pool* pool);
int Pool_getAbsoluteIndex(Pool* pool, int lookBehindIndex);
void Pool_subscribe(Pool* pool, void (*poolChange)(POOL_ITEM_TYPE* poolItem));

#endif // LOOKBEHINDPOOL_H_INCLUDED
