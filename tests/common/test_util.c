#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <float.h>
#include <limits.h>

#include "test_util.h"

const int util_vec_fill_n_names  = 27;
const char *util_vec_fill_names[] = {"constant",
                                     "+inf",
                                     "++inf",
                                     "+-inf",
                                     "nan",
                                     "+inf_nan",
                                     "++inf_nan",
                                     "+-inf_nan",
                                     "+big",
                                     "++big",
                                     "+-big",
                                     "rand",
                                     "2*rand-1",
                                     "rand+(rand-1)",
                                     "normal",
                                     "sine",
                                     "small+grow*big",
                                     "small+rand*big",
                                     "rand_cond",
                                     "N_cond",
                                     "N_cond+rand",
                                     "full_range",
                                     "half_range",
                                     "mountain",
                                     "constant[drop]",
                                     "rand[drop]",
                                     "2*rand-1[drop]",
                                     "rand+(rand-1)[drop]",
                                     "normal[drop]",
                                     "sine[drop]"};
const char *util_vec_fill_descs[] = {"Constant",
                                     "+Inf",
                                     "++Inf",
                                     "+-Inf",
                                     "NaN",
                                     "+Inf_NaN",
                                     "++Inf_NaN",
                                     "+-Inf_NaN",
                                     "+Big",
                                     "++Big",
                                     "+-Big",
                                     "Random",
                                     "2*Random-1",
                                     "Random+(Random-1)",
                                     "Normal",
                                     "Sine(2pi*(i/n))",
                                     "Small+(i/n)*Big",
                                     "Small+Rand*Big",
                                     "RandomConditioned",
                                     "NConditioned",
                                     "NConditioned+Rand",
                                     "FullRange",
                                     "HalfRange",
                                     "Mountain",
                                     "Constant[drop]",
                                     "Random[drop]",
                                     "2*Random-1[drop]",
                                     "Random+(Random-1)[drop]",
                                     "Normal[drop]",
                                     "Sine(2pi*(i/n))[drop]"};

const int  util_mat_fill_n_names  = 28;
const char *util_mat_fill_names[] = {"constant",
                                     "+inf",
                                     "++inf",
                                     "+-inf",
                                     "nan",
                                     "+inf_nan",
                                     "++inf_nan",
                                     "+-inf_nan",
                                     "+big",
                                     "++big",
                                     "+-big",
                                     "rand",
                                     "2*rand-1",
                                     "rand+(rand-1)",
                                     "normal",
                                     "sine",
                                     "small+grow*big",
                                     "small+rand*big",
                                     "rand_cond",
                                     "N_cond",
                                     "N_cond+rand",
                                     "full_range",
                                     "half_range",
                                     "mountain",
                                     "constant[drop]",
                                     "rand[drop]",
                                     "2*rand-1[drop]",
                                     "rand+(rand-1)[drop]",
                                     "normal[drop]",
                                     "sine[drop]",
                                     "identity"};
const char *util_mat_fill_descs[] = {"Constant",
                                     "+Inf",
                                     "++Inf",
                                     "+-Inf",
                                     "NaN",
                                     "+Inf_NaN",
                                     "++Inf_NaN",
                                     "+-Inf_NaN",
                                     "+Big",
                                     "++Big",
                                     "+-Big",
                                     "Random",
                                     "2*Random-1",
                                     "Random+(Random-1)",
                                     "Normal",
                                     "Sine(2pi*(i/n))",
                                     "Small+(i/n)*Big",
                                     "Small+Rand*Big",
                                     "RandomConditioned",
                                     "NConditioned",
                                     "NConditioned+Rand",
                                     "FullRange",
                                     "HalfRange",
                                     "Mountain",
                                     "Constant[drop]",
                                     "Random[drop]",
                                     "2*Random-1[drop]",
                                     "Random+(Random-1)[drop]",
                                     "Normal[drop]",
                                     "Sine(2pi*(i/n))[drop]",
                                     "Identity"};

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */
#define MT_N 624
#define MT_M 397
#define MT_MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define MT_UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define MT_LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[MT_N]; /* the array for the state vector  */
static int mti=MT_N+1; /* mti==MT_N+1 means mt[MT_N] is not initialized */

