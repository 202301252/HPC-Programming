#include <math.h>
#include "utils.h"
#include <stdlib.h>


/* =====================================================
   Problem 01: Basic Matrix Multiplication (i-j-k)
   ===================================================== */
void matrix_multiplication(double** m1, double** m2, double** result, int N) {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            double sum = 0.0;

            for (int k = 0; k < N; k++) {
                sum += m1[i][k] * m2[k][j];
            }

            result[i][j] = sum;
        }
    }
}

/* =====================================================
   Problem 02: Transpose a Matrix
   ===================================================== */
void transpose(double** m, double** mt, int N) {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mt[j][i] = m[i][j];
        }
    }
}

/* =====================================================
   Problem 02: Matrix Multiplication using Transpose
   ===================================================== */
void transposed_matrix_multiplication(double** m1, double** m2, double** result, int N) {

    // Allocate transpose matrix
    double** mt = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        mt[i] = (double*)malloc(N * sizeof(double));
    }

    // Transpose second matrix
    transpose(m2, mt, N);

    // Multiply using transposed matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            double sum = 0.0;

            for (int k = 0; k < N; k++) {
                sum += m1[i][k] * mt[j][k];
            }

            result[i][j] = sum;
        }
    }

    // Free transpose matrix
    for (int i = 0; i < N; i++) {
        free(mt[i]);
    }
    free(mt);
}

/* =====================================================
   Problem 03: Blocked Matrix Multiplication
   ===================================================== */
void block_matrix_multiplication(double** m1, double** m2, double** result, int B, int N) {

    for (int ii = 0; ii < N; ii += B) {
        for (int jj = 0; jj < N; jj += B) {
            for (int kk = 0; kk < N; kk += B) {

                for (int i = ii; i < ii + B && i < N; i++) {
                    for (int j = jj; j < jj + B && j < N; j++) {

                        double sum = result[i][j];

                        for (int k = kk; k < kk + B && k < N; k++) {
                            sum += m1[i][k] * m2[k][j];
                        }

                        result[i][j] = sum;
                    }
                }
            }
        }
    }
}

void matrix_multiplication_ikj(double **A, double **B, double **C, int N) {

    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            double aik = A[i][k];
            for (int j = 0; j < N; j++) {
                C[i][j] += aik * B[k][j];
            }
        }
    }
}

void matrix_multiplication_kij(double **A, double **B, double **C, int N) {

    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            double aik = A[i][k];
            for (int j = 0; j < N; j++) {
                C[i][j] += aik * B[k][j];
            }
        }
    }
}

