#ifndef POOLCOMMON_H_INCLUDED
#define POOLCOMMON_H_INCLUDED

Pool* POOL_TYPE_PREFIX(new)(int capacity);
void POOL_TYPE_PREFIX(subscribe)(Pool* pool, void (*poolChange)(POOL_ITEM_TYPE* poolItem));
POOL_ITEM_TYPE* POOL_TYPE_PREFIX(current)(Pool* pool);
POOL_ITEM_TYPE* POOL_TYPE_PREFIX(previous)(Pool* pool);

#endif // POOLCOMMON_H_INCLUDED