/* initializes mt[MT_N] with a seed */
static void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<MT_N; mti++) {
        mt[mti] =
      (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* generates a random number on [0,0xffffffff]-interval */
static unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MT_MATRIX_A};
    /* mag01[x] = x * MT_MATRIX_A  for x=0,1 */

    if (mti >= MT_N) { /* generate MT_N words at one time */
        int kk;

        if (mti == MT_N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<MT_N-MT_M;kk++) {
            y = (mt[kk]&MT_UPPER_MASK)|(mt[kk+1]&MT_LOWER_MASK);
            mt[kk] = mt[kk+MT_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<MT_N-1;kk++) {
            y = (mt[kk]&MT_UPPER_MASK)|(mt[kk+1]&MT_LOWER_MASK);
            mt[kk] = mt[kk+(MT_M-MT_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[MT_N-1]&MT_UPPER_MASK)|(mt[0]&MT_LOWER_MASK);
        mt[MT_N-1] = mt[MT_M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,1) with 53-bit resolution*/
static double genrand_res53(void)
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6;
    return(a*67108864.0+b)*(1.0/9007199254740992.0);
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

void util_random_seed(void) {
  struct timeval st;
  gettimeofday( &st, NULL );
  init_genrand((unsigned long)(st.tv_usec + 1e6*st.tv_sec));
}

double util_drand(){
  return genrand_res53();
}

int util_rand(int range){
  long r;
  do{
      r = genrand_int32();
  }while(r >= (0xffffffff/range)*range);
  return r % range;
}

void util_ddpd(double* a, double b) {
  double q;
  double s1, s2, t1, t2;

  s1 = a[0] + b;
  q = s1 - a[0];
  s2 = ((b - q) + (a[0] - (s1 - q)));

  t1 = a[1] + s2;
  q = t1 - a[1];
  t2 = ((s2 - q) + (a[1] - (t1 - q)));

  s2 = t1;

  t1 = s1 + s2;
  t2 += s2 - (t1 - s1);

  a[0] = t1 + t2;
  a[1] = t2 - (a[0] - t1);
}

double complex zmul(double complex a, double complex b){
  double complex c = 0.0;
  ((double*)&c)[0] = ((double*)&a)[0] * ((double*)&b)[0] - ((double*)&a)[1] * ((double*)&b)[1];
  ((double*)&c)[1] = ((double*)&a)[0] * ((double*)&b)[1] + ((double*)&a)[1] * ((double*)&b)[0];
  return c;
}

float complex cmul(float complex a, float complex b){
  float complex c = 0.0;
  ((float*)&c)[0] = ((float*)&a)[0] * ((float*)&b)[0] - ((float*)&a)[1] * ((float*)&b)[1];
  ((float*)&c)[1] = ((float*)&a)[0] * ((float*)&b)[1] + ((float*)&a)[1] * ((float*)&b)[0];
  return c;
}

void util_dsplit(double a, double *a_l, double *a_t){
  if (fabs(a) >= DBL_MAX/(1ull << ((DBL_MANT_DIG + 3)/2))){
    util_dsplit(a/(1ull << ((DBL_MANT_DIG + 3)/2)), a_l, a_t);
    *a_l *= (1ull << ((DBL_MANT_DIG + 3)/2));
    *a_t *= (1ull << ((DBL_MANT_DIG + 3)/2));
    return;
  }
  double t = ((double)((1ull << ((DBL_MANT_DIG + 1)/2)) + 1)) * a;
  *a_l = t - (t - a);
  *a_t = a - *a_l;
}

void util_zsplit(double complex a, double complex *a_l, double complex *a_t){
  util_dsplit(((double*)&a)[0], ((double*)a_l) + 0, ((double*)a_t) + 0);
  util_dsplit(((double*)&a)[1], ((double*)a_l) + 1, ((double*)a_t) + 1);
}

void util_ssplit(float a, float *a_l, float *a_t){
  if (fabsf(a) >= FLT_MAX/(1ull << ((FLT_MANT_DIG + 3)/2))){
    util_ssplit(a/(1ull << ((FLT_MANT_DIG + 3)/2)), a_l, a_t);
    *a_l *= (1ull << ((FLT_MANT_DIG + 3)/2));
    *a_t *= (1ull << ((FLT_MANT_DIG + 3)/2));
    return;
  }
  float t = ((float)((1ull << ((FLT_MANT_DIG + 1)/2)) + 1)) * a;
  *a_l = t - (t - a);
  *a_t = a - *a_l;
}

void util_csplit(float complex a, float complex *a_l, float complex *a_t){
  util_ssplit(((float*)&a)[0], ((float*)a_l) + 0, ((float*)a_t) + 0);
  util_ssplit(((float*)&a)[1], ((float*)a_l) + 1, ((float*)a_t) + 1);
}

void util_dvec_dotsplit_twins(int N, double *V, int incV, double *U, int incU){
  for(int i = 0; i < N; i++){
    util_dsplit(V[i], U + 0, U + 2 * incU);
    util_dsplit(V[i], U + incU, U + 3 * incU);
    U += 4 * incU;
  }
}

void util_dvec_dotsplit_couples(int N, double *V, int incV, double *U, int incU){
  for(int i = 0; i < N; i++){
    util_dsplit(V[i], U + 0, U + incU);
    util_dsplit(V[i], U + 2 * incU, U + 3 * incU);
    U += 4 * incU;
  }
}

void util_zvec_dotsplit_twins(int N, double complex *V, int incV, double complex *U, int incU){
  for(int i = 0; i < N; i++){
    util_zsplit(V[i], U + 0, U + 2 * incU);
    util_zsplit(V[i], U + incU, U + 3 * incU);
    U += 4 * incU;
  }
}

void util_zvec_dotsplit_couples(int N, double complex *V, int incV, double complex *U, int incU){
  for(int i = 0; i < N; i++){
    util_zsplit(V[i], U + 0, U + incU);
    util_zsplit(V[i], U + 2 * incU, U + 3 * incU);
    U += 4 * incU;
  }
}

void util_svec_dotsplit_twins(int N, float *V, int incV, float *U, int incU){
  for(int i = 0; i < N; i++){
    util_ssplit(V[i], U + 0, U + 2 * incU);
    util_ssplit(V[i], U + incU, U + 3 * incU);
    U += 4 * incU;
  }
}

void util_svec_dotsplit_couples(int N, float *V, int incV, float *U, int incU){
  for(int i = 0; i < N; i++){
    util_ssplit(V[i], U + 0, U + incU);
    util_ssplit(V[i], U + 2 * incU, U + 3 * incU);
    U += 4 * incU;
  }
}

void util_cvec_dotsplit_twins(int N, float complex *V, int incV, float complex *U, int incU){
  for(int i = 0; i < N; i++){
    util_csplit(V[i], U + 0, U + 2 * incU);
    util_csplit(V[i], U + incU, U + 3 * incU);
    U += 4 * incU;
  }
}

void util_cvec_dotsplit_couples(int N, float complex *V, int incV, float complex *U, int incU){
  for(int i = 0; i < N; i++){
    util_csplit(V[i], U + 0, U + incU);
    util_csplit(V[i], U + 2 * incU, U + 3 * incU);
    U += 4 * incU;
  }
}

int util_dsoftequals(double a, double b, double bound){
  if(isnan(a) && isnan(b)){
    return 1;
  }
  if(isinf(a) && isinf(b) && ((a > 0.0 && b > 0.0) || (a < 0.0 && b < 0.0))){
    return 1;
  }
  if(fabs(a - b) <= bound){
    return 1;
  }
  return 0;
}

int util_zsoftequals(double complex a, double complex b, double complex bound){
  return util_dsoftequals(creal(a), creal(b), creal(bound)) && util_dsoftequals(cimag(a), cimag(b), cimag(bound));
}

int util_ssoftequals(float a, float b, float bound){
  if(isnan(a) && isnan(b)){
    return 1;
  }
  if(isinf(a) && isinf(b) && ((a > 0.0 && b > 0.0) || (a < 0.0 && b < 0.0))){
    return 1;
  }
  if(fabs(a - b) <= bound){
    return 1;
  }
  return 0;
}

int util_csoftequals(float complex a, float complex b, float complex bound){
  return util_ssoftequals(crealf(a), crealf(b), crealf(bound)) && util_ssoftequals(cimagf(a), cimagf(b), cimagf(bound));
}

typedef int (*compare_func)(int i, int j, void *data);
typedef void (*swap_func)(int i, int j, void *data);
typedef int (*elem_compare_func)(void *a, void *b, util_comp_t comp);

int util_dcompare(void *a, void *b, util_comp_t comp){
  double a_prime;
  double b_prime;
  switch(comp){
    case util_Increasing:
      if(*((double*)a) < *((double*)b)){
        return -1;
      }else if(*((double*)a) > *((double*)b)){
        return 1;
      }else{
        return 0;
      }
    case util_Decreasing:
      return -1 * util_dcompare(a, b, util_Increasing);
    case util_Increasing_Magnitude:
      a_prime = fabs(*((double*)a));
      b_prime = fabs(*((double*)b));
      return util_dcompare(&a_prime, &b_prime, util_Increasing);
    case util_Decreasing_Magnitude:
      return -1 * util_dcompare(a, b, util_Increasing_Magnitude);
    default:
      fprintf(stderr, "error: default case (%s:%d)\n", __FILE__, __LINE__);
      exit(125);
  }
}

int util_scompare(void *a, void *b, util_comp_t comp){
  float a_prime;
  float b_prime;
  switch(comp){
    case util_Increasing:
      if(*((float*)a) < *((float*)b)){
        return -1;
      }else if(*((float*)a) > *((float*)b)){
        return 1;
      }else{
        return 0;
      }
    case util_Decreasing:
      return -1 * util_scompare(a, b, util_Increasing);
    case util_Increasing_Magnitude:
      a_prime = fabsf(*((float*)a));
      b_prime = fabsf(*((float*)b));
      return util_scompare(&a_prime, &b_prime, util_Increasing);
    case util_Decreasing_Magnitude:
      return -1 * util_scompare(a, b, util_Increasing_Magnitude);
    default:
      fprintf(stderr, "error: default case (%s:%d)\n", __FILE__, __LINE__);
      exit(125);
  }
}

int util_zcompare(void *a, void *b, util_comp_t comp){
  double a_prime;
  double b_prime;
  switch(comp){
    case util_Increasing:
      a_prime = creal(*((double complex*)a));
      b_prime = creal(*((double complex*)b));
      return util_dcompare(&a_prime, &b_prime, util_Increasing);
    case util_Decreasing:
      return -1 * util_zcompare(a, b, util_Increasing);
    case util_Increasing_Magnitude:
      a_prime = fabs(((double*)a)[0]) + fabs(((double*)a)[1]);
      b_prime = fabs(((double*)b)[0]) + fabs(((double*)b)[1]);
      return util_dcompare(&a_prime, &b_prime, util_Increasing);
    case util_Decreasing_Magnitude:
      return -1 * util_zcompare(a, b, util_Increasing_Magnitude);
    default:
      fprintf(stderr, "error: default case (%s:%d)\n", __FILE__, __LINE__);
      exit(125);
  }
}

int util_ccompare(void *a, void *b, util_comp_t comp){
  float a_prime;
  float b_prime;
  switch(comp){
    case util_Increasing:
      a_prime = crealf(*((float complex*)a));
      b_prime = crealf(*((float complex*)b));
      return util_scompare(&a_prime, &b_prime, util_Increasing);
    case util_Decreasing:
      return -1 * util_ccompare(a, b, util_Increasing);
    case util_Increasing_Magnitude:
      a_prime = fabsf(((float*)a)[0]) + fabsf(((float*)a)[1]);
      b_prime = fabsf(((float*)b)[0]) + fabsf(((float*)b)[1]);
      return util_scompare(&a_prime, &b_prime, util_Increasing);
    case util_Decreasing_Magnitude:
      return -1 * util_ccompare(a, b, util_Increasing_Magnitude);
    default:
      fprintf(stderr, "error: default case (%s:%d)\n", __FILE__, __LINE__);
      exit(125);
  }
}

/**
 * @brief a quicksort using the extra permutation infastructure that I implemented.
 *
 * @author Willow Ahrens
 * @author Logic lovingly stolen from Jonathan Shewchuk's CS61B notes from 2013
 */
static void quicksort(int low, int high, compare_func compare, void *compare_data, swap_func swap, void *swap_data) {
  int i, j;

  if (low >= high) {
    return;
  }

  i = low + (util_rand(high - low));

  swap(i, high, swap_data);

  i = low - 1;
  j = high;
  do{
    do {
      i++;
    } while (compare(i, high, compare_data) < 0);
    do {
      j--;
    } while (compare(j, high, compare_data) > 0 && j > low);

    if (i < j) {
      swap(i, j, swap_data);
    }
  }while (i < j);

  swap(i, high, swap_data);
  quicksort(low, i - 1, compare, compare_data, swap, swap_data);
  quicksort(i + 1, high, compare, compare_data, swap, swap_data);
}

static void sort(int N, compare_func compare, void *compare_data, swap_func swap, void *swap_data) {
  quicksort(0, N - 1, compare, compare_data, swap, swap_data);
}

static void elem_swap(void *a, void *b, size_t elem_size){
  void *t = malloc(elem_size);
  if(a != b){
    memcpy(t, b, elem_size);
    memcpy(b, a, elem_size);
    memcpy(a, t, elem_size);
  }
  free(t);
}

typedef struct vec_swap_data{
  void   *V;
  int    incV;
  size_t elem_size;
  int    *P;
  int    incP;
} vec_swap_data_t;

typedef struct mat_row_swap_data{
  char     Order;
  char TransA;
  int               M;
  int               N;
  void              *A;
  int               lda;
  size_t            elem_size;
  int               *P;
  int               incP;
} mat_row_swap_data_t;

static void vec_swap(int a, int b, void *data){
  vec_swap_data_t *d = (vec_swap_data_t*)data;
  elem_swap((void*)((char*)d->V + a * d->incV * d->elem_size), (void*)((char*)d->V + b * d->incV * d->elem_size), d->elem_size);
  if(d->P){
    elem_swap(d->P + a * d->incP, d->P + b * d->incP, sizeof(int));
  }
}

static void mat_row_swap(int a, int b, void *data){
  int i;
  mat_row_swap_data_t *d = (mat_row_swap_data_t*)data;
  switch(d->Order){
    case 'r':
    case 'R':
      switch(d->TransA){
        case 'n':
        case 'N':
          elem_swap((void*)((char*)d->A + a * d->lda * d->elem_size), (void*)((char*)d->A + b * d->lda * d->elem_size), d->N * d->elem_size);
          break;
        default:
          for(i = 0; i < d->M; i++){
            elem_swap((void*)((char*)d->A + (a + (i * d->lda)) * d->elem_size), (void*)((char*)d->A + (b + (i * d->lda)) * d->elem_size), d->elem_size);
          }
          break;
      }
      break;
    default:
      switch(d->TransA){
        case 'n':
        case 'N':
          for(i = 0; i < d->N; i++){
            elem_swap((void*)((char*)d->A + (a + (i * d->lda)) * d->elem_size), (void*)((char*)d->A + (b + (i * d->lda)) * d->elem_size), d->elem_size);
          }
          break;
        default:
          elem_swap((void*)((char*)d->A + a * d->lda * d->elem_size), (void*)((char*)d->A + b * d->lda * d->elem_size), d->M * d->elem_size);
          break;
      }
      break;
  }
  if(d->P){
    elem_swap(d->P + a * d->incP, d->P + b * d->incP, sizeof(int));
  }
}

typedef struct vec_compare_data{
  void              *V;
  int               incV;
  size_t            elem_size;
  elem_compare_func elem_compare;
  util_comp_t       comp;
} vec_compare_data_t;

typedef struct mat_row_compare_data{
  char              Order;
  char              TransA;
  int               M;
  int               N;
  void              *A;
  int               lda;
  size_t            elem_size;
  elem_compare_func elem_compare;
  util_comp_t       comp;
  int               col;
} mat_row_compare_data_t;

static int vec_compare(int a, int b, void *data){
  vec_compare_data_t *d = (vec_compare_data_t*)data;
  return d->elem_compare((void*)((char*)d->V + a * d->incV * d->elem_size), (void*)((char*)d->V + b * d->incV * d->elem_size), d->comp);
}

static int mat_row_compare(int a, int b, void *data){
  mat_row_compare_data_t *d = (mat_row_compare_data_t*)data;
  switch(d->Order){
    case 'r':
    case 'R':
      switch(d->TransA){
        case 'n':
        case 'N':
          return d->elem_compare((void*)((char*)d->A + (a * d->lda + d->col) * d->elem_size), (void*)((char*)d->A + (b * d->lda + d->col) * d->elem_size), d->comp);
        default:
          return d->elem_compare((void*)((char*)d->A + (a + d->lda * d->col) * d->elem_size), (void*)((char*)d->A + (b + d->lda * d->col) * d->elem_size), d->comp);
      }
      break;
    default:
      switch(d->TransA){
        case 'n':
        case 'N':
          return d->elem_compare((void*)((char*)d->A + (a + d->lda * d->col) * d->elem_size), (void*)((char*)d->A + (b + d->lda * d->col) * d->elem_size), d->comp);
        default:
          return d->elem_compare((void*)((char*)d->A + (a * d->lda + d->col) * d->elem_size), (void*)((char*)d->A + (b * d->lda + d->col) * d->elem_size), d->comp);
      }
      break;
  }
}

static int mat_row_permute_size(char Order, char TransA, int M, int N){
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          return M;
        default:
          return N;
      }
    default:
      switch(TransA){
        case 'n':
        case 'N':
          return M;
        default:
          return N;
      }
  }
}

void util_dvec_sort(int N, double *V, int incV, int *P, int incP, util_comp_t comp){
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(double),
                               .P         = P,
                               .incP      = incP};
  vec_compare_data_t compare_data = {.V            = V,
                                     .incV         = incV,
                                     .elem_size    = sizeof(double),
                                     .elem_compare = util_dcompare,
                                     .comp         = comp};
  sort(N, &vec_compare, &compare_data, &vec_swap, &swap_data);
}

void util_svec_sort(int N, float *V, int incV, int *P, int incP, util_comp_t comp){
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(float),
                               .P         = P,
                               .incP      = incP};
  vec_compare_data_t compare_data = {.V            = V,
                                     .incV         = incV,
                                     .elem_size    = sizeof(float),
                                     .elem_compare = util_scompare,
                                     .comp         = comp};
  sort(N, &vec_compare, &compare_data, &vec_swap, &swap_data);
}

void util_zvec_sort(int N, double complex *V, int incV, int *P, int incP, util_comp_t comp){
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(double complex),
                               .P         = P,
                               .incP      = incP};
  vec_compare_data_t compare_data = {.V            = V,
                                     .incV         = incV,
                                     .elem_size    = sizeof(double complex),
                                     .elem_compare = util_zcompare,
                                     .comp         = comp};
  sort(N, &vec_compare, &compare_data, &vec_swap, &swap_data);
}

