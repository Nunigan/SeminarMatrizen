#include<stdio.h>
#include"c_matrix.h"

void MM(const int A[][n] ,const int B[][n], int C[][n],  int n);
void MM_dc(const int A[][n], const int B[][n], int C[][n], const int n);

void printMatrix(int C[][n]);
void split(const int in[][n], int out[][n], int n, int col, int row);
void join(const int in[][n], int out[][n], int n, int col, int row);
void add(const int A[][n] ,const int B[][n], int C[][n], int n);
void sub(const int A[][n] ,const int B[][n], int C[][n]);
void multiply(const int A[][n] ,const int B[][n], int C[][n]);

int main() 
{
  int C[n][n];
  MM_dc(A,B,C,n);
  printMatrix(C);
	return 0;
}

void MM(const int A[][n] ,const int B[][n], int C[][n],  int n)
{
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			C[i][j] = 0;
			for(int k=0;k<n;++k)
			{
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}



void MM_dc(const int A[][n], const int B[][n], int C[][n], const int n)
{
		printf("entered MM_dc");
		if(n<=2)
		{
	    multiply(A ,B, C);
		}
		else
	  {
    		int A11[n/2][n/2];
				int A12[n/2][n/2];
				int A21[n/2][n/2];
				int A22[n/2][n/2];
    		int B11[n/2][n/2];
				int B12[n/2][n/2];
				int B21[n/2][n/2];
				int B22[n/2][n/2];
		    
		    split(A, A11, n/2, 0 , 0);
        split(A, A12, n/2, 0 , n/2);
        split(A, A21, n/2, n/2, 0);
        split(A, A22, n/2, n/2, n/2);
        split(B, B11, n/2, 0 , 0);
        split(B, B12, n/2, 0 , n/2);
 	     	split(B, B21, n/2, n/2, 0);
   	    split(B, B22, n/2, n/2, n/2);
   	    		        	    		  	    		     
				int tmp1[n/2][n/2];
        int tmp2[n/2][n/2];
        int tmp3[n/2][n/2];
        int tmp4[n/2][n/2];       
        int tmp5[n/2][n/2];
        int tmp6[n/2][n/2];
        int tmp7[n/2][n/2];
        int tmp8[n/2][n/2];
        
               
        MM_dc(A11, B11, tmp1, n/2);
        MM_dc(A12, B21, tmp2, n/2);
        MM_dc(A11, B12, tmp3, n/2);
        MM_dc(A12, B22, tmp4, n/2);
        MM_dc(A21, B11, tmp5, n/2);
        MM_dc(A22, B21, tmp6, n/2);
        MM_dc(A21, B12, tmp7, n/2);
        MM_dc(A22, B22, tmp8, n/2);
        
      	int C11[n/2][n/2];
        int C12[n/2][n/2];
        int C21[n/2][n/2];
        int C22[n/2][n/2];  
        
        add(tmp1, tmp2, C11, n/2);
        add(tmp3, tmp4, C12, n/2);
        add(tmp5, tmp6, C21, n/2);
        add(tmp7, tmp8, C22, n/2);
        
        join(C11, C, n/2, 0, 0);
        join(C12, C, n/2, 0, n/2);
        join(C21, C, n/2, n/2, 0);
        join(C22, C, n/2, n/2, n/2);
        
	  }
}




void add(const int A[][n] ,const int B[][n], int C[][n], int n)
{
	for (int i = 0; i < n; i++)
	  for (int j = 0; j < n; j++)
        C[i][j] = A[i][j] + B[i][j];
}

void sub(const int A[][n] ,const int B[][n], int C[][n])
{
	C[0][0] = A[0][0]-B[0][0];
	C[0][1] = A[0][1]-B[0][1];
	C[1][0] = A[1][0]-B[1][0];
	C[1][1] = A[1][1]-B[1][1];
}


void multiply(const int A[][n] ,const int B[][n], int C[][n])
{
	C[0][0] = A[0][0]*B[0][0];
	C[0][1] = A[0][1]*B[0][1];
	C[1][0] = A[1][0]*B[1][0];
	C[1][1] = A[1][1]*B[1][1];
}

void split(const int in[][n], int out[][n], int n, int col, int row)
{
	for(int i1 = 0, i2 = col; i1 < n; i1++, i2++)
		for(int j1 = 0, j2 = row; j1 < n; j1++, j2++)
    	out[i1][j1] = in[i2][j2];
}

void join(const int in[][n], int out[][n], int n, int col, int row)
{
	for(int i1 = 0, i2 = col; i1 < n; i1++, i2++)
		for(int j1 = 0, j2 = row; j1 < n; j1++, j2++)
    	out[i2][j2] = in[i1][j1];
}



void printMatrix(int C[][n])
{
	for(int i=0;i<n;++i)
	  {
		    for(int j=0;j<n;++j)
		    {
		    	printf("%d ", C[i][j]);
		    }
		    printf("\n");
	   }
}
