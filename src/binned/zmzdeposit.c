#include <math.h>

#include <binned.h>

#include "../common/common.h"

/**
 * @internal
 * @brief  Add complex double precision to suitably binned manually specified binned complex double precision (Y += X)
 *
 * Performs the operation Y += X on an binned type Y where the index of Y is larger than the index of X
 *
 * @note This routine was provided as a means of allowing the you to optimize your code. After you have called #binned_zmzupdate() on Y with the maximum absolute value of all future elements you wish to deposit in Y, you can call #binned_zmzdeposit() to deposit a maximum of #binned_DBENDURANCE elements into Y before renormalizing Y with #binned_zmrenorm(). After any number of successive calls of #binned_zmzdeposit() on Y, you must renormalize Y with #binned_zmrenorm() before using any other function on Y.
 *
 * @param fold the fold of the binned types
 * @param X scalar X
 * @param priY Y's primary vector
 * @param incpriY stride within Y's primary vector (use every incpriY'th element)
 *
 * @author Hong Diep Nguyen
 * @author Willow Ahrens
 * @date   10 Jun 2015
 */
void binned_zmzdeposit(const int fold, const void *X, double *priY, const int incpriY){
  double MR, MI;
  long_double qR, qI;
  int i;
  double xR = ((double*)X)[0];
  double xI = ((double*)X)[1];

  if (ISNANINF(xR) || ISNANINF(priY[0])) {
    priY[0] += xR;
    binned_dmddeposit(fold, xI, priY + 1, 2 * incpriY);
    return;
  }
  if (ISNANINF(xI) || ISNANINF(priY[1])) {
    priY[1] += xI;
    binned_dmddeposit(fold, xR, priY, 2 * incpriY);
    return;
  }

  if(binned_dmindex0(priY) || binned_dmindex0(priY + 1)){
    binned_dmddeposit(fold, xR, priY, 2 * incpriY);
    binned_dmddeposit(fold, xI, priY + 1, 2 * incpriY);
    return;
  }

  for (i = 0; i < fold - 1; i++) {
    MR = priY[i * 2 * incpriY];
    MI = priY[i * 2 * incpriY + 1];
    qR.d = xR;
    qI.d = xI;
    qR.l |= 1;
    qI.l |= 1;
    qR.d += MR;
    qI.d += MI;
    priY[i * 2 * incpriY] = qR.d;
    priY[i * 2 * incpriY + 1] = qI.d;
    MR -= qR.d;
    MI -= qI.d;
    xR += MR;
    xI += MI;
  }
  qR.d = xR;
  qI.d = xI;
  qR.l |= 1;
  qI.l |= 1;
  priY[i * 2 * incpriY] += qR.d;
  priY[i * 2 * incpriY + 1] += qI.d;
}