void util_cvec_sort(int N, float complex *V, int incV, int *P, int incP, util_comp_t comp){
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(float complex),
                               .P         = P,
                               .incP      = incP};
  vec_compare_data_t compare_data = {.V            = V,
                                     .incV         = incV,
                                     .elem_size    = sizeof(float complex),
                                     .elem_compare = util_ccompare,
                                     .comp         = comp};
  sort(N, &vec_compare, &compare_data, &vec_swap, &swap_data);
}

void util_dmat_row_sort(char Order, char TransA, int M, int N, double *A, int lda, int *P, int incP, util_comp_t comp, int col){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(double),
                                   .P         = P,
                                   .incP      = incP};
  mat_row_compare_data_t compare_data = {.Order        = Order,
                                         .TransA       = TransA,
                                         .M            = M,
                                         .N            = N,
                                         .A            = A,
                                         .lda          = lda,
                                         .elem_size    = sizeof(double),
                                         .elem_compare = util_dcompare,
                                         .comp         = comp,
                                         .col          = col};
  sort(mat_row_permute_size(Order, TransA, M, N), &mat_row_compare, &compare_data, &mat_row_swap, &swap_data);
}

void util_smat_row_sort(char Order, char TransA, int M, int N, float *A, int lda, int *P, int incP, util_comp_t comp, int col){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(float),
                                   .P         = P,
                                   .incP      = incP};
  mat_row_compare_data_t compare_data = {.Order        = Order,
                                         .TransA       = TransA,
                                         .M            = M,
                                         .N            = N,
                                         .A            = A,
                                         .lda          = lda,
                                         .elem_size    = sizeof(float),
                                         .elem_compare = util_scompare,
                                         .comp         = comp,
                                         .col          = col};
  sort(mat_row_permute_size(Order, TransA, M, N), &mat_row_compare, &compare_data, &mat_row_swap, &swap_data);
}

void util_zmat_row_sort(char Order, char TransA, int M, int N, double complex *A, int lda, int *P, int incP, util_comp_t comp, int col){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(double complex),
                                   .P         = P,
                                   .incP      = incP};
  mat_row_compare_data_t compare_data = {.Order        = Order,
                                         .TransA       = TransA,
                                         .M            = M,
                                         .N            = N,
                                         .A            = A,
                                         .lda          = lda,
                                         .elem_size    = sizeof(double complex),
                                         .elem_compare = util_zcompare,
                                         .comp         = comp,
                                         .col          = col};
  sort(mat_row_permute_size(Order, TransA, M, N), &mat_row_compare, &compare_data, &mat_row_swap, &swap_data);
}

void util_cmat_row_sort(char Order, char TransA, int M, int N, float complex *A, int lda, int *P, int incP, util_comp_t comp, int col){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(float complex),
                                   .P         = P,
                                   .incP      = incP};
  mat_row_compare_data_t compare_data = {.Order        = Order,
                                         .TransA       = TransA,
                                         .M            = M,
                                         .N            = N,
                                         .A            = A,
                                         .lda          = lda,
                                         .elem_size    = sizeof(float complex),
                                         .elem_compare = util_ccompare,
                                         .comp         = comp,
                                         .col          = col};
  sort(mat_row_permute_size(Order, TransA, M, N), &mat_row_compare, &compare_data, &mat_row_swap, &swap_data);
}

static void reverse(int N, swap_func swap, void *swap_data) {
  int i;
  for (i = 0 ; i < N / 2; i++)
  {
    swap(i, N -i-1, swap_data);
  }
}

void util_dvec_reverse(int N, double* V, int incV, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(double),
                               .P         = P,
                               .incP      = incP};
  reverse(N, &vec_swap, &swap_data);
}

void util_svec_reverse(int N, float* V, int incV, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(float),
                               .P         = P,
                               .incP      = incP};
  reverse(N, &vec_swap, &swap_data);
}

void util_zvec_reverse(int N, double complex* V, int incV, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(double complex),
                               .P         = P,
                               .incP      = incP};
  reverse(N, &vec_swap, &swap_data);
}

void util_cvec_reverse(int N, float complex* V, int incV, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(float complex),
                               .P         = P,
                               .incP      = incP};
  reverse(N, &vec_swap, &swap_data);
}

void util_dmat_row_reverse(char Order, char TransA, int M, int N, double *A, int lda, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(double),
                                   .P         = P,
                                   .incP      = incP};
  reverse(mat_row_permute_size(Order, TransA, M, N), &mat_row_swap, &swap_data);
}

void util_smat_row_reverse(char Order, char TransA, int M, int N, float *A, int lda, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(float),
                                   .P         = P,
                                   .incP      = incP};
  reverse(mat_row_permute_size(Order, TransA, M, N), &mat_row_swap, &swap_data);
}

void util_zmat_row_reverse(char Order, char TransA, int M, int N, double complex *A, int lda, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(double complex),
                                   .P         = P,
                                   .incP      = incP};
  reverse(mat_row_permute_size(Order, TransA, M, N), &mat_row_swap, &swap_data);
}

void util_cmat_row_reverse(char Order, char TransA, int M, int N, float complex *A, int lda, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(float complex),
                                   .P         = P,
                                   .incP      = incP};
  reverse(mat_row_permute_size(Order, TransA, M, N), &mat_row_swap, &swap_data);
}

static void shuffle(int N, swap_func swap, void *swap_data) {
  int i;
  int gap;
  for (i = 0; i < N - 2; i++)
  {
    gap = util_rand(N - i);
    swap(i, i + gap, swap_data);
  }
}

void util_dvec_shuffle(int N, double* V, int incV, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(double),
                               .P         = P,
                               .incP      = incP};
  shuffle(N, &vec_swap, &swap_data);
}

void util_svec_shuffle(int N, float* V, int incV, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(float),
                               .P         = P,
                               .incP      = incP};
  shuffle(N, &vec_swap, &swap_data);
}

void util_zvec_shuffle(int N, double complex* V, int incV, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(double complex),
                               .P         = P,
                               .incP      = incP};
  shuffle(N, &vec_swap, &swap_data);
}

void util_cvec_shuffle(int N, float complex* V, int incV, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(float complex),
                               .P         = P,
                               .incP      = incP};
  shuffle(N, &vec_swap, &swap_data);
}

void util_dmat_row_shuffle(char Order, char TransA, int M, int N, double *A, int lda, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(double),
                                   .P         = P,
                                   .incP      = incP};
  shuffle(mat_row_permute_size(Order, TransA, M, N), &mat_row_swap, &swap_data);
}

void util_smat_row_shuffle(char Order, char TransA, int M, int N, float *A, int lda, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(float),
                                   .P         = P,
                                   .incP      = incP};
  shuffle(mat_row_permute_size(Order, TransA, M, N), &mat_row_swap, &swap_data);
}

void util_zmat_row_shuffle(char Order, char TransA, int M, int N, double complex *A, int lda, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(double complex),
                                   .P         = P,
                                   .incP      = incP};
  shuffle(mat_row_permute_size(Order, TransA, M, N), &mat_row_swap, &swap_data);
}

void util_cmat_row_shuffle(char Order, char TransA, int M, int N, float complex *A, int lda, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(float complex),
                                   .P         = P,
                                   .incP      = incP};
  shuffle(mat_row_permute_size(Order, TransA, M, N), &mat_row_swap, &swap_data);
}

int* util_identity_permutation(int N){
  int i;
  int *identity = (int*)malloc(N * sizeof(int));
  for (i = 0; i < N; i++)
  {
    identity[i] = i;
  }
  return identity;
}

int* util_inverse_permutation(int N, int *P, int incP){
  int i;
  int *P_inverse = (int*)malloc(N * sizeof(int));
  for (i = 0; i < N; i++)
  {
    P_inverse[P[i * incP]] = i;
  }
  return P_inverse;
}

