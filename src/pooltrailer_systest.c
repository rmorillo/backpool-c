#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif
#include <pthread.h>

#define POOL_NAME BackPool

#define POOL_ITEM_PTR LongPoolItem*

typedef struct LongPoolItem {
    long value;
} LongPoolItem;

#define POOL_TYPE_NAME(x) ThreadedLongObjectPool ## _ ## x
#include "objectpool.c"
#define POOL_TRAILER_TYPE_NAME(x) LongObjectPoolTrailer ## _ ## x
#include "pooltrailer.c"
#include "cutest.h"

POOL_ITEM_PTR ThreadedLongPoolItem_new(long value)
{
    POOL_ITEM_PTR item = malloc(sizeof(struct POOL_ITEM_PTR));

    item->value = value;

    return item;
}

void ThreadedLongPoolItem_update(POOL_ITEM_PTR poolItem, long value)
{
    poolItem->value = value;
}


void *myThreadFun(void* pool)
{
    int i;
    for(i=0; i<20; i++)
    {
        ThreadedLongObjectPool_update(pool, LAMBDA(void _(POOL_ITEM_PTR poolItem) {ThreadedLongPoolItem_update(poolItem, i);}));
        sleep(1);
    }

    return NULL;
}

void TestThreadedPoolTrailer_keepUp(CuTest* tc) {


    int capacity= 5;

    BackPool* pool = ThreadedLongObjectPool_new(capacity, LAMBDA(POOL_ITEM_PTR _(void){ return ThreadedLongPoolItem_new(LONG_MIN);}));

    pthread_t tid;
    printf("Before Thread\n");

    pthread_create(&tid, NULL, myThreadFun, pool);

    printf("After Thread.  Please wait...\n");


    sleep(2);

    PoolTrailer* trailer= LongObjectPoolTrailer_new(pool);

    sleep(10);

    CuAssertTrue(tc, LongObjectPoolTrailer_hasExpired(trailer, 0));
    //CuAssertTrue(tc, LongObjectPoolTrailer_keepUp(trailer));
    //CuAssertTrue(tc, LongObjectPoolTrailer_current(trailer)->value==4);
    //CuAssertTrue(tc, LongObjectPoolTrailer_keepUp(trailer));
    //CuAssertTrue(tc, LongObjectPoolTrailer_current(trailer)->value==5);
    //CuAssertTrue(tc, !LongObjectPoolTrailer_keepUp(trailer));

    pthread_join(tid, NULL);
}

CuSuite* ThreadedPoolTrailerTestSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestThreadedPoolTrailer_keepUp);

	return suite;
}
