#ifndef OBJECTPOOL_UNITTEST_H_INCLUDED
#define OBJECTPOOL_UNITTEST_H_INCLUDED

POOL_ITEM_TYPE PoolItem_new(float value);

void PoolItem_update(POOL_ITEM_TYPE poolItem, float value);

void TestObjectPool_capacity(CuTest* tc);

void TestObjectPool_update(CuTest* tc);

void TestObjectPool_consecutiveUpdates(CuTest* tc);

void TestObjectPool_rollOver(CuTest* tc);

void TestObjectPool_item(CuTest* tc);

void TestObjectPool_itemPoolIndexOutOfRange(CuTest* tc);

void TestObjectPool_subscribe(CuTest* tc);

CuSuite* ObjectPoolTestSuite(void);

#endif // OBJECTPOOL_UNITTEST_H_INCLUDED