static void permute(int N, int *P, int incP, swap_func swap, void *swap_data) {
  int i, j, k;
  int *Q = util_inverse_permutation(N, P, incP);
  for (i = 0; i < N; i++)
  {
    if(Q[i] != -1){
      j = Q[i];
      while(j != i){
        swap(i, j, swap_data);
        k = j;
        j = Q[j];
        Q[k] = -1;
      }
      Q[i] = -1;
    }
  }
  free(Q);
}

void util_dvec_permute(int N, double* V, int incV, int *Q, int incQ, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(double),
                               .P         = P,
                               .incP      = incP};
  permute(N, Q, incQ, &vec_swap, &swap_data);
}

void util_svec_permute(int N, float* V, int incV, int *Q, int incQ, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(float),
                               .P         = P,
                               .incP      = incP};
  permute(N, Q, incQ, &vec_swap, &swap_data);
}

void util_zvec_permute(int N, double complex* V, int incV, int *Q, int incQ, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(double complex),
                               .P         = P,
                               .incP      = incP};
  permute(N, Q, incQ, &vec_swap, &swap_data);
}

void util_cvec_permute(int N, float complex* V, int incV, int *Q, int incQ, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = sizeof(float complex),
                               .P         = P,
                               .incP      = incP};
  permute(N, Q, incQ, &vec_swap, &swap_data);
}

void util_divec_permute(int num, int N, double* V, int incV, int *Q, int incQ, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = num * sizeof(double),
                               .P         = P,
                               .incP      = incP};
  permute(N, Q, incQ, &vec_swap, &swap_data);
}

void util_sivec_permute(int num, int N, float* V, int incV, int *Q, int incQ, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = num * sizeof(float),
                               .P         = P,
                               .incP      = incP};
  permute(N, Q, incQ, &vec_swap, &swap_data);
}

void util_zivec_permute(int num, int N, double complex* V, int incV, int *Q, int incQ, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = num * sizeof(double complex),
                               .P         = P,
                               .incP      = incP};
  permute(N, Q, incQ, &vec_swap, &swap_data);
}

void util_civec_permute(int num, int N, float complex* V, int incV, int *Q, int incQ, int *P, int incP) {
  vec_swap_data_t swap_data = {.V         = V,
                               .incV      = incV,
                               .elem_size = num * sizeof(float complex),
                               .P         = P,
                               .incP      = incP};
  permute(N, Q, incQ, &vec_swap, &swap_data);
}

void util_dmat_row_permute(char Order, char TransA, int M, int N, double *A, int lda, int *Q, int incQ, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(double),
                                   .P         = P,
                                   .incP      = incP};
  permute(mat_row_permute_size(Order, TransA, M, N), Q, incQ, &mat_row_swap, &swap_data);
}

void util_smat_row_permute(char Order, char TransA, int M, int N, float *A, int lda, int *Q, int incQ, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(float),
                                   .P         = P,
                                   .incP      = incP};
  permute(mat_row_permute_size(Order, TransA, M, N), Q, incQ, &mat_row_swap, &swap_data);
}

void util_zmat_row_permute(char Order, char TransA, int M, int N, double complex *A, int lda, int *Q, int incQ, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(double complex),
                                   .P         = P,
                                   .incP      = incP};
  permute(mat_row_permute_size(Order, TransA, M, N), Q, incQ, &mat_row_swap, &swap_data);
}

void util_cmat_row_permute(char Order, char TransA, int M, int N, float complex *A, int lda, int *Q, int incQ, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = sizeof(float complex),
                                   .P         = P,
                                   .incP      = incP};
  permute(mat_row_permute_size(Order, TransA, M, N), Q, incQ, &mat_row_swap, &swap_data);
}

void util_dimat_row_permute(int num, char Order, char TransA, int M, int N, double *A, int lda, int *Q, int incQ, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = num * sizeof(double),
                                   .P         = P,
                                   .incP      = incP};
  permute(mat_row_permute_size(Order, TransA, M, N), Q, incQ, &mat_row_swap, &swap_data);
}

void util_simat_row_permute(int num, char Order, char TransA, int M, int N, float *A, int lda, int *Q, int incQ, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = num * sizeof(float),
                                   .P         = P,
                                   .incP      = incP};
  permute(mat_row_permute_size(Order, TransA, M, N), Q, incQ, &mat_row_swap, &swap_data);
}

void util_zimat_row_permute(int num, char Order, char TransA, int M, int N, double complex *A, int lda, int *Q, int incQ, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = num * sizeof(complex double),
                                   .P         = P,
                                   .incP      = incP};
  permute(mat_row_permute_size(Order, TransA, M, N), Q, incQ, &mat_row_swap, &swap_data);
}

void util_cimat_row_permute(int num, char Order, char TransA, int M, int N, float complex *A, int lda, int *Q, int incQ, int *P, int incP){
  mat_row_swap_data_t swap_data = {.Order     = Order,
                                   .TransA    = TransA,
                                   .M         = M,
                                   .N         = N,
                                   .A         = A,
                                   .lda       = lda,
                                   .elem_size = num * sizeof(complex float),
                                   .P         = P,
                                   .incP      = incP};
  permute(mat_row_permute_size(Order, TransA, M, N), Q, incQ, &mat_row_swap, &swap_data);
}

double* util_dvec_alloc(int N, int incV) {
  double *V = (double*)malloc(N * incV * sizeof(double));
  if(incV != 1){
    //fill empty space with NaN to check increments
    util_dvec_fill(N * incV, V, 1, util_Vec_Rand, NAN, 0.0);
    util_dvec_fill(N, V, incV, util_Vec_Constant, 0.0, 0.0);
  }
  return V;
}

float* util_svec_alloc(int N, int incV) {
  float *V = (float*)malloc(N * incV * sizeof(float));
  if(incV != 1){
    //fill empty space with NaN to check increments
    util_svec_fill(N * incV, V, 1, util_Vec_Rand, NAN, 0.0);
    util_svec_fill(N, V, incV, util_Vec_Constant, 0.0, 0.0);
  }
  return V;
}

double complex* util_zvec_alloc(int N, int incV) {
  double complex *V = (double complex*)malloc(N * incV * sizeof(double complex));
  if(incV != 1){
    //fill empty space with NaN to check increments
    util_zvec_fill(N * incV, V, 1, util_Vec_Rand, NAN, 0.0);
    util_zvec_fill(N, V, incV, util_Vec_Constant, 0.0, 0.0);
  }
  return V;
}

float complex* util_cvec_alloc(int N, int incV) {
  float complex *V = (float complex*)malloc(N * incV * sizeof(float complex));
  if(incV != 1){
    //fill empty space with NaN to check increments
    util_cvec_fill(N * incV, V, 1, util_Vec_Rand, NAN, 0.0);
    util_cvec_fill(N, V, incV, util_Vec_Constant, 0.0, 0.0);
  }
  return V;
}

