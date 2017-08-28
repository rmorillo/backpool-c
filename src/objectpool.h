#ifndef OBJECTObjectPool_H_INCLUDED
#define OBJECTObjectPool_H_INCLUDED

BackPool* ObjectPool_new(int capacity, POOL_ITEM_TYPE* (*initItem)(void));
void ObjectPool_update(BackPool* pool, void (*updateItem)(POOL_ITEM_TYPE* poolItem));
POOL_ITEM_TYPE* ObjectPool_current(BackPool* pool);
POOL_ITEM_TYPE* ObjectPool_previous(BackPool* pool);
POOL_ITEM_TYPE* ObjectPool_item(BackPool* pool, int index);
void ObjectPool_moveForward(BackPool* pool);
int ObjectPool_getAbsoluteIndex(BackPool* pool, int lookBehindIndex);
void ObjectPool_subscribe(BackPool* pool, void (*poolChange)(POOL_ITEM_TYPE* poolItem));

#endif // OBJECTObjectPool_H_INCLUDED
