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

typedef struct BackPool
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

} BackPool;
#endif // LOOKBEHINDPOOL_H_INCLUDED