double* util_dmat_alloc(char Order, int M, int N, int lda) {
  double *A;
  switch(Order){
    case 'r':
    case 'R':
      A = (double*)malloc(lda * M * sizeof(double));
      //fill empty space with NaN to check lda
      util_dmat_fill(Order, 'n', M, lda, A, lda, util_Mat_Row_Rand, NAN, 0.0);
      util_dmat_fill(Order, 'n', M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      break;
    default:
      A = (double*)malloc(lda * N * sizeof(double));
      //fill empty space with NaN to check lda
      util_dmat_fill(Order, 'n', lda, N, A, lda, util_Mat_Row_Rand, NAN, 0.0);
      util_dmat_fill(Order, 'n', M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      break;
  }
  return A;
}

float* util_smat_alloc(char Order, int M, int N, int lda) {
  float *A;
  switch(Order){
    case 'r':
    case 'R':
      A = (float*)malloc(lda * M * sizeof(float));
      //fill empty space with NaN to check lda
      util_smat_fill(Order, 'n', M, lda, A, lda, util_Mat_Row_Rand, NAN, 0.0);
      util_smat_fill(Order, 'n', M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      break;
    default:
      A = (float*)malloc(lda * N * sizeof(float));
      //fill empty space with NaN to check lda
      util_smat_fill(Order, 'n', lda, N, A, lda, util_Mat_Row_Rand, NAN, 0.0);
      util_smat_fill(Order, 'n', M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      break;
  }
  return A;
}

double complex* util_zmat_alloc(char Order, int M, int N, int lda) {
  double complex *A;
  switch(Order){
    case 'r':
    case 'R':
      A = (double complex*)malloc(lda * M * sizeof(double complex));
      //fill empty space with NaN to check lda
      util_zmat_fill(Order, 'n', M, lda, A, lda, util_Mat_Row_Rand, NAN, 0.0);
      util_zmat_fill(Order, 'n', M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      break;
    default:
      A = (double complex*)malloc(lda * N * sizeof(double complex));
      //fill empty space with NaN to check lda
      util_zmat_fill(Order, 'n', lda, N, A, lda, util_Mat_Row_Rand, NAN, 0.0);
      util_zmat_fill(Order, 'n', M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      break;
  }
  return A;
}

float complex* util_cmat_alloc(char Order, int M, int N, int lda) {
  float complex *A;
  switch(Order){
    case 'r':
    case 'R':
      A = (float complex*)malloc(lda * M * sizeof(float complex));
      //fill empty space with NaN to check lda
      util_cmat_fill(Order, 'n', M, lda, A, lda, util_Mat_Row_Rand, NAN, 0.0);
      util_cmat_fill(Order, 'n', M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      break;
    default:
      A = (float complex*)malloc(lda * N * sizeof(float complex));
      //fill empty space with NaN data to check lda
      util_cmat_fill(Order, 'n', lda, N, A, lda, util_Mat_Row_Rand, NAN, 0.0);
      util_cmat_fill(Order, 'n', M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      break;
  }
  return A;
}

void util_dvec_fill(int N, double* V, int incV, util_vec_fill_t Fill, double RealScale, double ImagScale) {
  int i;
  double small = 1.0 / (1024.0 * 1024.0 * 128.0); // 2^-27
  double big   = 1024.0 * 1024.0 * 128.0;         // 2^27
  (void)ImagScale;
  switch(Fill){
    case util_Vec_Constant_Drop:
    case util_Vec_Constant:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      break;
    case util_Vec_Pos_Inf:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      break;
    case util_Vec_Pos_Pos_Inf:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = INFINITY;
      break;
    case util_Vec_Pos_Neg_Inf:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = -1 * INFINITY;
      break;
    case util_Vec_NaN:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = NAN;
      break;
    case util_Vec_Pos_Inf_NaN:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = NAN;
      break;
    case util_Vec_Pos_Pos_Inf_NaN:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = INFINITY;
      V[(N/2) * incV] = NAN;
      break;
    case util_Vec_Pos_Neg_Inf_NaN:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = -1.0 * INFINITY;
      V[(N/2) * incV] = NAN;
      break;
    case util_Vec_Pos_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small;
      }
      V[0] = big;
      break;
    case util_Vec_Pos_Pos_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small;
      }
      V[0] = big;
      V[(N - 1) * incV] = big;
      break;
    case util_Vec_Pos_Neg_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small;
      }
      V[0] = big;
      V[(N - 1) * incV] = -1 * big;
      break;
    case util_Vec_Rand_Drop:
    case util_Vec_Rand:
      for (i = 0; i < N; i++) {
        V[i*incV] = util_drand() * (1+1e-9);
      }
      break;
    case util_Vec_2_Times_Rand_Minus_1_Drop:
    case util_Vec_2_Times_Rand_Minus_1:
      for (i = 0; i < N; i++) {
        V[i*incV] = (2 * util_drand() * (1+1e-9) - 1);
      }
      break;
    case util_Vec_Rand_Plus_Rand_Minus_1_Drop:
    case util_Vec_Rand_Plus_Rand_Minus_1:
      for (i = 0; i < N; i++) {
        V[i*incV] = util_drand() * (1+1e-9) + (util_drand() * (1+1e-9) - 1);
      }
      break;
    case util_Vec_Normal_Drop:
    case util_Vec_Normal:
      for (i = 0; i < N; i++) {
        double t1 = util_drand();
        double t2 = util_drand();
        V[i * incV] = sqrt(-2.0 * log(t1)) * cos(2.0 * M_PI * t2);
      }
      break;
    case util_Vec_Sine_Drop:
    case util_Vec_Sine:
      for (i = 0; i < (N + 1)/2; i++) {
        V[i*incV] = sin(2.0 * M_PI * ((double)i / (double)N));
      }
      V[0] = 0.0;
      for (; i < N; i++) {
        V[i*incV] = -1 * V[(i - N/2)*incV];
      }
      break;
    case util_Vec_Rand_Cond:
      {
        double cond = RealScale;
        int quart = (N / 8) & ~1;
        int mid = N - quart * 2;
        double c1, c2, c, f;
        int i;

        util_dvec_fill(quart / 2, V, incV, util_Vec_Rand, 1.0e-10, 0.0);
        util_dvec_fill(quart / 2, V + (quart / 2)*incV, incV, util_Vec_Rand, 1, 0.0);
        util_dvec_fill(quart / 8, V, incV, util_Vec_Rand, 1e-20, 0.0);
        c1 = 0.0;
        for (i = 0; i < quart; i++) {
          c1 += V[i*incV];
          V[(i + quart)*incV] = -V[i*incV];
        }
        util_dvec_fill(quart, V + 2 * quart*incV, incV, util_Vec_Rand, 1.0, 0.0);
        util_dvec_fill(mid - quart, V + 3 * quart *incV, incV, util_Vec_Rand, 1e-8, 0.0);
        c2 = 0.0; c = 0.0;
        for (i = 2 * quart; i < N; i++) {
          c2 += fabs(V[i*incV]);
          c  += V[i*incV];
        }

        f = 2 * c1 / (cond * fabs(c) - c2);
        if (f < 1e-16) f = 1e-16;
        if (f > 1e16) f = 1e16;
        for (i = 2 * quart; i < N; i++) {
          V[i*incV] *= f;
        }

        util_dvec_shuffle(N, V, incV, NULL, 1);
      }
      return; //don't scale the conditioned numbers
    case util_Vec_N_Cond:
      {
        V[0] = -1.0 * N;
        for(i = 1; i < N; i++){
          V[i * incV] = 1.0;
        }
      }
      break;
    case util_Vec_N_Cond_Plus_Rand:
      {
        V[0] = -1.0 * N;
        for(i = 1; i < N - 1; i++){
          V[i * incV] = 1.0;
        }
        V[(N - 1) * incV] = util_drand();
      }
      break;
    case util_Vec_Full_Range:
      for (i = 0; i < N; i++) {
        V[i*incV] = (1 - (i % 2) * 2) * MAX(MIN(exp(log(DBL_MIN) + (log(DBL_MAX) - log(DBL_MIN)) * ((double)i / (double)N)), DBL_MAX), -1 * DBL_MAX);
      }
      break;
    case util_Vec_Half_Range:
      for (i = 0; i < N; i++) {
        V[i*incV] = (1 - (i % 2) * 2) * MAX(MIN(exp(log(DBL_MAX) * ((double)i / (double)N)), DBL_MAX), -1 * DBL_MAX);
      }
      break;
    case util_Vec_Small_Plus_Increasing_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small + (big - small) * ((double)i / (double)N);
      }
      break;
    case util_Vec_Small_Plus_Rand_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small + (big - small) * util_drand() * (1+1e-9);
      }
      break;
    case util_Vec_Mountain:
      break;
  }

  for (i = 0; i < N; i++) {
    V[i*incV] *= RealScale;
  }

  switch(Fill){
    case util_Vec_Constant_Drop:
    case util_Vec_Rand_Drop:
    case util_Vec_2_Times_Rand_Minus_1_Drop:
    case util_Vec_Rand_Plus_Rand_Minus_1_Drop:
    case util_Vec_Normal_Drop:
    case util_Vec_Sine_Drop:
      for (i = N/2; i < N; i++) {
        V[i*incV] *= 1e-12;
      }
      break;
    case util_Vec_Mountain:
      {
        V[(N + 1)/2] = 0.0;
        double s = util_drand() * RealScale;
        for (i = 0; i < N/2; i++) {
          V[i*incV] = s;
          V[(N - i - 1)*incV] = -s;
          s = s * ldexp(0.5, 1 - (DBL_MANT_DIG/2));
        }
      }
      break;
    default:
      break;
  }
}


void util_svec_fill(int N, float* V, int incV, util_vec_fill_t Fill, float RealScale, float ImagScale) {
  int i;
  double small = 1.0 / (1024.0 * 4.0); // 2^-12
  double big   = 1024.0 * 8.0;         // 2^13
  (void)ImagScale;
  switch(Fill){
    case util_Vec_Constant_Drop:
    case util_Vec_Constant:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      break;
    case util_Vec_Pos_Inf:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      break;
    case util_Vec_Pos_Pos_Inf:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = INFINITY;
      break;
    case util_Vec_Pos_Neg_Inf:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = -1.0 * INFINITY;
      break;
    case util_Vec_NaN:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = NAN;
      break;
    case util_Vec_Pos_Inf_NaN:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = NAN;
      break;
    case util_Vec_Pos_Pos_Inf_NaN:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = INFINITY;
      V[(N/2) * incV] = NAN;
      break;
    case util_Vec_Pos_Neg_Inf_NaN:
      for (i = 0; i < N; i++) {
        V[i*incV] = 1.0;
      }
      V[0] = INFINITY;
      V[(N - 1) * incV] = -1.0 * INFINITY;
      V[(N/2) * incV] = NAN;
      break;
    case util_Vec_Pos_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small;
      }
      V[0] = big;
      break;
    case util_Vec_Pos_Pos_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small;
      }
      V[0] = big;
      V[(N - 1) * incV] = big;
      break;
    case util_Vec_Pos_Neg_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small;
      }
      V[0] = big;
      V[(N - 1) * incV] = -1 * big;
      break;
    case util_Vec_Rand_Drop:
    case util_Vec_Rand:
      for (i = 0; i < N; i++) {
        V[i*incV] = (float)util_drand() * (1+1e-4);
      }
      break;
    case util_Vec_2_Times_Rand_Minus_1_Drop:
    case util_Vec_2_Times_Rand_Minus_1:
      for (i = 0; i < N; i++) {
        V[i*incV] = (2 * (float)util_drand() * (1+1e-4) - 1);
      }
      break;
    case util_Vec_Rand_Plus_Rand_Minus_1_Drop:
    case util_Vec_Rand_Plus_Rand_Minus_1:
      for (i = 0; i < N; i++) {
        V[i*incV] = (float)util_drand() * (1+1e-4) + ((float)util_drand() * (1+1e-4) - 1);
      }
      break;
    case util_Vec_Normal_Drop:
    case util_Vec_Normal:
      for (i = 0; i < N; i++) {
        double t1 = util_drand();
        double t2 = util_drand();
        V[i * incV] = (float)(sqrt(-2.0 * log(t1)) * cos(2.0 * M_PI * t2));
      }
      break;
    case util_Vec_Sine_Drop:
    case util_Vec_Sine:
      for (i = 0; i < (N + 1)/2; i++) {
        V[i*incV] = (float)sin(2.0 * M_PI * ((double)i / (double)N));
      }
      V[0] = 0.0;
      for (; i < N; i++) {
        V[i*incV] = -1 * V[(i - N/2)*incV];
      }
      break;
    case util_Vec_Rand_Cond:
      {
        double cond = RealScale;
        int quart = (N / 8) & ~1;
        int mid = N - quart * 2;
        double c1, c2, c, f;
        int i;

        util_svec_fill(quart / 2, V, incV, util_Vec_Rand, 1.0e-10, 0.0);
        util_svec_fill(quart / 2, V + (quart / 2) * incV, incV, util_Vec_Rand, 1.0, 0.0);
        util_svec_fill(quart / 8, V, incV, util_Vec_Rand, 1e-20, 0.0);
        c1 = 0.0;
        for (i = 0; i < quart; i++) {
          c1 += V[i*incV];
          V[(i + quart)*incV] = -V[i*incV];
        }
        util_svec_fill(quart, V + 2 * quart * incV, incV, util_Vec_Rand, 1.0, 0.0);
        util_svec_fill(mid - quart, V + 3 * quart * incV, incV, util_Vec_Rand, 1e-8, 0.0);
        c2 = 0.0; c = 0.0;
        for (i = 2 * quart; i < N; i++) {
          c2 += fabs(V[i*incV]);
          c  += V[i*incV];
        }

        f = 2 * c1 / (cond * fabs(c) - c2);
        if (f < 1e-16) f = 1e-16;
        if (f > 1e16) f = 1e16;
        for (i = 2 * quart; i < N; i++) {
          V[i*incV] *= f;
        }

        util_svec_shuffle(N, V, incV, NULL, 1);
      }
      return; //don't scale the conditioned numbers
    case util_Vec_N_Cond:
      {
        V[0] = -1.0 * N;
        for(i = 1; i < N; i++){
          V[i * incV] = 1.0;
        }
      }
      break;
    case util_Vec_N_Cond_Plus_Rand:
      {
        V[0] = -1.0 * N;
        for(i = 1; i < N - 1; i++){
          V[i * incV] = 1.0;
        }
        V[(N - 1) * incV] = util_drand();
      }
      break;
    case util_Vec_Full_Range:
      for (i = 0; i < N; i++) {
        V[i*incV] = (1 - (i % 2) * 2) * MAX(MIN(exp(log(FLT_MIN) + (log(FLT_MAX) - log(FLT_MIN)) * ((double)i / (double)N)), FLT_MAX), -1 * FLT_MAX);
      }
      break;
    case util_Vec_Half_Range:
      for (i = 0; i < N; i++) {
        V[i*incV] = (1 - (i % 2) * 2) * MAX(MIN(exp(log(FLT_MAX) * ((double)i / (double)N)), FLT_MAX), -1 * FLT_MAX);
      }
      break;
    case util_Vec_Small_Plus_Increasing_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small + (big - small) * ((double)i / (double)N);
      }
      break;
    case util_Vec_Small_Plus_Rand_Big:
      for (i = 0; i < N; i++) {
        V[i*incV] = small + (big - small) * (float)util_drand() * (1+1e-4);
      }
      break;
    case util_Vec_Mountain:
      break;
  }

  for (i = 0; i < N; i++) {
    V[i*incV] *= RealScale;
  }

  switch(Fill){
    case util_Vec_Constant_Drop:
    case util_Vec_Rand_Drop:
    case util_Vec_2_Times_Rand_Minus_1_Drop:
    case util_Vec_Rand_Plus_Rand_Minus_1_Drop:
    case util_Vec_Normal_Drop:
    case util_Vec_Sine_Drop:
      for (i = N/2; i < N; i++) {
        V[i*incV] *= 1e-12;
      }
      break;
    case util_Vec_Mountain:
      {
        V[(N + 1)/2] = 0.0;
        double s = util_drand() * RealScale;
        for (i = 0; i < N/2; i++) {
          V[i*incV] = s;
          V[(N - i - 1)*incV] = -s;
          s = s * ldexpf(0.5, 1 - (FLT_MANT_DIG/2));
        }
      }
      break;
    default:
      break;
  }
}

