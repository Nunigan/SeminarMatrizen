#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "c_matrix.h"
#include <cblas.h>

void MM(int *A, int *B, int *C, int n);
void MM_dc(int *A, int *B, int *C, int n);
void strassen(int *A, int *B, int *C, int n);
void printMatrix(int *C, int n);
void printMatrix_double(double *C, int n);
void split(int *in, int *out, int n, int col, int row);
void join(int *in, int *out, int n, int col, int row);
void add(int *A, int *B, int *C, int n);
void sub(int *A, int *B, int *C, int n);
void multiply(int *A, int *B, int *C, int n);

int main() {

	double *dC = (double*) malloc(n * n * sizeof(double));

	clock_t t;
	t = clock();
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, *dA, n,
			*dB, n, 0.0, dC, n);
	t = clock() - t;
	double time_taken = ((double) t) / CLOCKS_PER_SEC; // calculate the elapsed time
	printf("The Cblas program took %f seconds to execute \n", time_taken);
//	printMatrix_double( (double*)dC, n);

	free(dC);

	int *C = (int*) malloc(n * n * sizeof(int));

	t = clock();
	strassen((int*) A, (int*) B, (int*) C, n);
	t = clock() - t;
	time_taken = ((double) t) / CLOCKS_PER_SEC; // calculate the elapsed time
	printf("The Strassen program took %f seconds to execute \n", time_taken);
//	printMatrix( (int *)C, n);

	free(C);

	int *D = (int*) malloc(n * n * sizeof(int));

	t = clock();
	MM((int*) A, (int*) B, (int*) D, n);
	t = clock() - t;
	time_taken = ((double) t) / CLOCKS_PER_SEC; // calculate the elapsed time
	printf("The MM program took %f seconds to execute \n", time_taken);

	free(D);

	int *E = (int*) malloc(n * n * sizeof(int));

	t = clock();
	MM_dc((int*) A, (int*) B, (int*) E, n);
	t = clock() - t;
	time_taken = ((double) t) / CLOCKS_PER_SEC; // calculate the elapsed time
	printf("The MM_dc program took %f seconds to execute \n", time_taken);

	free(E);

	return 0;
}

void MM(int *A, int *B, int *C, int n) {
	int sum;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			sum = 0;
//			C[i][j] = 0;
			for (int k = 0; k < n; ++k) {
				sum += (*((A + i * n) + k)) * (*((B + k * n) + j));
//				C[i][j] += A[i][k]*B[k][j];
			}
			*((C + i * n) + j) = sum;
		}
	}
}

void MM_dc(int *A, int *B, int *C, int n) {
	if (n <= 2) {
		MM((int*) A, (int*) B, (int*) C, n);
	} else {
		int *A11 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *A12 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *A21 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *A22 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *B11 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *B12 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *B21 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *B22 = (int*) malloc(n / 2 * n / 2 * sizeof(int));

		split((int*) A, (int*) A11, n / 2, 0, 0);
		split((int*) A, (int*) A12, n / 2, 0, n / 2);
		split((int*) A, (int*) A21, n / 2, n / 2, 0);
		split((int*) A, (int*) A22, n / 2, n / 2, n / 2);
		split((int*) B, (int*) B11, n / 2, 0, 0);
		split((int*) B, (int*) B12, n / 2, 0, n / 2);
		split((int*) B, (int*) B21, n / 2, n / 2, 0);
		split((int*) B, (int*) B22, n / 2, n / 2, n / 2);

		int *tmp1 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp2 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp3 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp4 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp5 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp6 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp7 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp8 = (int*) malloc(n / 2 * n / 2 * sizeof(int));

		MM_dc((int*) A11, (int*) B11, (int*) tmp1, n / 2);
		MM_dc((int*) A12, (int*) B21, (int*) tmp2, n / 2);
		MM_dc((int*) A11, (int*) B12, (int*) tmp3, n / 2);
		MM_dc((int*) A12, (int*) B22, (int*) tmp4, n / 2);
		MM_dc((int*) A21, (int*) B11, (int*) tmp5, n / 2);
		MM_dc((int*) A22, (int*) B21, (int*) tmp6, n / 2);
		MM_dc((int*) A21, (int*) B12, (int*) tmp7, n / 2);
		MM_dc((int*) A22, (int*) B22, (int*) tmp8, n / 2);

		free(A11);
		free(A12);
		free(A21);
		free(A22);
		free(B11);
		free(B12);
		free(B21);
		free(B22);

		int *C11 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *C12 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *C21 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *C22 = (int*) malloc(n / 2 * n / 2 * sizeof(int));

		add((int*) tmp1, (int*) tmp2, (int*) C11, n / 2);
		add((int*) tmp3, (int*) tmp4, (int*) C12, n / 2);
		add((int*) tmp5, (int*) tmp6, (int*) C21, n / 2);
		add((int*) tmp7, (int*) tmp8, (int*) C22, n / 2);

		free(tmp1);
		free(tmp2);
		free(tmp3);
		free(tmp4);
		free(tmp5);
		free(tmp6);
		free(tmp7);
		free(tmp8);

		join((int*) C11, (int*) C, n / 2, 0, 0);
		join((int*) C12, (int*) C, n / 2, 0, n / 2);
		join((int*) C21, (int*) C, n / 2, n / 2, 0);
		join((int*) C22, (int*) C, n / 2, n / 2, n / 2);

		free(C11);
		free(C12);
		free(C21);
		free(C22);

	}
}

