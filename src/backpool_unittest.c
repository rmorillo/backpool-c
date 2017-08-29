#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "backpool.h"
#include "objectpool.h"
#include "linkedlist.h"
#include "CuTest.h"

POOL_ITEM_TYPE* PoolItem_new(int value)
{
    POOL_ITEM_TYPE* item = malloc(sizeof(struct POOL_ITEM_TYPE));

    item->value = value;

    return item;
}

void PoolItem_update(POOL_ITEM_TYPE* poolItem, int value)
{
    poolItem->value = value;
}

void TestBackPool_capacity(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    CuAssertTrue(tc, pool->capacity==capacity);
}

void TestBackPool_update(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));

    CuAssertTrue(tc, pool->length==1);
    CuAssertTrue(tc, pool->sequence==1);
    CuAssertTrue(tc, pool->lastPosition==0);
    CuAssertTrue(tc, pool->currentPosition==1);
    CuAssertTrue(tc, ObjectPool_current(pool)->value==1);
}

void TestBackPool_consecutiveUpdates(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));

    CuAssertTrue(tc, pool->length==2);
    CuAssertTrue(tc, pool->sequence==2);
    CuAssertTrue(tc, pool->lastPosition==1);
    CuAssertTrue(tc, pool->currentPosition==2);
    CuAssertTrue(tc, ObjectPool_current(pool)->value==2);
    CuAssertTrue(tc, ObjectPool_previous(pool)->value==1);
    CuAssertTrue(tc, !pool->hasRolledOver);
}

void TestBackPool_rollOver(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, pool->hasRolledOver);
    CuAssertTrue(tc, pool->length==3);
    CuAssertTrue(tc, pool->sequence==3);
    CuAssertTrue(tc, pool->lastPosition==2);
    CuAssertTrue(tc, pool->currentPosition==0);
    CuAssertTrue(tc, ObjectPool_current(pool)->value==3);
    CuAssertTrue(tc, ObjectPool_previous(pool)->value==2);
}

void TestBackPool_item(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, ObjectPool_item(pool, 0)->value==3);
    CuAssertTrue(tc, ObjectPool_item(pool, 1)->value==2);
    CuAssertTrue(tc, ObjectPool_item(pool, 2)->value==1);
}

void TestBackPool_itemPoolIndexOutOfRange(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, ObjectPool_item(pool, 0)->value==3);
    CuAssertTrue(tc, ObjectPool_item(pool, 1)->value==2);
    CuAssertTrue(tc, ObjectPool_item(pool, 2)->value==1);
    CuAssertTrue(tc, ObjectPool_item(pool, 3)==NULL);
}

void TestBackPool_subscribe(CuTest* tc) {
    int capacity= 3;
    int poolItem1Index= 0;
    int poolItem2Index= 0;
    POOL_ITEM_TYPE* poolItem1[3];
    POOL_ITEM_TYPE* poolItem2[3];

    BackPool* pool = ObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    ObjectPool_subscribe(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { poolItem1[poolItem1Index]= poolItem; poolItem1Index++;}));
    ObjectPool_subscribe(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { poolItem2[poolItem2Index]= poolItem; poolItem2Index++;}));

    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));
    ObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, poolItem1Index == 3);
    CuAssertTrue(tc, poolItem2Index == 3);
    CuAssertTrue(tc, ObjectPool_item(pool, 0)->value == poolItem1[2]->value);
    CuAssertTrue(tc, ObjectPool_item(pool, 0)->value == poolItem2[2]->value);
    CuAssertTrue(tc, ObjectPool_item(pool, 1)->value == poolItem1[1]->value);
    CuAssertTrue(tc, ObjectPool_item(pool, 1)->value == poolItem2[1]->value);
    CuAssertTrue(tc, ObjectPool_item(pool, 2)->value == poolItem1[0]->value);
    CuAssertTrue(tc, ObjectPool_item(pool, 2)->value == poolItem2[0]->value);
}

CuSuite* BackPoolTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestBackPool_capacity);
	SUITE_ADD_TEST(suite, TestBackPool_update);
	SUITE_ADD_TEST(suite, TestBackPool_consecutiveUpdates);
	SUITE_ADD_TEST(suite, TestBackPool_rollOver);
	SUITE_ADD_TEST(suite, TestBackPool_item);
	SUITE_ADD_TEST(suite, TestBackPool_itemPoolIndexOutOfRange);
	SUITE_ADD_TEST(suite, TestBackPool_subscribe);

	return suite;
}