void util_zvec_fill(int N, double complex* V, int incV, util_vec_fill_t Fill, double RealScale, double ImagScale) {
  int i;
  if(Fill == util_Vec_Rand_Cond){
    util_dvec_fill(N, (double*)V, incV * 2, Fill, RealScale, 1.0);
    util_dvec_fill(N, (double*)V + 1, incV * 2, Fill, ImagScale, 1.0);
    return;
  }
  util_dvec_fill(N, (double*)V, incV * 2, Fill, 1.0, 0.0);
  switch(Fill){
    case util_Vec_Rand_Drop:
    case util_Vec_Rand:
    case util_Vec_N_Cond:
    case util_Vec_N_Cond_Plus_Rand:
    case util_Vec_Full_Range:
    case util_Vec_Half_Range:
    case util_Vec_Mountain:
    case util_Vec_2_Times_Rand_Minus_1_Drop:
    case util_Vec_2_Times_Rand_Minus_1:
    case util_Vec_Rand_Plus_Rand_Minus_1_Drop:
    case util_Vec_Rand_Plus_Rand_Minus_1:
    case util_Vec_Small_Plus_Rand_Big:
      util_dvec_fill(N, (double*)V + 1, incV * 2, Fill, 1.0, 0.0);
      break;
    case util_Vec_Constant_Drop:
    case util_Vec_Constant:
    case util_Vec_Pos_Inf:
    case util_Vec_Pos_Pos_Inf:
    case util_Vec_Pos_Neg_Inf:
    case util_Vec_NaN:
    case util_Vec_Pos_Inf_NaN:
    case util_Vec_Pos_Pos_Inf_NaN:
    case util_Vec_Pos_Neg_Inf_NaN:
    case util_Vec_Pos_Big:
    case util_Vec_Pos_Pos_Big:
    case util_Vec_Pos_Neg_Big:
    case util_Vec_Normal_Drop:
    case util_Vec_Normal:
    case util_Vec_Sine_Drop:
    case util_Vec_Sine:
    case util_Vec_Rand_Cond:
    case util_Vec_Small_Plus_Increasing_Big:
      util_dvec_fill(N, (double*)V + 1, incV * 2, util_Vec_Constant, 0.0, 1.0);
      break;
  }
  double complex Scale = RealScale + ImagScale * I;
  double tmp;
  if(RealScale == 0.0){
    if(ImagScale == 0.0){
      for (i = 0; i < N; i++) {
        V[i * incV] = 0.0;
      }
    }else{
      for (i = 0; i < N; i++) {
        tmp = ((double*)V)[i * 2 * incV];
        ((double*)V)[i * 2 * incV] = ((double*)V)[i * 2 * incV + 1] * -1 * ImagScale;
        ((double*)V)[i * 2 * incV + 1] = tmp * ImagScale;
      }
    }
  }else if(ImagScale == 0.0){
    for (i = 0; i < N; i++) {
      ((double*)V)[i * 2 * incV] *= RealScale;
      ((double*)V)[i * 2 * incV + 1] *= RealScale;
    }
  }else{
    for (i = 0; i < N; i++) {
      V[i*incV] *= Scale;
    }
  }
}

void util_cvec_fill(int N, float complex* V, int incV, util_vec_fill_t Fill, float RealScale, float ImagScale) {
  int i;
  if(Fill == util_Vec_Rand_Cond){
    util_svec_fill(N, (float*)V, incV * 2, Fill, RealScale, 1.0);
    util_svec_fill(N, (float*)V + 1, incV * 2, Fill, ImagScale, 1.0);
    return;
  }
  util_svec_fill(N, (float*)V, incV * 2, Fill, 1.0, 1.0);
  switch(Fill){
    case util_Vec_Rand_Drop:
    case util_Vec_Rand:
    case util_Vec_N_Cond:
    case util_Vec_N_Cond_Plus_Rand:
    case util_Vec_Full_Range:
    case util_Vec_Half_Range:
    case util_Vec_Mountain:
    case util_Vec_2_Times_Rand_Minus_1_Drop:
    case util_Vec_2_Times_Rand_Minus_1:
    case util_Vec_Rand_Plus_Rand_Minus_1_Drop:
    case util_Vec_Rand_Plus_Rand_Minus_1:
    case util_Vec_Small_Plus_Rand_Big:
      util_svec_fill(N, (float*)V + 1, incV * 2, Fill, 1.0, 1.0);
      break;
    case util_Vec_Constant_Drop:
    case util_Vec_Constant:
    case util_Vec_Pos_Inf:
    case util_Vec_Pos_Pos_Inf:
    case util_Vec_Pos_Neg_Inf:
    case util_Vec_NaN:
    case util_Vec_Pos_Inf_NaN:
    case util_Vec_Pos_Pos_Inf_NaN:
    case util_Vec_Pos_Neg_Inf_NaN:
    case util_Vec_Pos_Big:
    case util_Vec_Pos_Pos_Big:
    case util_Vec_Pos_Neg_Big:
    case util_Vec_Normal_Drop:
    case util_Vec_Normal:
    case util_Vec_Sine_Drop:
    case util_Vec_Sine:
    case util_Vec_Rand_Cond:
    case util_Vec_Small_Plus_Increasing_Big:
      util_svec_fill(N, (float*)V + 1, incV * 2, util_Vec_Constant, 0.0, 0.0);
      break;
  }
  float complex Scale = RealScale + ImagScale * I;
  float tmp;
  if(RealScale == 0.0){
    if(ImagScale == 0.0){
      for (i = 0; i < N; i++) {
        V[i * incV] = 0.0;
      }
    }else{
      for (i = 0; i < N; i++) {
        tmp = ((float*)V)[i * 2 * incV];
        ((float*)V)[i * 2 * incV] = ((float*)V)[i * 2 * incV + 1] * -1 * ImagScale;
        ((float*)V)[i * 2 * incV + 1] = tmp * ImagScale;
      }
    }
  }else if(ImagScale == 0.0){
    for (i = 0; i < N; i++) {
      ((float*)V)[i * 2 * incV] *= RealScale;
      ((float*)V)[i * 2 * incV + 1] *= RealScale;
    }
  }else{
    for (i = 0; i < N; i++) {
      V[i*incV] *= Scale;
    }
  }
}

