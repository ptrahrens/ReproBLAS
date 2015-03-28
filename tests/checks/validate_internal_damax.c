#include <indexedBLAS.h>
#include <indexed.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/test_vecvec_header.h"

int vecvec_show_help(void){
  return 0;
}

const char* vecvec_name(int argc, char** argv) {
  (void)argc;
  (void)argv;
  return "Validate damax(m) (1Big)";
}

int vecvec_test(int argc, char** argv, int N, int incx, int incy) {
  (void)argc;
  (void)argv;
  double small = 1.0 / (1024.0 * 1024.0);       // 2^-20
  double big   = 1024.0 * 1024.0 * 1024.0 * 32; // 2^35
  double ref;
  double res;

  util_random_seed();

  //allocate vectors
  double *x    = util_dvec_alloc(N, incx);
  double *y    = util_dvec_alloc(N, incy);

  //fill y with 1 where necessary
  util_dvec_fill(N, y, incy, util_Vec_Constant, 1, 1.0);

  //1 Big
  ref   = big;

  //1 Big at beginning
  util_dvec_fill(N, x, incx, util_Vec_Constant, small, 1.0);
  x[0]         = -big;

  res = damax(N, x, incx);
  if (res != ref) {
    printf("damax(x) = %g != %g (1 Big at beginning)\n", res, ref);
    return 1;
  }

  res = damaxm(N, x, incx, y, incy);
  if (res != ref) {
    printf("damaxm(x) = %g != %g (1 Big at beginning)\n", res, ref);
    return 1;
  }

  //1 Big at end
  util_dvec_fill(N, x, incx, util_Vec_Constant, small, 1.0);
  x[(N-1)*incx]         = -big;

  res = damax(N, x, incx);
  if (res != ref) {
    printf("damax(x) = %g != %g (1 Big at end)\n", res, ref);
    return 1;
  }

  res = damaxm(N, x, incx, y, incy);
  if (res != ref) {
    printf("damaxm(x) = %g != %g (1 Big at end)\n", res, ref);
    return 1;
  }

  free(x);
  free(y);

  return 0;
}
