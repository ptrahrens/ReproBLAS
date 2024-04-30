#include <stdlib.h>
#include <string.h>

#include <reproBLAS.h>
#include <binnedBLAS.h>

/**
 * @brief Add to double precision matrix C the reproducible matrix-matrix product of double precision matrices A and B
 *
 * Performs one of the matrix-matrix operations
 *
 *   C := alpha*op(A)*op(B) + beta*C,
 *
 * where  op(X) is one of
 *
 *   op(X) = X   or   op(X) = X**T,
 *
 * alpha and beta are scalars, A and B and C are matrices with op(A) an M by K matrix, op(B) a K by N matrix, and C is an M by N matrix.
 *
 * The matrix-matrix product is computed using binned types with #binnedBLAS_dbdgemm()
 *
 * @param fold the fold of the binned types
 * @param Order a character specifying the matrix ordering ('r' or 'R' for row-major, 'c' or 'C' for column major)
 * @param TransA a character specifying whether or not to transpose A before taking the matrix-matrix product ('n' or 'N' not to transpose, 't' or 'T' or 'c' or 'C' to transpose)
 * @param TransB a character specifying whether or not to transpose B before taking the matrix-matrix product ('n' or 'N' not to transpose, 't' or 'T' or 'c' or 'C' to transpose)
 * @param M number of rows of matrix op(A) and of the matrix C.
 * @param N number of columns of matrix op(B) and of the matrix C.
 * @param K number of columns of matrix op(A) and columns of the matrix op(B).
 * @param alpha scalar alpha
 * @param A double precision matrix of dimension (ma, lda) in row-major or (lda, na) in column-major. (ma, na) is (M, K) if A is not transposed and (K, M) otherwise.
 * @param lda the first dimension of A as declared in the calling program. lda must be at least na in row major or ma in column major.
 * @param B double precision matrix of dimension (mb, ldb) in row-major or (ldb, nb) in column-major. (mb, nb) is (K, N) if B is not transposed and (N, K) otherwise.
 * @param ldb the first dimension of B as declared in the calling program. ldb must be at least nb in row major or mb in column major.
 * @param beta scalar beta
 * @param C double precision matrix of dimension (M, ldc) in row-major or (ldc, N) in column-major.
 * @param ldc the first dimension of C as declared in the calling program. ldc must be at least N in row major or M in column major.
 *
 * @author Willow Ahrens
 * @date   18 Jan 2016
 */
void reproBLAS_rdgemm(const int fold, const char Order, const char TransA, const char TransB,
                      const int M, const int N, const int K,
                      const double alpha, const double *A, const int lda,
                      const double *B, const int ldb,
                      const double beta, double *C, const int ldc){
  double_binned *CI;
  int i;
  int j;

  if(M == 0 || N == 0){
    return;
  }

  CI = (double_binned*)malloc(M * N * binned_dbsize(fold));
  switch(Order){
    case 'r':
    case 'R':
      if(beta == 0.0){
        memset(CI, 0, M * N * binned_dbsize(fold));
      }else if(beta == 1.0){
        for(i = 0; i < M; i++){
          for(j = 0; j < N; j++){
            binned_dbdconv(fold, C[i * ldc + j], CI + (i * N + j) * binned_dbnum(fold));
          }
        }
      }else{
        for(i = 0; i < M; i++){
          for(j = 0; j < N; j++){
            binned_dbdconv(fold, C[i * ldc + j] * beta, CI + (i * N + j) * binned_dbnum(fold));
          }
        }
      }
      binnedBLAS_dbdgemm(fold, Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, CI, N);
      for(i = 0; i < M; i++){
        for(j = 0; j < N; j++){
          C[i * ldc + j] = binned_ddbconv(fold, CI + (i * N + j) * binned_dbnum(fold));
        }
      }
      break;
    default:
      if(beta == 0.0){
        memset(CI, 0, M * N * binned_dbsize(fold));
      }else if(beta == 1.0){
        for(j = 0; j < N; j++){
          for(i = 0; i < M; i++){
            binned_dbdconv(fold, C[j * ldc + i], CI + (j * M + i) * binned_dbnum(fold));
          }
        }
      }else{
        for(j = 0; j < N; j++){
          for(i = 0; i < M; i++){
            binned_dbdconv(fold, C[j * ldc + i] * beta, CI + (j * M + i) * binned_dbnum(fold));
          }
        }
      }
      binnedBLAS_dbdgemm(fold, Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, CI, M);
      for(j = 0; j < N; j++){
        for(i = 0; i < M; i++){
          C[j * ldc + i] = binned_ddbconv(fold, CI + (j * M + i) * binned_dbnum(fold));
        }
      }
      break;
  }
  free(CI);
}
