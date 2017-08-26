#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "lookbehindpool.h"

#include "linkedlist.h"

#include "CuTest.h"

/*-------------------------------------------------------------------------*
 * LookBehindPool Test
 *-------------------------------------------------------------------------*/

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

void TestLookBehindPool_capacity(CuTest* tc) {
    int capacity= 3;

    Pool* pool = Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    CuAssertTrue(tc, pool->capacity==capacity);
}

void TestLookBehindPool_update(CuTest* tc) {
    int capacity= 3;

    Pool* pool = Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));

    CuAssertTrue(tc, pool->length==1);
    CuAssertTrue(tc, pool->sequence==1);
    CuAssertTrue(tc, pool->lastPosition==0);
    CuAssertTrue(tc, pool->currentPosition==1);
    CuAssertTrue(tc, Pool_current(pool)->value==1);
}

void TestLookBehindPool_consecutiveUpdates(CuTest* tc) {
    int capacity= 3;

    Pool* pool = Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));

    CuAssertTrue(tc, pool->length==2);
    CuAssertTrue(tc, pool->sequence==2);
    CuAssertTrue(tc, pool->lastPosition==1);
    CuAssertTrue(tc, pool->currentPosition==2);
    CuAssertTrue(tc, Pool_current(pool)->value==2);
    CuAssertTrue(tc, Pool_previous(pool)->value==1);
    CuAssertTrue(tc, !pool->hasRolledOver);
}

void TestLookBehindPool_rollOver(CuTest* tc) {
    int capacity= 3;

    Pool* pool = Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, pool->hasRolledOver);
    CuAssertTrue(tc, pool->length==3);
    CuAssertTrue(tc, pool->sequence==3);
    CuAssertTrue(tc, pool->lastPosition==2);
    CuAssertTrue(tc, pool->currentPosition==0);
    CuAssertTrue(tc, Pool_current(pool)->value==3);
    CuAssertTrue(tc, Pool_previous(pool)->value==2);
}

void TestLookBehindPool_item(CuTest* tc) {
    int capacity= 3;

    Pool* pool = Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, Pool_item(pool, 0)->value==3);
    CuAssertTrue(tc, Pool_item(pool, 1)->value==2);
    CuAssertTrue(tc, Pool_item(pool, 2)->value==1);
}

void TestLookBehindPool_itemPoolIndexOutOfRange(CuTest* tc) {
    int capacity= 3;

    Pool* pool = Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, Pool_item(pool, 0)->value==3);
    CuAssertTrue(tc, Pool_item(pool, 1)->value==2);
    CuAssertTrue(tc, Pool_item(pool, 2)->value==1);
    CuAssertTrue(tc, Pool_item(pool, 3)==NULL);
}

void TestLookBehindPool_subscribe(CuTest* tc) {
    int capacity= 3;
    int poolItem1Index= 0;
    int poolItem2Index= 0;
    POOL_ITEM_TYPE* poolItem1[3];
    POOL_ITEM_TYPE* poolItem2[3];

    Pool* pool = Pool_new(capacity, LAMBDA(POOL_ITEM_TYPE* _(void){ return PoolItem_new(INT_MIN);}));

    Pool_subscribe(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { poolItem1[poolItem1Index]= poolItem; poolItem1Index++;}));
    Pool_subscribe(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { poolItem2[poolItem2Index]= poolItem; poolItem2Index++;}));

    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 1);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 2);}));
    Pool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE* poolItem) { PoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, poolItem1Index == 3);
    CuAssertTrue(tc, poolItem2Index == 3);
    CuAssertTrue(tc, Pool_item(pool, 0)->value == poolItem1[2]->value);
    CuAssertTrue(tc, Pool_item(pool, 0)->value == poolItem2[2]->value);
    CuAssertTrue(tc, Pool_item(pool, 1)->value == poolItem1[1]->value);
    CuAssertTrue(tc, Pool_item(pool, 1)->value == poolItem2[1]->value);
    CuAssertTrue(tc, Pool_item(pool, 2)->value == poolItem1[0]->value);
    CuAssertTrue(tc, Pool_item(pool, 2)->value == poolItem2[0]->value);
}

CuSuite* LookBehindPoolTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestLookBehindPool_capacity);
	SUITE_ADD_TEST(suite, TestLookBehindPool_update);
	SUITE_ADD_TEST(suite, TestLookBehindPool_consecutiveUpdates);
	SUITE_ADD_TEST(suite, TestLookBehindPool_rollOver);
	SUITE_ADD_TEST(suite, TestLookBehindPool_item);
	SUITE_ADD_TEST(suite, TestLookBehindPool_itemPoolIndexOutOfRange);
	SUITE_ADD_TEST(suite, TestLookBehindPool_subscribe);

	return suite;
}
