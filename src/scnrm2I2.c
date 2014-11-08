#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "config.h"
#include "Common/Common.h"
#include <immintrin.h>
#include <emmintrin.h>


#if defined( __AVX__ )
  void scnrm2I2(int n, float complex* v, int incv, float scale, int fold, float complex* sum){
    __m256 scale_mask = _mm256_set1_ps(scale);
    __m256 mask_BLP; AVX_BLP_MASKS(mask_BLP);
    float complex tmp_cons[4] __attribute__((aligned(32)));
    SET_DAZ_FLAG;
    switch(fold){
      case 3:{
        int i;

        float* sum_base = (float*) sum;
        float* v_base = (float*) v;
        __m256 v_0, v_1, v_2, v_3;
        __m256 q_0;
        __m256 s_0_0;
        __m256 s_1_0;
        __m256 s_2_0;

        s_0_0 = (__m256)_mm256_broadcast_sd((double *)(sum_base));
        s_1_0 = (__m256)_mm256_broadcast_sd((double *)(sum_base + 2));
        s_2_0 = (__m256)_mm256_broadcast_sd((double *)(sum_base + 4));
        if(incv == 1){

          for(i = 0; i + 16 <= n; i += 16, v_base += 32){
            v_0 = _mm256_mul_ps(_mm256_loadu_ps(v_base), scale_mask);
            v_1 = _mm256_mul_ps(_mm256_loadu_ps(v_base + 8), scale_mask);
            v_2 = _mm256_mul_ps(_mm256_loadu_ps(v_base + 16), scale_mask);
            v_3 = _mm256_mul_ps(_mm256_loadu_ps(v_base + 24), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            v_1 = _mm256_mul_ps(v_1, v_1);
            v_2 = _mm256_mul_ps(v_2, v_2);
            v_3 = _mm256_mul_ps(v_3, v_3);
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_1 = _mm256_add_ps(v_1, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_1 = _mm256_add_ps(v_1, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_2, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_2 = _mm256_add_ps(v_2, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_2, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_2 = _mm256_add_ps(v_2, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_2, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_3, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_3 = _mm256_add_ps(v_3, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_3, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_3 = _mm256_add_ps(v_3, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_3, mask_BLP));
          }
          if(i + 8 <= n){
            v_0 = _mm256_mul_ps(_mm256_loadu_ps(v_base), scale_mask);
            v_1 = _mm256_mul_ps(_mm256_loadu_ps(v_base + 8), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            v_1 = _mm256_mul_ps(v_1, v_1);
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_1 = _mm256_add_ps(v_1, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_1 = _mm256_add_ps(v_1, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_1, mask_BLP));
            i += 8, v_base += 16;
          }
          if(i + 4 <= n){
            v_0 = _mm256_mul_ps(_mm256_loadu_ps(v_base), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_0, mask_BLP));
            i += 4, v_base += 8;
          }
          if(i < n){
            v_0 = _mm256_mul_ps((__m256)_mm256_set_pd(0, (n - i)>2?((double*)v_base)[2]:0, (n - i)>1?((double*)v_base)[1]:0, ((double*)v_base)[0]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_0, mask_BLP));
          }
        }else{

          for(i = 0; i + 16 <= n; i += 16, v_base += (incv * 32)){
            v_0 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 6) + 1)], v_base[(incv * 6)], v_base[((incv * 4) + 1)], v_base[(incv * 4)], v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_1 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 14) + 1)], v_base[(incv * 14)], v_base[((incv * 12) + 1)], v_base[(incv * 12)], v_base[((incv * 10) + 1)], v_base[(incv * 10)], v_base[((incv * 8) + 1)], v_base[(incv * 8)]), scale_mask);
            v_2 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 22) + 1)], v_base[(incv * 22)], v_base[((incv * 20) + 1)], v_base[(incv * 20)], v_base[((incv * 18) + 1)], v_base[(incv * 18)], v_base[((incv * 16) + 1)], v_base[(incv * 16)]), scale_mask);
            v_3 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 30) + 1)], v_base[(incv * 30)], v_base[((incv * 28) + 1)], v_base[(incv * 28)], v_base[((incv * 26) + 1)], v_base[(incv * 26)], v_base[((incv * 24) + 1)], v_base[(incv * 24)]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            v_1 = _mm256_mul_ps(v_1, v_1);
            v_2 = _mm256_mul_ps(v_2, v_2);
            v_3 = _mm256_mul_ps(v_3, v_3);
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_1 = _mm256_add_ps(v_1, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_1 = _mm256_add_ps(v_1, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_2, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_2 = _mm256_add_ps(v_2, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_2, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_2 = _mm256_add_ps(v_2, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_2, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_3, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_3 = _mm256_add_ps(v_3, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_3, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_3 = _mm256_add_ps(v_3, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_3, mask_BLP));
          }
          if(i + 8 <= n){
            v_0 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 6) + 1)], v_base[(incv * 6)], v_base[((incv * 4) + 1)], v_base[(incv * 4)], v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_1 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 14) + 1)], v_base[(incv * 14)], v_base[((incv * 12) + 1)], v_base[(incv * 12)], v_base[((incv * 10) + 1)], v_base[(incv * 10)], v_base[((incv * 8) + 1)], v_base[(incv * 8)]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            v_1 = _mm256_mul_ps(v_1, v_1);
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_1 = _mm256_add_ps(v_1, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_1, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_1 = _mm256_add_ps(v_1, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_1, mask_BLP));
            i += 8, v_base += (incv * 16);
          }
          if(i + 4 <= n){
            v_0 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 6) + 1)], v_base[(incv * 6)], v_base[((incv * 4) + 1)], v_base[(incv * 4)], v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_0, mask_BLP));
            i += 4, v_base += (incv * 8);
          }
          if(i < n){
            v_0 = _mm256_mul_ps((__m256)_mm256_set_pd(0, (n - i)>2?((double*)v_base)[(incv * 2)]:0, (n - i)>1?((double*)v_base)[incv]:0, ((double*)v_base)[0]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            q_0 = s_0_0;
            s_0_0 = _mm256_add_ps(s_0_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_0_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm256_add_ps(s_1_0, _mm256_or_ps(v_0, mask_BLP));
            q_0 = _mm256_sub_ps(q_0, s_1_0);
            v_0 = _mm256_add_ps(v_0, q_0);
            s_2_0 = _mm256_add_ps(s_2_0, _mm256_or_ps(v_0, mask_BLP));
          }
        }
        s_0_0 = _mm256_sub_ps(s_0_0, _mm256_set_ps(sum_base[1], sum_base[0], sum_base[1], sum_base[0], sum_base[1], sum_base[0], 0, 0));
        _mm256_store_ps((float*)tmp_cons, s_0_0);
        sum[0] = tmp_cons[0] + tmp_cons[1] + tmp_cons[2] + tmp_cons[3];
        s_1_0 = _mm256_sub_ps(s_1_0, _mm256_set_ps(sum_base[3], sum_base[2], sum_base[3], sum_base[2], sum_base[3], sum_base[2], 0, 0));
        _mm256_store_ps((float*)tmp_cons, s_1_0);
        sum[1] = tmp_cons[0] + tmp_cons[1] + tmp_cons[2] + tmp_cons[3];
        s_2_0 = _mm256_sub_ps(s_2_0, _mm256_set_ps(sum_base[5], sum_base[4], sum_base[5], sum_base[4], sum_base[5], sum_base[4], 0, 0));
        _mm256_store_ps((float*)tmp_cons, s_2_0);
        sum[2] = tmp_cons[0] + tmp_cons[1] + tmp_cons[2] + tmp_cons[3];
        RESET_DAZ_FLAG
        return;
      }
      default:{
        int i, j;

        float* sum_base = (float*) sum;
        float* v_base = (float*) v;
        __m256 v_0, v_1;
        __m256 q_0, q_1;
        __m256 s_0, s_1;
        __m256 s_buffer[(MAX_FOLD * 2)];

        for(j = 0; j < fold; j += 1){
          s_buffer[(j * 2)] = s_buffer[((j * 2) + 1)] = (__m256)_mm256_broadcast_sd((double *)(sum_base + (j * 2)));
        }
        if(incv == 1){

          for(i = 0; i + 8 <= n; i += 8, v_base += 16){
            v_0 = _mm256_mul_ps(_mm256_loadu_ps(v_base), scale_mask);
            v_1 = _mm256_mul_ps(_mm256_loadu_ps(v_base + 8), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            v_1 = _mm256_mul_ps(v_1, v_1);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 2)];
              s_1 = s_buffer[((j * 2) + 1)];
              q_0 = _mm256_add_ps(s_0, _mm256_or_ps(v_0, mask_BLP));
              q_1 = _mm256_add_ps(s_1, _mm256_or_ps(v_1, mask_BLP));
              s_buffer[(j * 2)] = q_0;
              s_buffer[((j * 2) + 1)] = q_1;
              q_0 = _mm256_sub_ps(s_0, q_0);
              q_1 = _mm256_sub_ps(s_1, q_1);
              v_0 = _mm256_add_ps(v_0, q_0);
              v_1 = _mm256_add_ps(v_1, q_1);
            }
            s_buffer[(j * 2)] = _mm256_add_ps(s_buffer[(j * 2)], _mm256_or_ps(v_0, mask_BLP));
            s_buffer[((j * 2) + 1)] = _mm256_add_ps(s_buffer[((j * 2) + 1)], _mm256_or_ps(v_1, mask_BLP));
          }
          if(i + 4 <= n){
            v_0 = _mm256_mul_ps(_mm256_loadu_ps(v_base), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 2)];
              q_0 = _mm256_add_ps(s_0, _mm256_or_ps(v_0, mask_BLP));
              s_buffer[(j * 2)] = q_0;
              q_0 = _mm256_sub_ps(s_0, q_0);
              v_0 = _mm256_add_ps(v_0, q_0);
            }
            s_buffer[(j * 2)] = _mm256_add_ps(s_buffer[(j * 2)], _mm256_or_ps(v_0, mask_BLP));
            i += 4, v_base += 8;
          }
          if(i < n){
            v_0 = _mm256_mul_ps((__m256)_mm256_set_pd(0, (n - i)>2?((double*)v_base)[2]:0, (n - i)>1?((double*)v_base)[1]:0, ((double*)v_base)[0]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 2)];
              q_0 = _mm256_add_ps(s_0, _mm256_or_ps(v_0, mask_BLP));
              s_buffer[(j * 2)] = q_0;
              q_0 = _mm256_sub_ps(s_0, q_0);
              v_0 = _mm256_add_ps(v_0, q_0);
            }
            s_buffer[(j * 2)] = _mm256_add_ps(s_buffer[(j * 2)], _mm256_or_ps(v_0, mask_BLP));
          }
        }else{

          for(i = 0; i + 8 <= n; i += 8, v_base += (incv * 16)){
            v_0 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 6) + 1)], v_base[(incv * 6)], v_base[((incv * 4) + 1)], v_base[(incv * 4)], v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_1 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 14) + 1)], v_base[(incv * 14)], v_base[((incv * 12) + 1)], v_base[(incv * 12)], v_base[((incv * 10) + 1)], v_base[(incv * 10)], v_base[((incv * 8) + 1)], v_base[(incv * 8)]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            v_1 = _mm256_mul_ps(v_1, v_1);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 2)];
              s_1 = s_buffer[((j * 2) + 1)];
              q_0 = _mm256_add_ps(s_0, _mm256_or_ps(v_0, mask_BLP));
              q_1 = _mm256_add_ps(s_1, _mm256_or_ps(v_1, mask_BLP));
              s_buffer[(j * 2)] = q_0;
              s_buffer[((j * 2) + 1)] = q_1;
              q_0 = _mm256_sub_ps(s_0, q_0);
              q_1 = _mm256_sub_ps(s_1, q_1);
              v_0 = _mm256_add_ps(v_0, q_0);
              v_1 = _mm256_add_ps(v_1, q_1);
            }
            s_buffer[(j * 2)] = _mm256_add_ps(s_buffer[(j * 2)], _mm256_or_ps(v_0, mask_BLP));
            s_buffer[((j * 2) + 1)] = _mm256_add_ps(s_buffer[((j * 2) + 1)], _mm256_or_ps(v_1, mask_BLP));
          }
          if(i + 4 <= n){
            v_0 = _mm256_mul_ps(_mm256_set_ps(v_base[((incv * 6) + 1)], v_base[(incv * 6)], v_base[((incv * 4) + 1)], v_base[(incv * 4)], v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 2)];
              q_0 = _mm256_add_ps(s_0, _mm256_or_ps(v_0, mask_BLP));
              s_buffer[(j * 2)] = q_0;
              q_0 = _mm256_sub_ps(s_0, q_0);
              v_0 = _mm256_add_ps(v_0, q_0);
            }
            s_buffer[(j * 2)] = _mm256_add_ps(s_buffer[(j * 2)], _mm256_or_ps(v_0, mask_BLP));
            i += 4, v_base += (incv * 8);
          }
          if(i < n){
            v_0 = _mm256_mul_ps((__m256)_mm256_set_pd(0, (n - i)>2?((double*)v_base)[(incv * 2)]:0, (n - i)>1?((double*)v_base)[incv]:0, ((double*)v_base)[0]), scale_mask);
            v_0 = _mm256_mul_ps(v_0, v_0);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 2)];
              q_0 = _mm256_add_ps(s_0, _mm256_or_ps(v_0, mask_BLP));
              s_buffer[(j * 2)] = q_0;
              q_0 = _mm256_sub_ps(s_0, q_0);
              v_0 = _mm256_add_ps(v_0, q_0);
            }
            s_buffer[(j * 2)] = _mm256_add_ps(s_buffer[(j * 2)], _mm256_or_ps(v_0, mask_BLP));
          }
        }
        for(j = 0; j < fold; j += 1){
          s_buffer[(j * 2)] = _mm256_sub_ps(s_buffer[(j * 2)], _mm256_set_ps(sum_base[((j * 2) + 1)], sum_base[(j * 2)], sum_base[((j * 2) + 1)], sum_base[(j * 2)], sum_base[((j * 2) + 1)], sum_base[(j * 2)], 0, 0));
          q_0 = (__m256)_mm256_broadcast_sd((double *)(sum_base + (j * 2)));
          s_buffer[(j * 2)] = _mm256_add_ps(s_buffer[(j * 2)], _mm256_sub_ps(s_buffer[((j * 2) + 1)], q_0));
          _mm256_store_ps((float*)tmp_cons, s_buffer[(j * 2)]);
          sum[j] = tmp_cons[0] + tmp_cons[1] + tmp_cons[2] + tmp_cons[3];
        }
        RESET_DAZ_FLAG
        return;
      }
    }
  }
