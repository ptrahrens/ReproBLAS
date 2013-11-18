/*
 *  Created   13/10/25   H.D. Nguyen
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "rblas1.h"

I_double dsumI(int N, double* v, int inc) {
	I_double sum;
	dISetZero(sum);
	dsumI1(N, v, inc, DEFAULT_FOLD, 0, sum.m, sum.c);
	return sum;
}

double rdsum(int N, double* v, int inc) {
	I_double sum;
	dISetZero(sum);
	dsumI1(N, v, inc, DEFAULT_FOLD, 0, sum.m, sum.c);
	return Iconv2d(sum);
}

