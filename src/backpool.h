#ifndef BACKPOOL_H_INCLUDED
#define BACKPOOL_H_INCLUDED

#ifdef SCALAR_POOL_TYPE
    #ifndef POOL_ITEM_PTR
        #define POOL_ITEM_PTR IntPoolItemType*
    #endif
#else
    #ifndef POOL_ITEM_PTR
        #define POOL_ITEM_PTR IntObjectPool*
    #endif
#endif // SCALAR_POOL_TYPE

typedef int IntPoolItemType;

typedef struct IntObjectPool {
    int value;
} IntObjectPool;

#include <stdbool.h>
#define LAMBDA(c_) ({ c_ _;})

#ifndef POOL_NAME
#define POOL_NAME BackPools
#endif

#ifndef NULL
#define NULL ( (void *) 0)
#endif // NULL

typedef struct POOL_NAME
{
    POOL_ITEM_PTR* items;
    int subscriberSize;
    void (**subscribers)(POOL_ITEM_PTR poolItem);
    POOL_ITEM_PTR defaultValue;
    int capacity;
    int lastPosition;
    int currentPosition;
    int sequence;
    int length;
    bool hasRolledOver;
    int offset;

} POOL_NAME;

#endif