#elif defined( __SSE2__ )
  void scnrm2I2(int n, float complex* v, int incv, float scale, int fold, float complex* sum){
    __m128 scale_mask = _mm_set1_ps(scale);
    __m128 mask_BLP; SSE_BLP_MASKS(mask_BLP);
    float complex tmp_cons[2] __attribute__((aligned(16)));
    SET_DAZ_FLAG;
    switch(fold){
      case 3:{
        int i;

        float* sum_base = (float*) sum;
        float* v_base = (float*) v;
        __m128 v_0, v_1;
        __m128 q_0;
        __m128 s_0_0;
        __m128 s_1_0;
        __m128 s_2_0;

        s_0_0 = (__m128)_mm_load1_pd((double *)(sum_base));
        s_1_0 = (__m128)_mm_load1_pd((double *)(sum_base + 2));
        s_2_0 = (__m128)_mm_load1_pd((double *)(sum_base + 4));
        if(incv == 1){

          for(i = 0; i + 4 <= n; i += 4, v_base += 8){
            v_0 = _mm_mul_ps(_mm_loadu_ps(v_base), scale_mask);
            v_1 = _mm_mul_ps(_mm_loadu_ps(v_base + 4), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            v_1 = _mm_mul_ps(v_1, v_1);
            q_0 = s_0_0;
            s_0_0 = _mm_add_ps(s_0_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_0_0);
            v_0 = _mm_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm_add_ps(s_1_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_1_0);
            v_0 = _mm_add_ps(v_0, q_0);
            s_2_0 = _mm_add_ps(s_2_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm_add_ps(s_0_0, _mm_or_ps(v_1, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_0_0);
            v_1 = _mm_add_ps(v_1, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm_add_ps(s_1_0, _mm_or_ps(v_1, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_1_0);
            v_1 = _mm_add_ps(v_1, q_0);
            s_2_0 = _mm_add_ps(s_2_0, _mm_or_ps(v_1, mask_BLP));
          }
          if(i + 2 <= n){
            v_0 = _mm_mul_ps(_mm_loadu_ps(v_base), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            q_0 = s_0_0;
            s_0_0 = _mm_add_ps(s_0_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_0_0);
            v_0 = _mm_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm_add_ps(s_1_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_1_0);
            v_0 = _mm_add_ps(v_0, q_0);
            s_2_0 = _mm_add_ps(s_2_0, _mm_or_ps(v_0, mask_BLP));
            i += 2, v_base += 4;
          }
          if(i < n){
            v_0 = _mm_mul_ps(_mm_set_ps(0, 0, v_base[1], v_base[0]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            q_0 = s_0_0;
            s_0_0 = _mm_add_ps(s_0_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_0_0);
            v_0 = _mm_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm_add_ps(s_1_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_1_0);
            v_0 = _mm_add_ps(v_0, q_0);
            s_2_0 = _mm_add_ps(s_2_0, _mm_or_ps(v_0, mask_BLP));
          }
        }else{

          for(i = 0; i + 4 <= n; i += 4, v_base += (incv * 8)){
            v_0 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_1 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 6) + 1)], v_base[(incv * 6)], v_base[((incv * 4) + 1)], v_base[(incv * 4)]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            v_1 = _mm_mul_ps(v_1, v_1);
            q_0 = s_0_0;
            s_0_0 = _mm_add_ps(s_0_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_0_0);
            v_0 = _mm_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm_add_ps(s_1_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_1_0);
            v_0 = _mm_add_ps(v_0, q_0);
            s_2_0 = _mm_add_ps(s_2_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = s_0_0;
            s_0_0 = _mm_add_ps(s_0_0, _mm_or_ps(v_1, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_0_0);
            v_1 = _mm_add_ps(v_1, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm_add_ps(s_1_0, _mm_or_ps(v_1, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_1_0);
            v_1 = _mm_add_ps(v_1, q_0);
            s_2_0 = _mm_add_ps(s_2_0, _mm_or_ps(v_1, mask_BLP));
          }
          if(i + 2 <= n){
            v_0 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            q_0 = s_0_0;
            s_0_0 = _mm_add_ps(s_0_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_0_0);
            v_0 = _mm_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm_add_ps(s_1_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_1_0);
            v_0 = _mm_add_ps(v_0, q_0);
            s_2_0 = _mm_add_ps(s_2_0, _mm_or_ps(v_0, mask_BLP));
            i += 2, v_base += (incv * 4);
          }
          if(i < n){
            v_0 = _mm_mul_ps(_mm_set_ps(0, 0, v_base[1], v_base[0]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            q_0 = s_0_0;
            s_0_0 = _mm_add_ps(s_0_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_0_0);
            v_0 = _mm_add_ps(v_0, q_0);
            q_0 = s_1_0;
            s_1_0 = _mm_add_ps(s_1_0, _mm_or_ps(v_0, mask_BLP));
            q_0 = _mm_sub_ps(q_0, s_1_0);
            v_0 = _mm_add_ps(v_0, q_0);
            s_2_0 = _mm_add_ps(s_2_0, _mm_or_ps(v_0, mask_BLP));
          }
        }
        s_0_0 = _mm_sub_ps(s_0_0, _mm_set_ps(sum_base[1], sum_base[0], 0, 0));
        _mm_store_ps((float*)tmp_cons, s_0_0);
        sum[0] = tmp_cons[0] + tmp_cons[1];
        s_1_0 = _mm_sub_ps(s_1_0, _mm_set_ps(sum_base[3], sum_base[2], 0, 0));
        _mm_store_ps((float*)tmp_cons, s_1_0);
        sum[1] = tmp_cons[0] + tmp_cons[1];
        s_2_0 = _mm_sub_ps(s_2_0, _mm_set_ps(sum_base[5], sum_base[4], 0, 0));
        _mm_store_ps((float*)tmp_cons, s_2_0);
        sum[2] = tmp_cons[0] + tmp_cons[1];
        RESET_DAZ_FLAG
        return;
      }
      default:{
        int i, j;

        float* sum_base = (float*) sum;
        float* v_base = (float*) v;
        __m128 v_0, v_1, v_2, v_3;
        __m128 q_0, q_1, q_2, q_3;
        __m128 s_0, s_1, s_2, s_3;
        __m128 s_buffer[(MAX_FOLD * 4)];

        for(j = 0; j < fold; j += 1){
          s_buffer[(j * 4)] = s_buffer[((j * 4) + 1)] = s_buffer[((j * 4) + 2)] = s_buffer[((j * 4) + 3)] = (__m128)_mm_load1_pd((double *)(sum_base + (j * 2)));
        }
        if(incv == 1){

          for(i = 0; i + 8 <= n; i += 8, v_base += 16){
            v_0 = _mm_mul_ps(_mm_loadu_ps(v_base), scale_mask);
            v_1 = _mm_mul_ps(_mm_loadu_ps(v_base + 4), scale_mask);
            v_2 = _mm_mul_ps(_mm_loadu_ps(v_base + 8), scale_mask);
            v_3 = _mm_mul_ps(_mm_loadu_ps(v_base + 12), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            v_1 = _mm_mul_ps(v_1, v_1);
            v_2 = _mm_mul_ps(v_2, v_2);
            v_3 = _mm_mul_ps(v_3, v_3);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              s_1 = s_buffer[((j * 4) + 1)];
              s_2 = s_buffer[((j * 4) + 2)];
              s_3 = s_buffer[((j * 4) + 3)];
              q_0 = _mm_add_ps(s_0, _mm_or_ps(v_0, mask_BLP));
              q_1 = _mm_add_ps(s_1, _mm_or_ps(v_1, mask_BLP));
              q_2 = _mm_add_ps(s_2, _mm_or_ps(v_2, mask_BLP));
              q_3 = _mm_add_ps(s_3, _mm_or_ps(v_3, mask_BLP));
              s_buffer[(j * 4)] = q_0;
              s_buffer[((j * 4) + 1)] = q_1;
              s_buffer[((j * 4) + 2)] = q_2;
              s_buffer[((j * 4) + 3)] = q_3;
              q_0 = _mm_sub_ps(s_0, q_0);
              q_1 = _mm_sub_ps(s_1, q_1);
              q_2 = _mm_sub_ps(s_2, q_2);
              q_3 = _mm_sub_ps(s_3, q_3);
              v_0 = _mm_add_ps(v_0, q_0);
              v_1 = _mm_add_ps(v_1, q_1);
              v_2 = _mm_add_ps(v_2, q_2);
              v_3 = _mm_add_ps(v_3, q_3);
            }
            s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_or_ps(v_0, mask_BLP));
            s_buffer[((j * 4) + 1)] = _mm_add_ps(s_buffer[((j * 4) + 1)], _mm_or_ps(v_1, mask_BLP));
            s_buffer[((j * 4) + 2)] = _mm_add_ps(s_buffer[((j * 4) + 2)], _mm_or_ps(v_2, mask_BLP));
            s_buffer[((j * 4) + 3)] = _mm_add_ps(s_buffer[((j * 4) + 3)], _mm_or_ps(v_3, mask_BLP));
          }
          if(i + 4 <= n){
            v_0 = _mm_mul_ps(_mm_loadu_ps(v_base), scale_mask);
            v_1 = _mm_mul_ps(_mm_loadu_ps(v_base + 4), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            v_1 = _mm_mul_ps(v_1, v_1);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              s_1 = s_buffer[((j * 4) + 1)];
              q_0 = _mm_add_ps(s_0, _mm_or_ps(v_0, mask_BLP));
              q_1 = _mm_add_ps(s_1, _mm_or_ps(v_1, mask_BLP));
              s_buffer[(j * 4)] = q_0;
              s_buffer[((j * 4) + 1)] = q_1;
              q_0 = _mm_sub_ps(s_0, q_0);
              q_1 = _mm_sub_ps(s_1, q_1);
              v_0 = _mm_add_ps(v_0, q_0);
              v_1 = _mm_add_ps(v_1, q_1);
            }
            s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_or_ps(v_0, mask_BLP));
            s_buffer[((j * 4) + 1)] = _mm_add_ps(s_buffer[((j * 4) + 1)], _mm_or_ps(v_1, mask_BLP));
            i += 4, v_base += 8;
          }
          if(i + 2 <= n){
            v_0 = _mm_mul_ps(_mm_loadu_ps(v_base), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              q_0 = _mm_add_ps(s_0, _mm_or_ps(v_0, mask_BLP));
              s_buffer[(j * 4)] = q_0;
              q_0 = _mm_sub_ps(s_0, q_0);
              v_0 = _mm_add_ps(v_0, q_0);
            }
            s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_or_ps(v_0, mask_BLP));
            i += 2, v_base += 4;
          }
          if(i < n){
            v_0 = _mm_mul_ps(_mm_set_ps(0, 0, v_base[1], v_base[0]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              q_0 = _mm_add_ps(s_0, _mm_or_ps(v_0, mask_BLP));
              s_buffer[(j * 4)] = q_0;
              q_0 = _mm_sub_ps(s_0, q_0);
              v_0 = _mm_add_ps(v_0, q_0);
            }
            s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_or_ps(v_0, mask_BLP));
          }
        }else{

          for(i = 0; i + 8 <= n; i += 8, v_base += (incv * 16)){
            v_0 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_1 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 6) + 1)], v_base[(incv * 6)], v_base[((incv * 4) + 1)], v_base[(incv * 4)]), scale_mask);
            v_2 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 10) + 1)], v_base[(incv * 10)], v_base[((incv * 8) + 1)], v_base[(incv * 8)]), scale_mask);
            v_3 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 14) + 1)], v_base[(incv * 14)], v_base[((incv * 12) + 1)], v_base[(incv * 12)]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            v_1 = _mm_mul_ps(v_1, v_1);
            v_2 = _mm_mul_ps(v_2, v_2);
            v_3 = _mm_mul_ps(v_3, v_3);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              s_1 = s_buffer[((j * 4) + 1)];
              s_2 = s_buffer[((j * 4) + 2)];
              s_3 = s_buffer[((j * 4) + 3)];
              q_0 = _mm_add_ps(s_0, _mm_or_ps(v_0, mask_BLP));
              q_1 = _mm_add_ps(s_1, _mm_or_ps(v_1, mask_BLP));
              q_2 = _mm_add_ps(s_2, _mm_or_ps(v_2, mask_BLP));
              q_3 = _mm_add_ps(s_3, _mm_or_ps(v_3, mask_BLP));
              s_buffer[(j * 4)] = q_0;
              s_buffer[((j * 4) + 1)] = q_1;
              s_buffer[((j * 4) + 2)] = q_2;
              s_buffer[((j * 4) + 3)] = q_3;
              q_0 = _mm_sub_ps(s_0, q_0);
              q_1 = _mm_sub_ps(s_1, q_1);
              q_2 = _mm_sub_ps(s_2, q_2);
              q_3 = _mm_sub_ps(s_3, q_3);
              v_0 = _mm_add_ps(v_0, q_0);
              v_1 = _mm_add_ps(v_1, q_1);
              v_2 = _mm_add_ps(v_2, q_2);
              v_3 = _mm_add_ps(v_3, q_3);
            }
            s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_or_ps(v_0, mask_BLP));
            s_buffer[((j * 4) + 1)] = _mm_add_ps(s_buffer[((j * 4) + 1)], _mm_or_ps(v_1, mask_BLP));
            s_buffer[((j * 4) + 2)] = _mm_add_ps(s_buffer[((j * 4) + 2)], _mm_or_ps(v_2, mask_BLP));
            s_buffer[((j * 4) + 3)] = _mm_add_ps(s_buffer[((j * 4) + 3)], _mm_or_ps(v_3, mask_BLP));
          }
          if(i + 4 <= n){
            v_0 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_1 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 6) + 1)], v_base[(incv * 6)], v_base[((incv * 4) + 1)], v_base[(incv * 4)]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            v_1 = _mm_mul_ps(v_1, v_1);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              s_1 = s_buffer[((j * 4) + 1)];
              q_0 = _mm_add_ps(s_0, _mm_or_ps(v_0, mask_BLP));
              q_1 = _mm_add_ps(s_1, _mm_or_ps(v_1, mask_BLP));
              s_buffer[(j * 4)] = q_0;
              s_buffer[((j * 4) + 1)] = q_1;
              q_0 = _mm_sub_ps(s_0, q_0);
              q_1 = _mm_sub_ps(s_1, q_1);
              v_0 = _mm_add_ps(v_0, q_0);
              v_1 = _mm_add_ps(v_1, q_1);
            }
            s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_or_ps(v_0, mask_BLP));
            s_buffer[((j * 4) + 1)] = _mm_add_ps(s_buffer[((j * 4) + 1)], _mm_or_ps(v_1, mask_BLP));
            i += 4, v_base += (incv * 8);
          }
          if(i + 2 <= n){
            v_0 = _mm_mul_ps(_mm_set_ps(v_base[((incv * 2) + 1)], v_base[(incv * 2)], v_base[1], v_base[0]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              q_0 = _mm_add_ps(s_0, _mm_or_ps(v_0, mask_BLP));
              s_buffer[(j * 4)] = q_0;
              q_0 = _mm_sub_ps(s_0, q_0);
              v_0 = _mm_add_ps(v_0, q_0);
            }
            s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_or_ps(v_0, mask_BLP));
            i += 2, v_base += (incv * 4);
          }
          if(i < n){
            v_0 = _mm_mul_ps(_mm_set_ps(0, 0, v_base[1], v_base[0]), scale_mask);
            v_0 = _mm_mul_ps(v_0, v_0);
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              q_0 = _mm_add_ps(s_0, _mm_or_ps(v_0, mask_BLP));
              s_buffer[(j * 4)] = q_0;
              q_0 = _mm_sub_ps(s_0, q_0);
              v_0 = _mm_add_ps(v_0, q_0);
            }
            s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_or_ps(v_0, mask_BLP));
          }
        }
        for(j = 0; j < fold; j += 1){
          s_buffer[(j * 4)] = _mm_sub_ps(s_buffer[(j * 4)], _mm_set_ps(sum_base[((j * 2) + 1)], sum_base[(j * 2)], 0, 0));
          q_0 = (__m128)_mm_load1_pd((double *)(sum_base + (j * 2)));
          s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_sub_ps(s_buffer[((j * 4) + 1)], q_0));
          s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_sub_ps(s_buffer[((j * 4) + 2)], q_0));
          s_buffer[(j * 4)] = _mm_add_ps(s_buffer[(j * 4)], _mm_sub_ps(s_buffer[((j * 4) + 3)], q_0));
          _mm_store_ps((float*)tmp_cons, s_buffer[(j * 4)]);
          sum[j] = tmp_cons[0] + tmp_cons[1];
        }
        RESET_DAZ_FLAG
        return;
      }
    }
  }
