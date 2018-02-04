#include "CuTest.h"
#include <stdio.h>

CuSuite* ScalarPoolTestSuite(void);
CuSuite* ObjectPoolTestSuite(void);
CuSuite* SegmentPoolTestSuite(void);
CuSuite* PoolTrailerTestSuite(void);
CuSuite* ThreadedPoolTrailerTestSuite(void);

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, ScalarPoolTestSuite());
	CuSuiteAddSuite(suite, SegmentPoolTestSuite());
	CuSuiteAddSuite(suite, ObjectPoolTestSuite());
	CuSuiteAddSuite(suite, PoolTrailerTestSuite());
	CuSuiteAddSuite(suite, ThreadedPoolTrailerTestSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
	return 0;
}
