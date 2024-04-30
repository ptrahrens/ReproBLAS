#include <binned.h>

/**
 * @internal
 * @brief rescale manually specified binned complex single precision sum of squares
 *
 * Rescale an binned complex single precision sum of squares Y
 *
 * @param fold the fold of the binned types
 * @param X Y's new scaleY (X == #binned_sscale (f) for some @c float f) (X >= scaleY)
 * @param scaleY Y's current scaleY (scaleY == #binned_sscale (f) for some @c float f) (X >= scaleY)
 * @param priY Y's primary vector
 * @param incpriY stride within Y's primary vector (use every incpriY'th element)
 * @param carY Y's carry vector
 * @param inccarY stride within Y's carry vector (use every inccarY'th element)
 *
 * @author Willow Ahrens
 * @date   19 Jun 2015
 */
void binned_cmsrescale(const int fold, const float X, const float scaleY, float *priY, const int incpriY, float *carY, const int inccarY){
  int i;
  float rescaleY;

  if(X == scaleY || X == 0.0 || scaleY == 0.0){
    return;
  }

  rescaleY = X/scaleY;
  rescaleY *= rescaleY;
  for(i = 0; i < fold; i++){
    priY[i * 2 * incpriY] /= rescaleY;
    priY[i * 2 * incpriY + 1] /= rescaleY;
    if(priY[i * incpriY] == 0.0){
      binned_cmsupdate(fold - i, 0.0, priY + i * 2 * incpriY, incpriY, carY + i * 2 * inccarY, inccarY);
      return;
    }
  }
}