#else
  void scnrm2I2(int n, float complex* v, int incv, float scale, int fold, float complex* sum){
    float scale_mask = scale;
    i_float tmp_BLP;
    SET_DAZ_FLAG;
    switch(fold){
      case 3:{
        int i;

        float* sum_base = (float*) sum;
        float* v_base = (float*) v;
        float v_0, v_1, v_2, v_3;
        float q_0, q_1;
        float s_0_0, s_0_1;
        float s_1_0, s_1_1;
        float s_2_0, s_2_1;

        s_0_0 = sum_base[0];
        s_0_1 = sum_base[1];
        s_1_0 = sum_base[2];
        s_1_1 = sum_base[3];
        s_2_0 = sum_base[4];
        s_2_1 = sum_base[5];
        if(incv == 1){

          for(i = 0; i + 2 <= n; i += 2, v_base += 4){
            v_0 = v_base[0] * scale_mask;
            v_1 = v_base[1] * scale_mask;
            v_2 = v_base[2] * scale_mask;
            v_3 = v_base[3] * scale_mask;
            v_0 = v_0 * v_0;
            v_1 = v_1 * v_1;
            v_2 = v_2 * v_2;
            v_3 = v_3 * v_3;
            q_0 = s_0_0;
            q_1 = s_0_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_0_0 = s_0_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_0_1 = s_0_1 + tmp_BLP.f;
            q_0 = q_0 - s_0_0;
            q_1 = q_1 - s_0_1;
            v_0 = v_0 + q_0;
            v_1 = v_1 + q_1;
            q_0 = s_1_0;
            q_1 = s_1_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_1_0 = s_1_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_1_1 = s_1_1 + tmp_BLP.f;
            q_0 = q_0 - s_1_0;
            q_1 = q_1 - s_1_1;
            v_0 = v_0 + q_0;
            v_1 = v_1 + q_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_2_0 = s_2_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_2_1 = s_2_1 + tmp_BLP.f;
            q_0 = s_0_0;
            q_1 = s_0_1;
            tmp_BLP.f = v_2;
            tmp_BLP.i |= 1;
            s_0_0 = s_0_0 + tmp_BLP.f;
            tmp_BLP.f = v_3;
            tmp_BLP.i |= 1;
            s_0_1 = s_0_1 + tmp_BLP.f;
            q_0 = q_0 - s_0_0;
            q_1 = q_1 - s_0_1;
            v_2 = v_2 + q_0;
            v_3 = v_3 + q_1;
            q_0 = s_1_0;
            q_1 = s_1_1;
            tmp_BLP.f = v_2;
            tmp_BLP.i |= 1;
            s_1_0 = s_1_0 + tmp_BLP.f;
            tmp_BLP.f = v_3;
            tmp_BLP.i |= 1;
            s_1_1 = s_1_1 + tmp_BLP.f;
            q_0 = q_0 - s_1_0;
            q_1 = q_1 - s_1_1;
            v_2 = v_2 + q_0;
            v_3 = v_3 + q_1;
            tmp_BLP.f = v_2;
            tmp_BLP.i |= 1;
            s_2_0 = s_2_0 + tmp_BLP.f;
            tmp_BLP.f = v_3;
            tmp_BLP.i |= 1;
            s_2_1 = s_2_1 + tmp_BLP.f;
          }
          if(i + 1 <= n){
            v_0 = v_base[0] * scale_mask;
            v_1 = v_base[1] * scale_mask;
            v_0 = v_0 * v_0;
            v_1 = v_1 * v_1;
            q_0 = s_0_0;
            q_1 = s_0_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_0_0 = s_0_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_0_1 = s_0_1 + tmp_BLP.f;
            q_0 = q_0 - s_0_0;
            q_1 = q_1 - s_0_1;
            v_0 = v_0 + q_0;
            v_1 = v_1 + q_1;
            q_0 = s_1_0;
            q_1 = s_1_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_1_0 = s_1_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_1_1 = s_1_1 + tmp_BLP.f;
            q_0 = q_0 - s_1_0;
            q_1 = q_1 - s_1_1;
            v_0 = v_0 + q_0;
            v_1 = v_1 + q_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_2_0 = s_2_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_2_1 = s_2_1 + tmp_BLP.f;
            i += 1, v_base += 2;
          }
        }else{

          for(i = 0; i + 2 <= n; i += 2, v_base += (incv * 4)){
            v_0 = v_base[0] * scale_mask;
            v_1 = v_base[1] * scale_mask;
            v_2 = v_base[(incv * 2)] * scale_mask;
            v_3 = v_base[((incv * 2) + 1)] * scale_mask;
            v_0 = v_0 * v_0;
            v_1 = v_1 * v_1;
            v_2 = v_2 * v_2;
            v_3 = v_3 * v_3;
            q_0 = s_0_0;
            q_1 = s_0_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_0_0 = s_0_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_0_1 = s_0_1 + tmp_BLP.f;
            q_0 = q_0 - s_0_0;
            q_1 = q_1 - s_0_1;
            v_0 = v_0 + q_0;
            v_1 = v_1 + q_1;
            q_0 = s_1_0;
            q_1 = s_1_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_1_0 = s_1_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_1_1 = s_1_1 + tmp_BLP.f;
            q_0 = q_0 - s_1_0;
            q_1 = q_1 - s_1_1;
            v_0 = v_0 + q_0;
            v_1 = v_1 + q_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_2_0 = s_2_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_2_1 = s_2_1 + tmp_BLP.f;
            q_0 = s_0_0;
            q_1 = s_0_1;
            tmp_BLP.f = v_2;
            tmp_BLP.i |= 1;
            s_0_0 = s_0_0 + tmp_BLP.f;
            tmp_BLP.f = v_3;
            tmp_BLP.i |= 1;
            s_0_1 = s_0_1 + tmp_BLP.f;
            q_0 = q_0 - s_0_0;
            q_1 = q_1 - s_0_1;
            v_2 = v_2 + q_0;
            v_3 = v_3 + q_1;
            q_0 = s_1_0;
            q_1 = s_1_1;
            tmp_BLP.f = v_2;
            tmp_BLP.i |= 1;
            s_1_0 = s_1_0 + tmp_BLP.f;
            tmp_BLP.f = v_3;
            tmp_BLP.i |= 1;
            s_1_1 = s_1_1 + tmp_BLP.f;
            q_0 = q_0 - s_1_0;
            q_1 = q_1 - s_1_1;
            v_2 = v_2 + q_0;
            v_3 = v_3 + q_1;
            tmp_BLP.f = v_2;
            tmp_BLP.i |= 1;
            s_2_0 = s_2_0 + tmp_BLP.f;
            tmp_BLP.f = v_3;
            tmp_BLP.i |= 1;
            s_2_1 = s_2_1 + tmp_BLP.f;
          }
          if(i + 1 <= n){
            v_0 = v_base[0] * scale_mask;
            v_1 = v_base[1] * scale_mask;
            v_0 = v_0 * v_0;
            v_1 = v_1 * v_1;
            q_0 = s_0_0;
            q_1 = s_0_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_0_0 = s_0_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_0_1 = s_0_1 + tmp_BLP.f;
            q_0 = q_0 - s_0_0;
            q_1 = q_1 - s_0_1;
            v_0 = v_0 + q_0;
            v_1 = v_1 + q_1;
            q_0 = s_1_0;
            q_1 = s_1_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_1_0 = s_1_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_1_1 = s_1_1 + tmp_BLP.f;
            q_0 = q_0 - s_1_0;
            q_1 = q_1 - s_1_1;
            v_0 = v_0 + q_0;
            v_1 = v_1 + q_1;
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_2_0 = s_2_0 + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_2_1 = s_2_1 + tmp_BLP.f;
            i += 1, v_base += (incv * 2);
          }
        }
        ((float*)sum)[0] = s_0_0;
        ((float*)sum)[1] = s_0_1;
        ((float*)sum)[2] = s_1_0;
        ((float*)sum)[3] = s_1_1;
        ((float*)sum)[4] = s_2_0;
        ((float*)sum)[5] = s_2_1;
        RESET_DAZ_FLAG
        return;
      }
      default:{
        int i, j;

        float* sum_base = (float*) sum;
        float* v_base = (float*) v;
        float v_0, v_1, v_2, v_3;
        float q_0, q_1, q_2, q_3;
        float s_0, s_1, s_2, s_3;
        float s_buffer[(MAX_FOLD * 4)];

        for(j = 0; j < fold; j += 1){
          s_buffer[(j * 4)] = s_buffer[((j * 4) + 2)] = sum_base[(j * 2)];
          s_buffer[((j * 4) + 1)] = s_buffer[((j * 4) + 3)] = sum_base[((j * 2) + 1)];
        }
        if(incv == 1){

          for(i = 0; i + 2 <= n; i += 2, v_base += 4){
            v_0 = v_base[0] * scale_mask;
            v_1 = v_base[1] * scale_mask;
            v_2 = v_base[2] * scale_mask;
            v_3 = v_base[3] * scale_mask;
            v_0 = v_0 * v_0;
            v_1 = v_1 * v_1;
            v_2 = v_2 * v_2;
            v_3 = v_3 * v_3;
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              s_1 = s_buffer[((j * 4) + 1)];
              s_2 = s_buffer[((j * 4) + 2)];
              s_3 = s_buffer[((j * 4) + 3)];
              tmp_BLP.f = v_0;
              tmp_BLP.i |= 1;
              q_0 = s_0 + tmp_BLP.f;
              tmp_BLP.f = v_1;
              tmp_BLP.i |= 1;
              q_1 = s_1 + tmp_BLP.f;
              tmp_BLP.f = v_2;
              tmp_BLP.i |= 1;
              q_2 = s_2 + tmp_BLP.f;
              tmp_BLP.f = v_3;
              tmp_BLP.i |= 1;
              q_3 = s_3 + tmp_BLP.f;
              s_buffer[(j * 4)] = q_0;
              s_buffer[((j * 4) + 1)] = q_1;
              s_buffer[((j * 4) + 2)] = q_2;
              s_buffer[((j * 4) + 3)] = q_3;
              q_0 = s_0 - q_0;
              q_1 = s_1 - q_1;
              q_2 = s_2 - q_2;
              q_3 = s_3 - q_3;
              v_0 = v_0 + q_0;
              v_1 = v_1 + q_1;
              v_2 = v_2 + q_2;
              v_3 = v_3 + q_3;
            }
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_buffer[(j * 4)] = s_buffer[(j * 4)] + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_buffer[((j * 4) + 1)] = s_buffer[((j * 4) + 1)] + tmp_BLP.f;
            tmp_BLP.f = v_2;
            tmp_BLP.i |= 1;
            s_buffer[((j * 4) + 2)] = s_buffer[((j * 4) + 2)] + tmp_BLP.f;
            tmp_BLP.f = v_3;
            tmp_BLP.i |= 1;
            s_buffer[((j * 4) + 3)] = s_buffer[((j * 4) + 3)] + tmp_BLP.f;
          }
          if(i + 1 <= n){
            v_0 = v_base[0] * scale_mask;
            v_1 = v_base[1] * scale_mask;
            v_0 = v_0 * v_0;
            v_1 = v_1 * v_1;
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              s_1 = s_buffer[((j * 4) + 1)];
              tmp_BLP.f = v_0;
              tmp_BLP.i |= 1;
              q_0 = s_0 + tmp_BLP.f;
              tmp_BLP.f = v_1;
              tmp_BLP.i |= 1;
              q_1 = s_1 + tmp_BLP.f;
              s_buffer[(j * 4)] = q_0;
              s_buffer[((j * 4) + 1)] = q_1;
              q_0 = s_0 - q_0;
              q_1 = s_1 - q_1;
              v_0 = v_0 + q_0;
              v_1 = v_1 + q_1;
            }
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_buffer[(j * 4)] = s_buffer[(j * 4)] + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_buffer[((j * 4) + 1)] = s_buffer[((j * 4) + 1)] + tmp_BLP.f;
            i += 1, v_base += 2;
          }
        }else{

          for(i = 0; i + 2 <= n; i += 2, v_base += (incv * 4)){
            v_0 = v_base[0] * scale_mask;
            v_1 = v_base[1] * scale_mask;
            v_2 = v_base[(incv * 2)] * scale_mask;
            v_3 = v_base[((incv * 2) + 1)] * scale_mask;
            v_0 = v_0 * v_0;
            v_1 = v_1 * v_1;
            v_2 = v_2 * v_2;
            v_3 = v_3 * v_3;
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              s_1 = s_buffer[((j * 4) + 1)];
              s_2 = s_buffer[((j * 4) + 2)];
              s_3 = s_buffer[((j * 4) + 3)];
              tmp_BLP.f = v_0;
              tmp_BLP.i |= 1;
              q_0 = s_0 + tmp_BLP.f;
              tmp_BLP.f = v_1;
              tmp_BLP.i |= 1;
              q_1 = s_1 + tmp_BLP.f;
              tmp_BLP.f = v_2;
              tmp_BLP.i |= 1;
              q_2 = s_2 + tmp_BLP.f;
              tmp_BLP.f = v_3;
              tmp_BLP.i |= 1;
              q_3 = s_3 + tmp_BLP.f;
              s_buffer[(j * 4)] = q_0;
              s_buffer[((j * 4) + 1)] = q_1;
              s_buffer[((j * 4) + 2)] = q_2;
              s_buffer[((j * 4) + 3)] = q_3;
              q_0 = s_0 - q_0;
              q_1 = s_1 - q_1;
              q_2 = s_2 - q_2;
              q_3 = s_3 - q_3;
              v_0 = v_0 + q_0;
              v_1 = v_1 + q_1;
              v_2 = v_2 + q_2;
              v_3 = v_3 + q_3;
            }
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_buffer[(j * 4)] = s_buffer[(j * 4)] + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_buffer[((j * 4) + 1)] = s_buffer[((j * 4) + 1)] + tmp_BLP.f;
            tmp_BLP.f = v_2;
            tmp_BLP.i |= 1;
            s_buffer[((j * 4) + 2)] = s_buffer[((j * 4) + 2)] + tmp_BLP.f;
            tmp_BLP.f = v_3;
            tmp_BLP.i |= 1;
            s_buffer[((j * 4) + 3)] = s_buffer[((j * 4) + 3)] + tmp_BLP.f;
          }
          if(i + 1 <= n){
            v_0 = v_base[0] * scale_mask;
            v_1 = v_base[1] * scale_mask;
            v_0 = v_0 * v_0;
            v_1 = v_1 * v_1;
            for(j = 0; j < fold - 1; j++){
              s_0 = s_buffer[(j * 4)];
              s_1 = s_buffer[((j * 4) + 1)];
              tmp_BLP.f = v_0;
              tmp_BLP.i |= 1;
              q_0 = s_0 + tmp_BLP.f;
              tmp_BLP.f = v_1;
              tmp_BLP.i |= 1;
              q_1 = s_1 + tmp_BLP.f;
              s_buffer[(j * 4)] = q_0;
              s_buffer[((j * 4) + 1)] = q_1;
              q_0 = s_0 - q_0;
              q_1 = s_1 - q_1;
              v_0 = v_0 + q_0;
              v_1 = v_1 + q_1;
            }
            tmp_BLP.f = v_0;
            tmp_BLP.i |= 1;
            s_buffer[(j * 4)] = s_buffer[(j * 4)] + tmp_BLP.f;
            tmp_BLP.f = v_1;
            tmp_BLP.i |= 1;
            s_buffer[((j * 4) + 1)] = s_buffer[((j * 4) + 1)] + tmp_BLP.f;
            i += 1, v_base += (incv * 2);
          }
        }
        for(j = 0; j < fold; j += 1){
          q_0 = ((float*)sum)[(j * 2)];
          s_buffer[(j * 4)] = s_buffer[(j * 4)] + (s_buffer[((j * 4) + 2)] - q_0);
          q_0 = ((float*)sum)[((j * 2) + 1)];
          s_buffer[((j * 4) + 1)] = s_buffer[((j * 4) + 1)] + (s_buffer[((j * 4) + 3)] - q_0);
          ((float*)sum)[(j * 2)] = s_buffer[(j * 4)];
          ((float*)sum)[((j * 2) + 1)] = s_buffer[((j * 4) + 1)];
        }
        RESET_DAZ_FLAG
        return;
      }
    }
  }
#endif