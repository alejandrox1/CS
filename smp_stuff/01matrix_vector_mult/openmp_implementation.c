/*
 * Calculate the result vector a by computing the dot product of the rows of
 * the matrix B and vecctor c:
 *
 * a_{m x 1} = B_{m x n} c_{n x 1}
 *
 * Row variant:
 * a_i = \sum_{j=1}^{n} B_{i,j} * c_j          for i = 1 ,..., n
 *
 * */
#include <stdio.h>
#include <stdlib.h>

// Matrix times vector routine. The matrix b is of size m by n, the result is
// stored in the vector a of size n by 1.
void mxv(int m, int n, double * restrict a, double * restrict b, double * restrict c);


int main(int argc, char *argv[]) {
	double *a, *b, *c;
	int i, j, m, n;
	
	printf("Please give m and n: ");
	scanf("%d %d", &m, &n);

	if ( (a=(double *)malloc(m*sizeof(double))) == NULL )
		perror("memory allocation for a");
	if ( (b=(double *)malloc(m*n*sizeof(double))) == NULL )
		perror("memory allocation for b");
	if ( (c=(double *)malloc(n*sizeof(double))) == NULL )
		perror("memory allocation for c");

	printf("Initializing matrix B and vector c\n");
	for (j=0; j<n; j++) 
		c[j] = 2.0;
	for (i=0; i<m; i++)
		for (j=0; j<n; j++)
			b[i*n + j] = i;

	printf("Executing the mxv function for m = %d and n = %d\n", m, n);
	(void) mxv(m, n, a, b, c);

	free(a); free(b); free(c);
	return(0);
}

// Row invariant of the problem.
void mxv(int m, int n, double * restrict a, double * restrict b, double * restrict c) {
	int i, j;

#pragma omp parallel for default(none) shared(m,n,a,b,c) private(i,j)
	for (i=0; i<m; i++) {
		a[i] = 0.0;
		// Dot product of row i of b with c.
		for (j=0; j<n; j++) {
			a[i] += b[i*n + j] * c[j];
		}
	}
}

