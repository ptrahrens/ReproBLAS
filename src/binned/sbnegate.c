#include <binned.h>

/**
 * @brief Negate binned single precision (X = -X)
 *
 * Performs the operation X = -X
 *
 * @param fold the fold of the binned types
 * @param X binned scalar X
 *
 * @author Hong Diep Nguyen
 * @author Willow Ahrens
 * @date   27 Apr 2015
 */
void binned_sbnegate(const int fold, float_binned* X){
  binned_smnegate(fold, X, 1, X + fold, 1);
}
