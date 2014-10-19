#include <rblas.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common/test_vec.h"

#include "../common/test_vecvec_header.h"

const char* vecvec_name(int argc, char** argv) {
  return "Validate rdsum, rdasum, rddot (1Big, 1BigPosNeg)";
}

int vecvec_test(int argc, char** argv, int N, int incx, int incy) {
  int i;
  double small = 1.0 / (1024.0 * 1024.0);       // 2^-20
  double big   = 1024.0 * 1024.0 * 1024.0 * 32; // 2^35
  double ref;
  double refa;
  double res;

  vec_random_seed();

  //allocate vectors
  double *x    = dvec_alloc(N, incx);
  double *y    = dvec_alloc(N, incy);

  //fill empty space with random data to check increment
  dvec_fill(N * incx, x, 1, vec_fill_RAND, 1.0, 1.0);
  dvec_fill(N * incy, y, 1, vec_fill_RAND, 1.0, 1.0);

  //fill y with 1 where necessary
  dvec_fill(N, y, incy, vec_fill_CONSTANT, 1.0, 1.0);

  //1 Big
  ref   = (N - 1) * small + big;

  //1 Big at beginning
  dvec_fill(N, x, incx, vec_fill_CONSTANT, small, 1.0);
  x[0]         = big;

  res = rdsum(N, x, incx);
  if (res != ref) {
    printf("rdsum(x) = %g != %g (1 Big at beginning)\n", res, ref);
    return 1;
  }

  res = rdasum(N, x, incx);
  if (res != ref) {
    printf("rdasum(x) = %g != %g (1 Big at beginning)\n", res, ref);
    return 1;
  }

  res = rddot(N, x, incx, y, incy);
  if (res != ref) {
    printf("rddot(x) = %g != %g (1 Big at beginning)\n", res, ref);
    return 1;
  }

  //1 Big in middle
  dvec_fill(N, x, incx, vec_fill_CONSTANT, small, 1.0);
  x[(N/2)*incx] = big;

  res = rdsum(N, x, incx);
  if (res != ref) {
    printf("rdsum(x) = %g != %g (1 Big in middle)\n", res, ref);
    return 1;
  }

  res = rdasum(N, x, incx);
  if (res != ref) {
    printf("rdasum(x) = %g != %g (1 Big in middle)\n", res, ref);
    return 1;
  }

  res = rddot(N, x, incx, y, incy);
  if (res != ref) {
    printf("rddot(x) = %g != %g (1 Big in middle)\n", res, ref);
    return 1;
  }

  //1 Big at end
  dvec_fill(N, x, incx, vec_fill_CONSTANT, small, 1.0);
  x[(N-1)*incx] = big;

  res = rdsum(N, x, incx);
  if (res != ref) {
    printf("rdsum(x) = %g != %g (1 Big at end)\n", res, ref);
    return 1;
  }

  res = rdasum(N, x, incx);
  if (res != ref) {
    printf("rdasum(x) = %g != %g (1 Big at end)\n", res, ref);
    return 1;
  }

  res = rddot(N, x, incx, y, incy);
  if (res != ref) {
    printf("rddot(x) = %g != %g (1 Big at end)\n", res, ref);
    return 1;
  }

  //1 Big pos neg
  ref   = (N - 2) * small;
  refa  = ((N - 2) * small) + 2 * big;

  //1 Big pos neg at beginning
  dvec_fill(N, x, incx, vec_fill_CONSTANT, small, 1.0);
  x[0]         = big;
  x[(N/2)*incx] = -big;

  res = rdsum(N, x, incx);
  if (res != ref) {
    printf("rdsum(x) = %g != %g (1 Big pos neg at beginning)\n", res, ref);
    return 1;
  }

  res = rdasum(N, x, incx);
  if (res != refa) {
    printf("rdasum(x) = %g != %g (1 Big pos neg at beginning)\n", res, refa);
    return 1;
  }

  res = rddot(N, x, incx, y, incy);
  if (res != ref) {
    printf("rddot(x) = %g != %g (1 Big pos neg at beginning)\n", res, ref);
    return 1;
  }

  //1 Big pos neg at ends
  dvec_fill(N, x, incx, vec_fill_CONSTANT, small, 1.0);
  x[0]         = big;
  x[(N-1)*incx] = -big;

  res = rdsum(N, x, incx);
  if (res != ref) {
    printf("rdsum(x) = %g != %g (1 Big pos neg at ends)\n", res, ref);
    return 1;
  }

  res = rdasum(N, x, incx);
  if (res != refa) {
    printf("rdasum(x) = %g != %g (1 Big pos neg at ends)\n", res, refa);
    return 1;
  }

  res = rddot(N, x, incx, y, incy);
  if (res != ref) {
    printf("rddot(x) = %g != %g (1 Big pos neg at ends)\n", res, ref);
    return 1;
  }

  //1 Big pos neg at end
  dvec_fill(N, x, incx, vec_fill_CONSTANT, small, 1.0);
  x[(N/2)*incx] = big;
  x[(N-1)*incx] = -big;

  res = rdsum(N, x, incx);
  if (res != ref) {
    printf("rdsum(x) = %g != %g (1 Big pos neg at end)\n", res, ref);
    return 1;
  }

  res = rdasum(N, x, incx);
  if (res != refa) {
    printf("rdasum(x) = %g != %g (1 Big pos neg at end)\n", res, refa);
    return 1;
  }

  res = rddot(N, x, incx, y, incy);
  if (res != ref) {
    printf("rddot(x) = %g != %g (1 Big pos neg at end)\n", res, ref);
    return 1;
  }

  free(x);
  free(y);

  return 0;
}