void util_dmat_fill(char Order, char TransA, int M, int N, double* A, int lda, util_mat_fill_t Fill, double RealScale, double ImagScale) {
  int i;
  util_vec_fill_t row_fill;
  switch(Fill){
    case util_Mat_Identity:
      util_dmat_fill(Order, TransA, M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      for(i = 0; i < M && i < N; i++){
        A[i * lda + i] = 1.0;
      }
      return;
    case util_Mat_Row_Constant_Drop:
      row_fill = util_Vec_Constant_Drop;
      break;
    case util_Mat_Row_Constant:
      row_fill = util_Vec_Constant;
      break;
    case util_Mat_Row_Pos_Inf:
      row_fill = util_Vec_Pos_Inf;
      break;
    case util_Mat_Row_Pos_Pos_Inf:
      row_fill = util_Vec_Pos_Pos_Inf;
      break;
    case util_Mat_Row_Pos_Neg_Inf:
      row_fill = util_Vec_Pos_Neg_Inf;
      break;
    case util_Mat_Row_NaN:
      row_fill = util_Vec_NaN;
      break;
    case util_Mat_Row_Pos_Inf_NaN:
      row_fill = util_Vec_Pos_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Pos_Inf_NaN:
      row_fill = util_Vec_Pos_Pos_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Neg_Inf_NaN:
      row_fill = util_Vec_Pos_Neg_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Big:
      row_fill = util_Vec_Pos_Big;
      break;
    case util_Mat_Row_Pos_Pos_Big:
      row_fill = util_Vec_Pos_Pos_Big;
      break;
    case util_Mat_Row_Pos_Neg_Big:
      row_fill = util_Vec_Pos_Neg_Big;
      break;
    case util_Mat_Row_Rand_Drop:
      row_fill = util_Vec_Rand_Drop;
      break;
    case util_Mat_Row_Rand:
      row_fill = util_Vec_Rand;
      break;
    case util_Mat_Row_2_Times_Rand_Minus_1_Drop:
      row_fill = util_Vec_2_Times_Rand_Minus_1_Drop;
      break;
    case util_Mat_Row_2_Times_Rand_Minus_1:
      row_fill = util_Vec_2_Times_Rand_Minus_1;
      break;
    case util_Mat_Row_Rand_Plus_Rand_Minus_1_Drop:
      row_fill = util_Vec_Rand_Plus_Rand_Minus_1_Drop;
      break;
    case util_Mat_Row_Rand_Plus_Rand_Minus_1:
      row_fill = util_Vec_Rand_Plus_Rand_Minus_1;
      break;
    case util_Mat_Row_Normal_Drop:
      row_fill = util_Vec_Normal_Drop;
      break;
    case util_Mat_Row_Normal:
      row_fill = util_Vec_Normal;
      break;
    case util_Mat_Row_Sine_Drop:
      row_fill = util_Vec_Sine_Drop;
      break;
    case util_Mat_Row_Sine:
      row_fill = util_Vec_Sine;
      break;
    case util_Mat_Row_Rand_Cond:
      row_fill = util_Vec_Rand_Cond;
      break;
    case util_Mat_Row_N_Cond:
      row_fill = util_Vec_N_Cond;
      break;
    case util_Mat_Row_N_Cond_Plus_Rand:
      row_fill = util_Vec_N_Cond_Plus_Rand;
      break;
    case util_Mat_Row_Full_Range:
      row_fill = util_Vec_Full_Range;
      break;
    case util_Mat_Row_Half_Range:
      row_fill = util_Vec_Half_Range;
      break;
    case util_Mat_Row_Mountain:
      row_fill = util_Vec_Mountain;
      break;
    case util_Mat_Row_Small_Plus_Increasing_Big:
      row_fill = util_Vec_Small_Plus_Increasing_Big;
      break;
    case util_Mat_Row_Small_Plus_Rand_Big:
      row_fill = util_Vec_Small_Plus_Rand_Big;
      break;
    default:
      exit(125);//TODO better error here
  }
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < M; i++){
            util_dvec_fill(N, A + i * lda, 1, row_fill, RealScale, ImagScale);
          }
          break;
        default:
          for(i = 0; i < N; i++){
            util_dvec_fill(M, A + i, lda, row_fill, RealScale, ImagScale);
          }
          break;
      }
      break;
    default:
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < M; i++){
            util_dvec_fill(N, A + i, lda, row_fill, RealScale, ImagScale);
          }
          break;
        default:
          for(i = 0; i < N; i++){
            util_dvec_fill(M, A + i * lda, 1, row_fill, RealScale, ImagScale);
          }
          break;
      }
      break;
  }
}

void util_smat_fill(char Order, char TransA, int M, int N, float* A, int lda, util_mat_fill_t Fill, float RealScale, float ImagScale) {
  int i;
  util_vec_fill_t row_fill;
  switch(Fill){
    case util_Mat_Identity:
      util_smat_fill(Order, TransA, M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      for(i = 0; i < M && i < N; i++){
        A[i * lda + i] = 1.0;
      }
      return;
    case util_Mat_Row_Constant_Drop:
      row_fill = util_Vec_Constant_Drop;
      break;
    case util_Mat_Row_Constant:
      row_fill = util_Vec_Constant;
      break;
    case util_Mat_Row_Pos_Inf:
      row_fill = util_Vec_Pos_Inf;
      break;
    case util_Mat_Row_Pos_Pos_Inf:
      row_fill = util_Vec_Pos_Pos_Inf;
      break;
    case util_Mat_Row_Pos_Neg_Inf:
      row_fill = util_Vec_Pos_Neg_Inf;
      break;
    case util_Mat_Row_NaN:
      row_fill = util_Vec_NaN;
      break;
    case util_Mat_Row_Pos_Inf_NaN:
      row_fill = util_Vec_Pos_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Pos_Inf_NaN:
      row_fill = util_Vec_Pos_Pos_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Neg_Inf_NaN:
      row_fill = util_Vec_Pos_Neg_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Big:
      row_fill = util_Vec_Pos_Big;
      break;
    case util_Mat_Row_Pos_Pos_Big:
      row_fill = util_Vec_Pos_Pos_Big;
      break;
    case util_Mat_Row_Pos_Neg_Big:
      row_fill = util_Vec_Pos_Neg_Big;
      break;
    case util_Mat_Row_Rand_Drop:
      row_fill = util_Vec_Rand_Drop;
      break;
    case util_Mat_Row_Rand:
      row_fill = util_Vec_Rand;
      break;
    case util_Mat_Row_2_Times_Rand_Minus_1_Drop:
      row_fill = util_Vec_2_Times_Rand_Minus_1_Drop;
      break;
    case util_Mat_Row_2_Times_Rand_Minus_1:
      row_fill = util_Vec_2_Times_Rand_Minus_1;
      break;
    case util_Mat_Row_Rand_Plus_Rand_Minus_1_Drop:
      row_fill = util_Vec_Rand_Plus_Rand_Minus_1_Drop;
      break;
    case util_Mat_Row_Rand_Plus_Rand_Minus_1:
      row_fill = util_Vec_Rand_Plus_Rand_Minus_1;
      break;
    case util_Mat_Row_Normal_Drop:
      row_fill = util_Vec_Normal_Drop;
      break;
    case util_Mat_Row_Normal:
      row_fill = util_Vec_Normal;
      break;
    case util_Mat_Row_Sine_Drop:
      row_fill = util_Vec_Sine_Drop;
      break;
    case util_Mat_Row_Sine:
      row_fill = util_Vec_Sine;
      break;
    case util_Mat_Row_Rand_Cond:
      row_fill = util_Vec_Rand_Cond;
      break;
    case util_Mat_Row_N_Cond:
      row_fill = util_Vec_N_Cond;
      break;
    case util_Mat_Row_N_Cond_Plus_Rand:
      row_fill = util_Vec_N_Cond_Plus_Rand;
      break;
    case util_Mat_Row_Full_Range:
      row_fill = util_Vec_Full_Range;
      break;
    case util_Mat_Row_Half_Range:
      row_fill = util_Vec_Half_Range;
      break;
    case util_Mat_Row_Mountain:
      row_fill = util_Vec_Mountain;
      break;
    case util_Mat_Row_Small_Plus_Increasing_Big:
      row_fill = util_Vec_Small_Plus_Increasing_Big;
      break;
    case util_Mat_Row_Small_Plus_Rand_Big:
      row_fill = util_Vec_Small_Plus_Rand_Big;
      break;
    default:
      exit(125); //TODO better error here
  }
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < M; i++){
            util_svec_fill(N, A + i * lda, 1, row_fill, RealScale, ImagScale);
          }
          break;
        default:
          for(i = 0; i < N; i++){
            util_svec_fill(M, A + i, lda, row_fill, RealScale, ImagScale);
          }
          break;
      }
      break;
    default:
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < M; i++){
            util_svec_fill(N, A + i, lda, row_fill, RealScale, ImagScale);
          }
          break;
        default:
          for(i = 0; i < N; i++){
            util_svec_fill(M, A + i * lda, 1, row_fill, RealScale, ImagScale);
          }
          break;
      }
    break;
  }
}

void util_zmat_fill(char Order, char TransA, int M, int N, double complex* A, int lda, util_mat_fill_t Fill, double RealScale, double ImagScale) {
  int i;
  int j;
  util_vec_fill_t row_fill;
  switch(Fill){
    case util_Mat_Identity:
      util_zmat_fill(Order, TransA, M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      for(i = 0; i < M && i < N; i++){
        A[i * lda + i] = 1.0;
      }
      return;
    case util_Mat_Row_Constant_Drop:
      row_fill = util_Vec_Constant_Drop;
      break;
    case util_Mat_Row_Constant:
      row_fill = util_Vec_Constant;
      break;
    case util_Mat_Row_Pos_Inf:
      row_fill = util_Vec_Pos_Inf;
      break;
    case util_Mat_Row_Pos_Pos_Inf:
      row_fill = util_Vec_Pos_Pos_Inf;
      break;
    case util_Mat_Row_Pos_Neg_Inf:
      row_fill = util_Vec_Pos_Neg_Inf;
      break;
    case util_Mat_Row_NaN:
      row_fill = util_Vec_NaN;
      break;
    case util_Mat_Row_Pos_Inf_NaN:
      row_fill = util_Vec_Pos_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Pos_Inf_NaN:
      row_fill = util_Vec_Pos_Pos_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Neg_Inf_NaN:
      row_fill = util_Vec_Pos_Neg_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Big:
      row_fill = util_Vec_Pos_Big;
      break;
    case util_Mat_Row_Pos_Pos_Big:
      row_fill = util_Vec_Pos_Pos_Big;
      break;
    case util_Mat_Row_Pos_Neg_Big:
      row_fill = util_Vec_Pos_Neg_Big;
      break;
    case util_Mat_Row_Rand_Drop:
      row_fill = util_Vec_Rand_Drop;
      break;
    case util_Mat_Row_Rand:
      row_fill = util_Vec_Rand;
      break;
    case util_Mat_Row_2_Times_Rand_Minus_1_Drop:
      row_fill = util_Vec_2_Times_Rand_Minus_1_Drop;
      break;
    case util_Mat_Row_2_Times_Rand_Minus_1:
      row_fill = util_Vec_2_Times_Rand_Minus_1;
      break;
    case util_Mat_Row_Rand_Plus_Rand_Minus_1_Drop:
      row_fill = util_Vec_Rand_Plus_Rand_Minus_1_Drop;
      break;
    case util_Mat_Row_Rand_Plus_Rand_Minus_1:
      row_fill = util_Vec_Rand_Plus_Rand_Minus_1;
      break;
    case util_Mat_Row_Normal_Drop:
      row_fill = util_Vec_Normal_Drop;
      break;
    case util_Mat_Row_Normal:
      row_fill = util_Vec_Normal;
      break;
    case util_Mat_Row_Sine_Drop:
      row_fill = util_Vec_Sine_Drop;
      break;
    case util_Mat_Row_Sine:
      row_fill = util_Vec_Sine;
      break;
    case util_Mat_Row_Rand_Cond:
      row_fill = util_Vec_Rand_Cond;
      break;
    case util_Mat_Row_N_Cond:
      row_fill = util_Vec_N_Cond;
      break;
    case util_Mat_Row_N_Cond_Plus_Rand:
      row_fill = util_Vec_N_Cond_Plus_Rand;
      break;
    case util_Mat_Row_Full_Range:
      row_fill = util_Vec_Full_Range;
      break;
    case util_Mat_Row_Half_Range:
      row_fill = util_Vec_Half_Range;
      break;
    case util_Mat_Row_Mountain:
      row_fill = util_Vec_Mountain;
      break;
    case util_Mat_Row_Small_Plus_Increasing_Big:
      row_fill = util_Vec_Small_Plus_Increasing_Big;
      break;
    case util_Mat_Row_Small_Plus_Rand_Big:
      row_fill = util_Vec_Small_Plus_Rand_Big;
      break;
    default:
      exit(125);//TODO better error
  }
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < M; i++){
            util_zvec_fill(N, A + i * lda, 1, row_fill, RealScale, ImagScale);
          }
          break;
        default:
          for(i = 0; i < N; i++){
            util_zvec_fill(M, A + i, lda, row_fill, RealScale, ImagScale);
            if(TransA == 'c' || TransA == 'C'){
              for(j = 0; j < M; j++){
                ((double*)A)[2 * (i + j * lda) + 1] = ((double*)A)[2 * (i + j * lda) + 1] * -1;
              }
            }
          }
          break;
      }
      break;
    default:
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < M; i++){
            util_zvec_fill(N, A + i, lda, row_fill, RealScale, ImagScale);
          }
          break;
        default:
          for(i = 0; i < N; i++){
            util_zvec_fill(M, A + i * lda, 1, row_fill, RealScale, ImagScale);
            if(TransA == 'c' || TransA == 'C'){
              for(j = 0; j < M; j++){
                ((double*)A)[2 * (i * lda + j) + 1] = ((double*)A)[2 * (i * lda + j) + 1] * -1;
              }
            }
          }
          break;
      }
      break;
  }
}

