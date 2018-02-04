#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define POOL_NAME BackPool

#define POOL_ITEM_PTR LongPoolItem*

typedef struct LongPoolItem {
    long value;
} LongPoolItem;

#define POOL_TYPE_NAME(x) LongObjectPool ## _ ## x
#include "objectpool.c"
#define POOL_TRAILER_TYPE_NAME(x) PoolTrailer ## _ ## x
#include "pooltrailer.c"
#include "cutest.h"

POOL_ITEM_PTR LongPoolItem_new(long value)
{
    POOL_ITEM_PTR item = malloc(sizeof(struct POOL_ITEM_PTR));

    item->value = value;

    return item;
}

void LongPoolItem_update(POOL_ITEM_PTR poolItem, long value)
{
    poolItem->value = value;
}

void TestPoolTrailer_current(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = LongObjectPool_new(capacity, LAMBDA(POOL_ITEM_PTR _(void){ return LongPoolItem_new(LONG_MIN);}));

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 1);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 2);}));

    PoolTrailer* trailer= PoolTrailer_new(pool);

    CuAssertTrue(tc, trailer->sequence==2);
    CuAssertTrue(tc, trailer->lastPosition==1);
    CuAssertTrue(tc, trailer->currentPosition==2);
    CuAssertTrue(tc, PoolTrailer_current(trailer)->value==2);
    CuAssertTrue(tc, PoolTrailer_previous(trailer)->value==1);
    CuAssertTrue(tc, !PoolTrailer_isStale(trailer));
}

void TestPoolTrailer_stale(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = LongObjectPool_new(capacity, LAMBDA(POOL_ITEM_PTR _(void){ return LongPoolItem_new(LONG_MIN);}));

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 1);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 2);}));

    PoolTrailer* trailer= PoolTrailer_new(pool);

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 3);}));

    CuAssertTrue(tc, trailer->sequence==2);
    CuAssertTrue(tc, trailer->lastPosition==1);
    CuAssertTrue(tc, trailer->currentPosition==2);
    CuAssertTrue(tc, PoolTrailer_current(trailer)->value==2);
    CuAssertTrue(tc, PoolTrailer_previous(trailer)->value==1);
    CuAssertTrue(tc, PoolTrailer_isStale(trailer));
}

void TestPoolTrailer_beforeExpiry(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = LongObjectPool_new(capacity, LAMBDA(POOL_ITEM_PTR _(void){ return LongPoolItem_new(LONG_MIN);}));

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 1);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 2);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 3);}));

    PoolTrailer* trailer= PoolTrailer_new(pool);

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 4);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 5);}));

    CuAssertTrue(tc, !PoolTrailer_hasExpired(trailer, 0));
}

void TestPoolTrailer_expiry(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = LongObjectPool_new(capacity, LAMBDA(POOL_ITEM_PTR _(void){ return LongPoolItem_new(LONG_MIN);}));

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 1);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 2);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 3);}));

    PoolTrailer* trailer= PoolTrailer_new(pool);

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 4);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 5);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 6);}));

    CuAssertTrue(tc, PoolTrailer_hasExpired(trailer, 0));
}

void TestPoolTrailer_keepUp(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = LongObjectPool_new(capacity, LAMBDA(POOL_ITEM_PTR _(void){ return LongPoolItem_new(LONG_MIN);}));

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 1);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 2);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 3);}));

    PoolTrailer* trailer= PoolTrailer_new(pool);

    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 4);}));
    LongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { LongPoolItem_update(poolItem, 5);}));

    CuAssertTrue(tc, PoolTrailer_keepUp(trailer));
    CuAssertTrue(tc, PoolTrailer_current(trailer)->value==4);
    CuAssertTrue(tc, PoolTrailer_keepUp(trailer));
    CuAssertTrue(tc, PoolTrailer_current(trailer)->value==5);
    CuAssertTrue(tc, !PoolTrailer_keepUp(trailer));
}

CuSuite* PoolTrailerTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestPoolTrailer_current);
	SUITE_ADD_TEST(suite, TestPoolTrailer_stale);
	SUITE_ADD_TEST(suite, TestPoolTrailer_beforeExpiry);
	SUITE_ADD_TEST(suite, TestPoolTrailer_expiry);
	SUITE_ADD_TEST(suite, TestPoolTrailer_keepUp);

	return suite;
}
