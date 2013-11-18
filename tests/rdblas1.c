#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <fenv.h>

#include <rblas.h>

#include "benchmark_macro.h"

int main( int argc, char **argv ) {
	double* v;
	double* y;
	double sum;
	double lsum[32];
	int n;
	double ref;

	CHECK_HEADER_()

	// ALLOCATE MEMORY
	ref = 0.0;
	v = (double*)malloc(stop * sizeof(double));
	y = (double*)malloc(stop * sizeof(double));

	int i;
	for (i = 0; i < stop; i++) y[i] = 1.0;
	
	double dd[6];

	if (print_header) {
	printf("\n");
	printf("%8s", "N");
	#ifdef CALL_DASUM
	if (tests & (1 << ASUM_BIT))
	printf("%10s", "DASUM");
	#endif
	if (tests & (1 << RASUM_BIT))
	printf("%10s", "RDASUM");
	if (tests & (1 << RSUM_BIT))
	printf("%10s", "RDSUM");
	#ifdef CALL_DNRM2
	if (tests & (1 << NRM2_BIT))
	printf("%10s", "DNRM2");
	#endif
	if (tests & (1 << RNRM2_BIT))
	printf("%10s", "RDNRM2");
	#ifdef CALL_DDOT
	if (tests & (1 << DOT_BIT))
	printf("%10s", "DDOT");
	#endif
	if (tests & (1 << RDOT_BIT))
	printf("%10s", "RDDOT");

	printf("\n");
	}

	for (n = start; n < stop + step; n += step) {
		dgenvec(n, v, dtype, 1.0);
		printf("%8d", n);

		//==== DASUM ====
#		ifdef CALL_DASUM
		time1_(ref,tests, ASUM_BIT, CALL_DASUM, n, v, incv, n)
#		endif
		//==== RDASUM ====
		time_(sum,tests, RASUM_BIT, rdasum, n, v, incv, n)

		//==== RDSUM ====
		time_(sum,tests, RSUM_BIT, rdsum, n, v, incv, n)

		//==== DNRM2 ====
#		ifdef CALL_DNRM2
		time1_(ref,tests, NRM2_BIT, CALL_DNRM2, n, v, incv, n)
#		endif

		//==== RDNRM2 ====
		time_(sum,tests, RNRM2_BIT, rdnrm2, n, v, incv, n)

		//==== DDOT ====
#		ifdef CALL_DDOT
		time3_(sum,tests, DOT_BIT, CALL_DDOT, n, v, incv, y, incy, 2*n)
#		endif

		//==== RDDOT ====
		time2_(sum, tests, RDOT_BIT, rddot, n, v, incv, y, incy, 2*n)

		fprintf(stdout, "\n");

	}

	free(v);
	free(y);
	return 0;
}