void util_cmat_fill(char Order, char TransA, int M, int N, float complex* A, int lda, util_mat_fill_t Fill, float RealScale, float ImagScale) {
  int i;
  int j;
  util_vec_fill_t row_fill;
  switch(Fill){
    case util_Mat_Identity:
      util_cmat_fill(Order, TransA, M, N, A, lda, util_Mat_Row_Constant, 0.0, 0.0);
      for(i = 0; i < M && i < N; i++){
        A[i * lda + i] = 1.0;
      }
      return;
    case util_Mat_Row_Constant_Drop:
      row_fill = util_Vec_Constant_Drop;
      break;
    case util_Mat_Row_Constant:
      row_fill = util_Vec_Constant;
      break;
    case util_Mat_Row_Pos_Inf:
      row_fill = util_Vec_Pos_Inf;
      break;
    case util_Mat_Row_Pos_Pos_Inf:
      row_fill = util_Vec_Pos_Pos_Inf;
      break;
    case util_Mat_Row_Pos_Neg_Inf:
      row_fill = util_Vec_Pos_Neg_Inf;
      break;
    case util_Mat_Row_NaN:
      row_fill = util_Vec_NaN;
      break;
    case util_Mat_Row_Pos_Inf_NaN:
      row_fill = util_Vec_Pos_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Pos_Inf_NaN:
      row_fill = util_Vec_Pos_Pos_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Neg_Inf_NaN:
      row_fill = util_Vec_Pos_Neg_Inf_NaN;
      break;
    case util_Mat_Row_Pos_Big:
      row_fill = util_Vec_Pos_Big;
      break;
    case util_Mat_Row_Pos_Pos_Big:
      row_fill = util_Vec_Pos_Pos_Big;
      break;
    case util_Mat_Row_Pos_Neg_Big:
      row_fill = util_Vec_Pos_Neg_Big;
      break;
    case util_Mat_Row_Rand_Drop:
      row_fill = util_Vec_Rand_Drop;
      break;
    case util_Mat_Row_Rand:
      row_fill = util_Vec_Rand;
      break;
    case util_Mat_Row_2_Times_Rand_Minus_1_Drop:
      row_fill = util_Vec_2_Times_Rand_Minus_1_Drop;
      break;
    case util_Mat_Row_2_Times_Rand_Minus_1:
      row_fill = util_Vec_2_Times_Rand_Minus_1;
      break;
    case util_Mat_Row_Rand_Plus_Rand_Minus_1_Drop:
      row_fill = util_Vec_Rand_Plus_Rand_Minus_1_Drop;
      break;
    case util_Mat_Row_Rand_Plus_Rand_Minus_1:
      row_fill = util_Vec_Rand_Plus_Rand_Minus_1;
      break;
    case util_Mat_Row_Normal_Drop:
      row_fill = util_Vec_Normal_Drop;
      break;
    case util_Mat_Row_Normal:
      row_fill = util_Vec_Normal;
      break;
    case util_Mat_Row_Sine_Drop:
      row_fill = util_Vec_Sine_Drop;
      break;
    case util_Mat_Row_Sine:
      row_fill = util_Vec_Sine;
      break;
    case util_Mat_Row_Rand_Cond:
      row_fill = util_Vec_Rand_Cond;
      break;
    case util_Mat_Row_N_Cond:
      row_fill = util_Vec_N_Cond;
      break;
    case util_Mat_Row_N_Cond_Plus_Rand:
      row_fill = util_Vec_N_Cond_Plus_Rand;
      break;
    case util_Mat_Row_Full_Range:
      row_fill = util_Vec_Full_Range;
      break;
    case util_Mat_Row_Half_Range:
      row_fill = util_Vec_Half_Range;
      break;
    case util_Mat_Row_Mountain:
      row_fill = util_Vec_Mountain;
      break;
    case util_Mat_Row_Small_Plus_Increasing_Big:
      row_fill = util_Vec_Small_Plus_Increasing_Big;
      break;
    case util_Mat_Row_Small_Plus_Rand_Big:
      row_fill = util_Vec_Small_Plus_Rand_Big;
      break;
    default:
      exit(125); //TODO better error here
  }
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < M; i++){
            util_cvec_fill(N, A + i * lda, 1, row_fill, RealScale, ImagScale);
          }
          break;
        default:
          for(i = 0; i < N; i++){
            util_cvec_fill(M, A + i, lda, row_fill, RealScale, ImagScale);
            if(TransA == 'c' || TransA == 'C'){
              for(j = 0; j < M; j++){
                ((float*)A)[2 * (i + j * lda) + 1] = ((float*)A)[2 * (i + j * lda) + 1] * -1;
              }
            }
          }
          break;
      }
      break;
    default:
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < M; i++){
            util_cvec_fill(N, A + i, lda, row_fill, RealScale, ImagScale);
          }
          break;
        default:
          for(i = 0; i < N; i++){
            util_cvec_fill(M, A + i * lda, 1, row_fill, RealScale, ImagScale);
            if(TransA == 'c' || TransA == 'C'){
              for(j = 0; j < M; j++){
                ((float*)A)[2 * (i * lda + j) + 1] = ((float*)A)[2 * (i * lda + j) + 1] * -1;
              }
            }
          }
          break;
      }
      break;
  }
}

double *util_dmat_op(char Order, char TransA, int opM, int opN, double *A, int lda){
  int i;
  int j;
  double *opA = (double*)malloc(opM * opN * sizeof(double));
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = A[i * lda + j];
            }
          }
          break;
        default:
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = A[j * lda + i];
            }
          }
          break;
      }
      break;
    default:
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = A[j * lda + i];
            }
          }
          break;
        default:
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = A[i * lda + j];
            }
          }
          break;
      }
      break;
  }
  return opA;
}


double complex *util_zmat_op(char Order, char TransA, int opM, int opN, double complex *A, int lda){
  int i;
  int j;
  double complex *opA = (double complex*)malloc(opM * opN * sizeof(double complex));
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = A[i * lda + j];
            }
          }
          break;
        case 't':
        case 'T':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = A[j * lda + i];
            }
          }
          break;
        default:
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = conj(A[j * lda + i]);
            }
          }
          break;
      }
      break;
    default:
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = A[j * lda + i];
            }
          }
          break;
        case 't':
        case 'T':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = A[i * lda + j];
            }
          }
          break;
        default:
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = conj(A[i * lda + j]);
            }
          }
          break;
      }
      break;
  }
  return opA;
}

float *util_smat_op(char Order, char TransA, int opM, int opN, float *A, int lda){
  int i;
  int j;
  float *opA = (float*)malloc(opM * opN * sizeof(float));
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = A[i * lda + j];
            }
          }
          break;
        default:
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = A[j * lda + i];
            }
          }
          break;
      }
      break;
    default:
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = A[j * lda + i];
            }
          }
          break;
        default:
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = A[i * lda + j];
            }
          }
          break;
      }
      break;
  }
  return opA;
}


float complex *util_cmat_op(char Order, char TransA, int opM, int opN, float complex *A, int lda){
  int i;
  int j;
  float complex *opA = (float complex*)malloc(opM * opN * sizeof(float complex));
  switch(Order){
    case 'r':
    case 'R':
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = A[i * lda + j];
            }
          }
          break;
        case 't':
        case 'T':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = A[j * lda + i];
            }
          }
          break;
        default:
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[i * opN + j] = conj(A[j * lda + i]);
            }
          }
          break;
      }
      break;
    default:
      switch(TransA){
        case 'n':
        case 'N':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = A[j * lda + i];
            }
          }
          break;
        case 't':
        case 'T':
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = A[i * lda + j];
            }
          }
          break;
        default:
          for(i = 0; i < opM; i++){
            for(j = 0; j < opN; j++){
              opA[j * opM + i] = conj(A[i * lda + j]);
            }
          }
          break;
      }
      break;
  }
  return opA;
}
