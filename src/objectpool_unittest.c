#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#define POOL_ITEM_TYPE FloatPoolItem*

typedef struct FloatPoolItem {
    float value;
} FloatPoolItem;

#define POOL_TYPE_NAME(x) FloatObjectPool ## _ ## x

#include "objectpool.c"
#include "cutest.h"

POOL_ITEM_TYPE PoolItem_new(float value)
{
    POOL_ITEM_TYPE item = malloc(sizeof(struct POOL_ITEM_TYPE));

    item->value = value;

    return item;
}

void PoolItem_update(POOL_ITEM_TYPE poolItem, float value)
{
    poolItem->value = value;
}

void TestFloatObjectPool_capacity(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE _(void){ return PoolItem_new(NAN);}));

    CuAssertTrue(tc, pool->capacity==capacity);
}

void TestFloatObjectPool_update(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE _(void){ return PoolItem_new(NAN);}));

    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 1.1f);}));

    CuAssertTrue(tc, pool->length==1);
    CuAssertTrue(tc, pool->sequence==1);
    CuAssertTrue(tc, pool->lastPosition==0);
    CuAssertTrue(tc, pool->currentPosition==1);
    float currentValue= FloatObjectPool_current(pool)->value;
    CuAssertTrue(tc,round(currentValue * 10)==11);
}

void TestFloatObjectPool_consecutiveUpdates(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE _(void){ return PoolItem_new(NAN);}));

    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 1);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 2);}));

    CuAssertTrue(tc, pool->length==2);
    CuAssertTrue(tc, pool->sequence==2);
    CuAssertTrue(tc, pool->lastPosition==1);
    CuAssertTrue(tc, pool->currentPosition==2);
    CuAssertTrue(tc, FloatObjectPool_current(pool)->value==2);
    CuAssertTrue(tc, FloatObjectPool_previous(pool)->value==1);
    CuAssertTrue(tc, !pool->hasRolledOver);
}

void TestFloatObjectPool_rollOver(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE _(void){ return PoolItem_new(NAN);}));

    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 1);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 2);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, pool->hasRolledOver);
    CuAssertTrue(tc, pool->length==3);
    CuAssertTrue(tc, pool->sequence==3);
    CuAssertTrue(tc, pool->lastPosition==2);
    CuAssertTrue(tc, pool->currentPosition==0);
    CuAssertTrue(tc, FloatObjectPool_current(pool)->value==3);
    CuAssertTrue(tc, FloatObjectPool_previous(pool)->value==2);
}

void TestFloatObjectPool_item(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE _(void){ return PoolItem_new(NAN);}));

    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 1);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 2);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, FloatObjectPool_item(pool, 0)->value==3);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 1)->value==2);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 2)->value==1);
}

void TestFloatObjectPool_itemPoolIndexOutOfRange(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE _(void){ return PoolItem_new(NAN);}));

    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 1);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 2);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, FloatObjectPool_item(pool, 0)->value==3);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 1)->value==2);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 2)->value==1);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 3)==NULL);
}

void TestFloatObjectPool_subscribe(CuTest* tc) {
    int capacity= 3;
    int poolItem1Index= 0;
    int poolItem2Index= 0;
    POOL_ITEM_TYPE poolItem1[3];
    POOL_ITEM_TYPE poolItem2[3];

    BackPool* pool = FloatObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE _(void){ return PoolItem_new(NAN);}));

    FloatObjectPool_subscribe(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { poolItem1[poolItem1Index]= poolItem; poolItem1Index++;}));
    FloatObjectPool_subscribe(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { poolItem2[poolItem2Index]= poolItem; poolItem2Index++;}));

    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 1);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 2);}));
    FloatObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, poolItem1Index == 3);
    CuAssertTrue(tc, poolItem2Index == 3);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 0)->value == poolItem1[2]->value);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 0)->value == poolItem2[2]->value);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 1)->value == poolItem1[1]->value);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 1)->value == poolItem2[1]->value);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 2)->value == poolItem1[0]->value);
    CuAssertTrue(tc, FloatObjectPool_item(pool, 2)->value == poolItem2[0]->value);
}

CuSuite* ObjectPoolTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFloatObjectPool_capacity);
	SUITE_ADD_TEST(suite, TestFloatObjectPool_update);
	SUITE_ADD_TEST(suite, TestFloatObjectPool_consecutiveUpdates);
	SUITE_ADD_TEST(suite, TestFloatObjectPool_rollOver);
	SUITE_ADD_TEST(suite, TestFloatObjectPool_item);
	SUITE_ADD_TEST(suite, TestFloatObjectPool_itemPoolIndexOutOfRange);
	SUITE_ADD_TEST(suite, TestFloatObjectPool_subscribe);

	return suite;
}
