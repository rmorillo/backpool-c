#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#define SCALARPOOL_TYPE
#define POOL_ITEM_TYPE float
#include "scalarpool.h"
#include "cutest.h"

void TestBackPool_capacity(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ScalarPool_new(capacity, NAN);

    CuAssertTrue(tc, pool->capacity==capacity);
}

void TestBackPool_update(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ScalarPool_new(capacity, NAN);

    ScalarPool_assign(pool, 1.1f);

    CuAssertTrue(tc, pool->length==1);
    CuAssertTrue(tc, pool->sequence==1);
    CuAssertTrue(tc, pool->lastPosition==0);
    CuAssertTrue(tc, pool->currentPosition==1);
    float currentValue= ScalarPool_current(pool);
    CuAssertTrue(tc,round(currentValue * 10)==11);
}

void TestBackPool_consecutiveUpdates(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ScalarPool_new(capacity, NAN);

    ScalarPool_assign(pool, 1);
    ScalarPool_assign(pool, 2);

    CuAssertTrue(tc, pool->length==2);
    CuAssertTrue(tc, pool->sequence==2);
    CuAssertTrue(tc, pool->lastPosition==1);
    CuAssertTrue(tc, pool->currentPosition==2);
    CuAssertTrue(tc, ScalarPool_current(pool)==2);
    CuAssertTrue(tc, ScalarPool_previous(pool)==1);
    CuAssertTrue(tc, !pool->hasRolledOver);
}

void TestBackPool_rollOver(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ScalarPool_new(capacity, NAN);

    ScalarPool_assign(pool, 1);
    ScalarPool_assign(pool, 2);
    ScalarPool_assign(pool, 3);

    CuAssertTrue(tc, pool->hasRolledOver);
    CuAssertTrue(tc, pool->length==3);
    CuAssertTrue(tc, pool->sequence==3);
    CuAssertTrue(tc, pool->lastPosition==2);
    CuAssertTrue(tc, pool->currentPosition==0);
    CuAssertTrue(tc, ScalarPool_current(pool)==3);
    CuAssertTrue(tc, ScalarPool_previous(pool)==2);
}

void TestBackPool_item(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ScalarPool_new(capacity, NAN);

    ScalarPool_assign(pool, 1);
    ScalarPool_assign(pool, 2);
    ScalarPool_assign(pool, 3);

    CuAssertTrue(tc, ScalarPool_item(pool, 0)==3);
    CuAssertTrue(tc, ScalarPool_item(pool, 1)==2);
    CuAssertTrue(tc, ScalarPool_item(pool, 2)==1);
}

void TestBackPool_itemPoolIndexOutOfRange(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = ScalarPool_new(capacity, NAN);

    ScalarPool_assign(pool, 1);
    ScalarPool_assign(pool, 2);
    ScalarPool_assign(pool, 3);

    CuAssertTrue(tc, ScalarPool_item(pool, 0)==3);
    CuAssertTrue(tc, ScalarPool_item(pool, 1)==2);
    CuAssertTrue(tc, ScalarPool_item(pool, 2)==1);
    CuAssertTrue(tc, ScalarPool_item(pool, 3)==NAN);
}

void TestBackPool_subscribe(CuTest* tc) {
    int capacity= 3;
    int poolItem1Index= 0;
    int poolItem2Index= 0;
    POOL_ITEM_TYPE poolItem1[3];
    POOL_ITEM_TYPE poolItem2[3];

    BackPool* pool = ScalarPool_new(capacity, NAN);

    ScalarPool_subscribe(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { poolItem1[poolItem1Index]= poolItem; poolItem1Index++;}));
    ScalarPool_subscribe(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { poolItem2[poolItem2Index]= poolItem; poolItem2Index++;}));

    ScalarPool_assign(pool, 1);
    ScalarPool_assign(pool, 2);
    ScalarPool_assign(pool, 3);

    CuAssertTrue(tc, poolItem1Index == 3);
    CuAssertTrue(tc, poolItem2Index == 3);
    CuAssertTrue(tc, ScalarPool_item(pool, 0) == poolItem1[2]);
    CuAssertTrue(tc, ScalarPool_item(pool, 0) == poolItem2[2]);
    CuAssertTrue(tc, ScalarPool_item(pool, 1) == poolItem1[1]);
    CuAssertTrue(tc, ScalarPool_item(pool, 1) == poolItem2[1]);
    CuAssertTrue(tc, ScalarPool_item(pool, 2) == poolItem1[0]);
    CuAssertTrue(tc, ScalarPool_item(pool, 2) == poolItem2[0]);
}

CuSuite* ScalarPoolTestSuite(void)
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
