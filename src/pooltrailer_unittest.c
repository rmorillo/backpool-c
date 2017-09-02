#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#define POOL_ITEM_TYPE LongPoolItem*

typedef struct LongPoolItem {
    long value;
} LongPoolItem;

#define POOL_TYPE_NAME(x) LongObjectPool ## _ ## x
#include "objectpool.c"
#include "cutest.h"

POOL_ITEM_TYPE LongPoolItem_new(long value)
{
    POOL_ITEM_TYPE item = malloc(sizeof(struct POOL_ITEM_TYPE));

    item->value = value;

    return item;
}

void LongPoolItem_update(POOL_ITEM_TYPE poolItem, long value)
{
    poolItem->value = value;
}

void TestPoolTrailer_consecutiveUpdates(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = LongObjectPool_new(capacity, LAMBDA(POOL_ITEM_TYPE _(void){ return LongPoolItem_new(LONG_MIN);}));

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { LongPoolItem_update(poolItem, 1);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_TYPE poolItem) { LongPoolItem_update(poolItem, 2);}));

    CuAssertTrue(tc, pool->length==2);
    CuAssertTrue(tc, pool->sequence==2);
    CuAssertTrue(tc, pool->lastPosition==1);
    CuAssertTrue(tc, pool->currentPosition==2);
    CuAssertTrue(tc, LongObjectPool_current(pool)->value==2);
    CuAssertTrue(tc, LongObjectPool_previous(pool)->value==1);
    CuAssertTrue(tc, !pool->hasRolledOver);
}


CuSuite* PoolTrailerTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();


	SUITE_ADD_TEST(suite, TestPoolTrailer_consecutiveUpdates);

	return suite;
}
