/*
 *  Created   13/10/25   H.D. Nguyen
 */

#include "indexed.h"

/**
 * @internal
 * @brief Convert double precision to manually specified indexed double precision (X -> Y)
 *
 * @param fold the fold of the indexed types
 * @param X scalar X
 * @param manY Y's mantissa vector
 * @param incmanY stride within Y's mantissa vector (use every incmanY'th element)
 * @param carY Y's carry vector
 * @param inccarY stride within Y's carry vector (use every inccarY'th element)
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   30 Apr 2015
 */
void dmdconv(const int fold, const double X, double* manY, const int incmanY, double* carY, const int inccarY) {
  dmsetzero(fold, manY, incmanY, carY, inccarY);
  dmdadd(fold, X, manY, incmanY, carY, inccarY);
}

/**
 * @brief Convert double precision to indexed double precision (X -> Y)
 *
 * @param fold the fold of the indexed types
 * @param X scalar X
 * @param Y indexed scalar Y
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
void didconv(const int fold, const double X, double_indexed *Y) {
  dmdconv(fold, X, Y, 1, Y + fold, 1);
}

/**
 * @internal
 * @brief Convert complex double precision to manually specified indexed complex double precision (X -> Y)
 *
 * @param fold the fold of the indexed types
 * @param X scalar X
 * @param manY Y's mantissa vector
 * @param incmanY stride within Y's mantissa vector (use every incmanY'th element)
 * @param carY Y's carry vector
 * @param inccarY stride within Y's carry vector (use every inccarY'th element)
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
void zmzconv(const int fold, const void *X, double *manY, const int incmanY, double *carY, const int inccarY) {
  dmdconv(fold, ((double*)X)[0], manY, incmanY * 2, carY, inccarY * 2);
  dmdconv(fold, ((double*)X)[1], manY + 1, incmanY * 2, carY + 1, inccarY * 2);
}

/**
 * @brief Convert complex double precision to indexed complex double precision (X -> Y)
 *
 * @param fold the fold of the indexed types
 * @param X scalar X
 * @param Y indexed scalar Y
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
void zizconv(const int fold, const void *X, double_complex_indexed *Y) {
  zmzconv(fold, X, Y, 1, Y + 2 * fold, 1);
}

static void ddpd(double* a, double b) {
  double q;
  double s1, s2, t1, t2;

  // Add hi words
  s1 = a[0] + b;
  q = s1 - a[0];
  s2 = ((b - q) + (a[0] - (s1 - q)));

  t1 = a[1] + s2;
  q = t1 - a[1];
  t2 = ((s2 - q) + (a[1] - (t1 - q)));

  s2 = t1;

  // Renormalize (s1, s2)  to  (t1, s2)
  t1 = s1 + s2;
  t2 += s2 - (t1 - s1);

  // Renormalize (t1, t2)
  a[0] = t1 + t2;
  a[1] = t2 - (a[0] - t1);
}

/**
 * @internal
 * @brief Convert manually specified indexed double precision to double precision (X -> Y)
 *
 * @param fold the fold of the indexed types
 * @param manX X's mantissa vector
 * @param incmanX stride within X's mantissa vector (use every incmanX'th element)
 * @param carX X's carry vector
 * @param inccarX stride within X's carry vector (use every inccarX'th element)
 * @return scalar Y
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
double ddmconv(const int fold, const double* manX, const int incmanX, const double* carX, const int inccarX) {
  int i = 0;
  long double Y = 0.0;
  double M;

  if (isinf(manX[0]) || isnan(manX[0]))
    return manX[0];

  if (manX[0] == 0.0) {
    return 0.0;
  }

  if(dmindex0(manX)){
    M = ufp(manX[i * incmanX]);
    Y += (carX[i * inccarX] * 0.25 * M + (manX[i * incmanX] - 1.5 * M)) * dmexpansion();
    i = 1;
  }

  for (; i < fold; i++) {
    M = ufp(manX[i * incmanX]);
    Y += carX[i * inccarX] * 0.25 * M + (manX[i * incmanX] - 1.5 * M);
  }

  return Y;
}

/**
 * @brief Convert indexed double precision to double precision (X -> Y)
 *
 * @param fold the fold of the indexed types
 * @param X indexed scalar X
 * @return scalar Y
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
double ddiconv(const int fold, const double_indexed *X) {
  return ddmconv(fold, X, 1, X + fold, 1);
}

/**
 * @internal
 * @brief Convert manually specified indexed complex double precision to complex double precision (X -> Y)
 *
 * @param fold the fold of the indexed types
 * @param manX X's mantissa vector
 * @param incmanX stride within X's mantissa vector (use every incmanX'th element)
 * @param carX X's carry vector
 * @param inccarX stride within X's carry vector (use every inccarX'th element)
 * @param conv scalar return
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
void zzmconv_sub(const int fold, const double *manX, const int incmanX, const double *carX, const int inccarX, void *conv) {
  ((double*)conv)[0] = ddmconv(fold, manX, incmanX * 2, carX, inccarX + 1);
  ((double*)conv)[1] = ddmconv(fold, manX + 1, incmanX * 2, carX + 1, inccarX + 1);
}

/**
 * @brief Convert indexed complex double precision to complex double precision (X -> Y)
 *
 * @param fold the fold of the indexed types
 * @param X indexed scalar X
 * @param conv scalar return
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
void zziconv_sub(const int fold, const double_complex_indexed *X, void *conv) {
  zzmconv_sub(fold, X, 1, X + 2 * fold, 1, conv);
}
