#ifndef SCALARPOOL_H_INCLUDED
#define SCALARPOOL_H_INCLUDED

BackPool* ScalarPool_new(int capacity, POOL_ITEM_TYPE* (*initItem)(void));
void ScalarPool_update(BackPool* pool, void (*updateItem)(POOL_ITEM_TYPE* poolItem));
POOL_ITEM_TYPE* ScalarPool_current(BackPool* pool);
POOL_ITEM_TYPE* ScalarPool_previous(BackPool* pool);
POOL_ITEM_TYPE* ScalarPool_item(BackPool* pool, int index);
void ScalarPool_moveForward(BackPool* pool);
int ScalarPool_getAbsoluteIndex(BackPool* pool, int lookBehindIndex);
void ScalarPool_subscribe(BackPool* pool, void (*poolChange)(POOL_ITEM_TYPE* poolItem));

#endif // SCALARPOOL_H_INCLUDED
