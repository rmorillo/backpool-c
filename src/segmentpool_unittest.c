#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#define POOL_NAME BackPool
#define SCALARPOOL_TYPE
#define POOL_ITEM_PTR float*
#define POOL_TYPE_NAME(x) FloatSegmentPool ## _ ## x
#include "segmentpool.c"
#include "cutest.h"

typedef float TriFloat[3];

void TestScalarPool_capacity(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatSegmentPool_new(capacity, 3, (TriFloat){NAN, NAN, NAN});

    CuAssertTrue(tc, pool->capacity==capacity);
}

void TestScalarPool_update(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatSegmentPool_new(capacity, 3, (TriFloat){NAN, NAN, NAN});

    FloatSegmentPool_assign(pool, (TriFloat){1.1, 1.1, 1.1});

    CuAssertTrue(tc, pool->length==1);
    CuAssertTrue(tc, pool->sequence==1);
    CuAssertTrue(tc, pool->lastPosition==0);
    CuAssertTrue(tc, pool->currentPosition==1 * pool->offset);
    float* currentValue= FloatSegmentPool_current(pool);
    CuAssertTrue(tc,round(currentValue[0] * 10)==11);
}

void TestScalarPool_consecutiveUpdates(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatSegmentPool_new(capacity, 3, (TriFloat){NAN, NAN, NAN});

    FloatSegmentPool_assign(pool, (TriFloat){1, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){2, 0, 0});

    CuAssertTrue(tc, pool->length==2);
    CuAssertTrue(tc, pool->sequence==2);
    CuAssertTrue(tc, pool->lastPosition==1 * pool->offset);
    CuAssertTrue(tc, pool->currentPosition==2 * pool->offset);
    CuAssertTrue(tc, FloatSegmentPool_current(pool)[0]==2);
    CuAssertTrue(tc, FloatSegmentPool_previous(pool)[0]==1);
    CuAssertTrue(tc, !pool->hasRolledOver);
}

void TestScalarPool_rollOver(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatSegmentPool_new(capacity, 3, (TriFloat){NAN, NAN, NAN});

    FloatSegmentPool_assign(pool, (TriFloat){1, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){2, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){3, 0, 0});

    CuAssertTrue(tc, pool->hasRolledOver);
    CuAssertTrue(tc, pool->length==3);
    CuAssertTrue(tc, pool->sequence==3);
    CuAssertTrue(tc, pool->lastPosition==2 * pool->offset);
    CuAssertTrue(tc, pool->currentPosition==0);
    CuAssertTrue(tc, FloatSegmentPool_current(pool)[0]==3);
    CuAssertTrue(tc, FloatSegmentPool_previous(pool)[0]==2);
}

void TestScalarPool_item(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatSegmentPool_new(capacity, 3, (TriFloat){NAN, NAN, NAN});

    FloatSegmentPool_assign(pool, (TriFloat){1, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){2, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){3, 0, 0});

    CuAssertTrue(tc, FloatSegmentPool_item(pool, 0)[0]==3);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 1)[0]==2);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 2)[0]==1);
}

void TestScalarPool_itemPoolIndexOutOfRange(CuTest* tc) {
    int capacity= 3;

    BackPool* pool = FloatSegmentPool_new(capacity, 3, (TriFloat){NAN, NAN, NAN});

    FloatSegmentPool_assign(pool, (TriFloat){1, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){2, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){3, 0, 0});

    CuAssertTrue(tc, FloatSegmentPool_item(pool, 0)[0]==3);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 1)[0]==2);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 2)[0]==1);
    CuAssertTrue(tc, isnan(FloatSegmentPool_item(pool, 3)[0]));
}

void TestScalarPool_subscribe(CuTest* tc) {
    int capacity= 3;
    int poolItem1Index= 0;
    int poolItem2Index= 0;
    POOL_ITEM_PTR poolItem1[3];
    POOL_ITEM_PTR poolItem2[3];

    BackPool* pool = FloatSegmentPool_new(capacity, 3, (TriFloat){NAN, NAN, NAN});

    FloatSegmentPool_subscribe(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { poolItem1[poolItem1Index]= poolItem; poolItem1Index++;}));
    FloatSegmentPool_subscribe(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) { poolItem2[poolItem2Index]= poolItem; poolItem2Index++;}));

    FloatSegmentPool_assign(pool, (TriFloat){1, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){2, 0, 0});
    FloatSegmentPool_assign(pool, (TriFloat){3, 0, 0});

    CuAssertTrue(tc, poolItem1Index == 3);
    CuAssertTrue(tc, poolItem2Index == 3);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 0) == poolItem1[2]);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 0) == poolItem2[2]);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 1) == poolItem1[1]);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 1) == poolItem2[1]);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 2) == poolItem1[0]);
    CuAssertTrue(tc, FloatSegmentPool_item(pool, 2) == poolItem2[0]);
}

CuSuite* SegmentPoolTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestScalarPool_capacity);
	SUITE_ADD_TEST(suite, TestScalarPool_update);
	SUITE_ADD_TEST(suite, TestScalarPool_consecutiveUpdates);
	SUITE_ADD_TEST(suite, TestScalarPool_rollOver);
	SUITE_ADD_TEST(suite, TestScalarPool_item);
	SUITE_ADD_TEST(suite, TestScalarPool_itemPoolIndexOutOfRange);
	SUITE_ADD_TEST(suite, TestScalarPool_subscribe);

	return suite;
}
