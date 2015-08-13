#include <reproBLAS.h>
#include <idxdBLAS.h>

#include "../../config.h"

float reproBLAS_rsasum(const int N, const float* X, const int incX) {
  float_indexed *asumi = idxd_sialloc(SIDEFAULTFOLD);
  float asum;

  idxd_sisetzero(SIDEFAULTFOLD, asumi);

  idxdBLAS_sisasum(SIDEFAULTFOLD, N, X, incX, asumi);

  asum = idxd_ssiconv(SIDEFAULTFOLD, asumi);
  free(asumi);
  return asum;
}