void strassen(int *A, int *B, int *C, int n) {
	if (n <= 2) {

		int P, Q, R, S, T, U, V;
		P = ((*((A + 0 * n) + 0)) + (*((A + 1 * n) + 1)))
				* ((*((B + 0 * n) + 0)) + (*((B + 1 * n) + 1)));
		Q = ((*((A + 1 * n) + 0)) + (*((A + 1 * n) + 1)))
				* ((*((B + 0 * n) + 0)));
		R = ((*((A + 0 * n) + 0)))
				* ((*((B + 0 * n) + 1)) - (*((B + 1 * n) + 1)));
		S = ((*((A + 1 * n) + 1)))
				* ((*((B + 1 * n) + 0)) - (*((B + 0 * n) + 0)));
		T = ((*((A + 0 * n) + 0)) + (*((A + 0 * n) + 1)))
				* ((*((B + 1 * n) + 1)));
		U = ((*((A + 1 * n) + 0)) - (*((A + 0 * n) + 0)))
				* ((*((B + 0 * n) + 0)) + (*((B + 0 * n) + 1)));
		V = ((*((A + 0 * n) + 1)) - (*((A + 1 * n) + 1)))
				* ((*((B + 1 * n) + 0)) + (*((B + 1 * n) + 1)));
		(*((C + 0 * n) + 0)) = P + S - T + V;
		(*((C + 0 * n) + 1)) = R + T;
		(*((C + 1 * n) + 0)) = Q + S;
		(*((C + 1 * n) + 1)) = P + R - Q + U;

	} else {
		int *A11 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *A12 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *A21 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *A22 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *B11 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *B12 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *B21 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *B22 = (int*) malloc(n / 2 * n / 2 * sizeof(int));

		split((int*) A, (int*) A11, n / 2, 0, 0);
		split((int*) A, (int*) A12, n / 2, 0, n / 2);
		split((int*) A, (int*) A21, n / 2, n / 2, 0);
		split((int*) A, (int*) A22, n / 2, n / 2, n / 2);
		split((int*) B, (int*) B11, n / 2, 0, 0);
		split((int*) B, (int*) B12, n / 2, 0, n / 2);
		split((int*) B, (int*) B21, n / 2, n / 2, 0);
		split((int*) B, (int*) B22, n / 2, n / 2, n / 2);

		int *tmp1 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp2 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp3 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp4 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp5 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp6 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp7 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *tmp8 = (int*) malloc(n / 2 * n / 2 * sizeof(int));

		strassen((int*) A11, (int*) B11, (int*) tmp1, n / 2);
		strassen((int*) A12, (int*) B21, (int*) tmp2, n / 2);
		strassen((int*) A11, (int*) B12, (int*) tmp3, n / 2);
		strassen((int*) A12, (int*) B22, (int*) tmp4, n / 2);
		strassen((int*) A21, (int*) B11, (int*) tmp5, n / 2);
		strassen((int*) A22, (int*) B21, (int*) tmp6, n / 2);
		strassen((int*) A21, (int*) B12, (int*) tmp7, n / 2);
		strassen((int*) A22, (int*) B22, (int*) tmp8, n / 2);

		free(A11);
		free(A12);
		free(A21);
		free(A22);
		free(B11);
		free(B12);
		free(B21);
		free(B22);

		int *C11 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *C12 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *C21 = (int*) malloc(n / 2 * n / 2 * sizeof(int));
		int *C22 = (int*) malloc(n / 2 * n / 2 * sizeof(int));

		add((int*) tmp1, (int*) tmp2, (int*) C11, n / 2);
		add((int*) tmp3, (int*) tmp4, (int*) C12, n / 2);
		add((int*) tmp5, (int*) tmp6, (int*) C21, n / 2);
		add((int*) tmp7, (int*) tmp8, (int*) C22, n / 2);

		free(tmp1);
		free(tmp2);
		free(tmp3);
		free(tmp4);
		free(tmp5);
		free(tmp6);
		free(tmp7);
		free(tmp8);

		join((int*) C11, (int*) C, n / 2, 0, 0);
		join((int*) C12, (int*) C, n / 2, 0, n / 2);
		join((int*) C21, (int*) C, n / 2, n / 2, 0);
		join((int*) C22, (int*) C, n / 2, n / 2, n / 2);

		free(C11);
		free(C12);
		free(C21);
		free(C22);
	}
}

void add(int *A, int *B, int *C, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			*((C + i * n) + j) = *((A + i * n) + j) + *((B + i * n) + j);
		}
	}
}

void sub(int *A, int *B, int *C, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			*((C + i * n) + j) = *((A + i * n) + j) - *((B + i * n) + j);
		}
	}
}

void multiply(int *A, int *B, int *C, int n) {
	int mul;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			mul = (*((A + i * n) + j)) * (*((B + i * n) + j));
			*((C + i * n) + j) = mul;
		}
	}
}

void split(int *in, int *out, int n, int col, int row) {
	for (int i1 = 0, i2 = col; i1 < n; i1++, i2++)
		for (int j1 = 0, j2 = row; j1 < n; j1++, j2++) {
			*((out + i1 * n) + j1) = *((in + i2 * n * 2) + j2);

		}
}

void join(int *in, int *out, int n, int col, int row) {
	for (int i1 = 0, i2 = col; i1 < n; i1++, i2++)
		for (int j1 = 0, j2 = row; j1 < n; j1++, j2++)
			*((out + i2 * n * 2) + j2) = *((in + i1 * n) + j1);
}

void printMatrix(int *C, int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%d ", *((C + i * n) + j));
		}
		printf("\n");
	}
}

void printMatrix_double(double *C, int n)
{
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%.0f ", *((C + i * n) + j));
		}
		printf("\n");
	}
}

