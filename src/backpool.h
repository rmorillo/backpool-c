#ifndef BACKPOOL_H_INCLUDED
#define BACKPOOL_H_INCLUDED

#ifdef SCALAR_POOL_TYPE
    #ifndef POOL_ITEM_TYPE
        #define POOL_ITEM_TYPE IntPoolItemType
    #endif
#else
    #ifndef POOL_ITEM_TYPE
        #define POOL_ITEM_TYPE IntObjectPool
    #endif
#endif // SCALAR_POOL_TYPE

typedef int IntPoolItemType;

typedef struct IntObjectPool {
    int value;
} IntObjectPool;

#include <stdbool.h>
#define LAMBDA(c_) ({ c_ _;})

#ifndef NULL
#define NULL ( (void *) 0)
#endif // NULL

typedef struct BackPool
{
    POOL_ITEM_TYPE* items;
    int subscriberSize;
    void (**subscribers)(POOL_ITEM_TYPE poolItem);
    POOL_ITEM_TYPE defaultValue;
    int capacity;
    int lastPosition;
    int currentPosition;
    int sequence;
    int length;
    bool hasRolledOver;
    int offset;

} BackPool;

#endif
