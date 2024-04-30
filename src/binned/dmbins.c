#include <math.h>

#include <binned.h>

#include "../../config.h"

static double bins[binned_DBMAXINDEX + binned_DBMAXFOLD];
static int bins_initialized = 0;

/**
 * @internal
 * @brief Get binned double precision reference bins
 *
 * returns a pointer to the bins corresponding to the given index
 *
 * @param X index
 * @return pointer to constant double precision bins of index X
 *
 * @author Willow Ahrens
 * @author Hong Diep Nguyen
 * @date   19 Jun 2015
 */
const double *binned_dmbins(const int X) {
  int index;

  if (!bins_initialized) {
    bins[0] = 2.0 * ldexp(0.75, DBL_MAX_EXP - 1);
    for(index = 1; index <= binned_DBMAXINDEX; index++){
      bins[index] = ldexp(0.75, (DBL_MAX_EXP + DBL_MANT_DIG - DBWIDTH + 1 - index * DBWIDTH));
    }
    for(; index < binned_DBMAXINDEX + binned_DBMAXFOLD; index++){
      bins[index] = bins[index - 1];
    }

    bins_initialized = 1;
  }

  return (const double*)bins + X;
}
