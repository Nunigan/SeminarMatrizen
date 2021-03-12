#include<stdio.h>

int * MM(int *A ,int *B, int n);

int main() {
  int n = 64;
	int A[n][n];
  int B[n][n];
  int C[n][n];


	return 0;
}

int * MM(int A[][] ,int B[][], int n)
{
  for(int i=0;i<n;++i)
  {
    for(int j=0;j<n;++j)
    {
      c[i][j] = 0;
      for(int k=0;k<n,++k)
      {
        c[i][j] += A;
      }
    }
  }
  return C;
}
