#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace cv;
using namespace std;

void conv(int S, int P, int F, int F_num, int W_row , int W_col , int C) // Stride, Padding, Filter_size, Filter_num, input_size_row, input_size_col
{
    int resultsize_row; //result size
    int resultsize_col;

    int ***p1 = NULL; //image(padding)
    float ****f1 = NULL; //convolution filter
    int ***r1 = NULL; //convolution result

    resultsize_row = (W_row-F+2*P)/S + 1;
    resultsize_col = (W_col-F+2*P)/S + 1;

// input malloc
    p1 = (int***)malloc(C*sizeof(int**));
    for(int i=0; i<C; i++)
    {
        p1[i] = (int**)malloc((W_row+ 2*P)*sizeof(int*));
        for(int j=0; j<(W_row+2*P); j++)
        {
            p1[i][j] = (int*)malloc((W_col+2*P)*sizeof(int));
        }
    }

// input matrix init
    for(int i=0;i<W_row+2*P;i++)
	{
		for(int j=0;j<W_col+2*P;j++)
		{
            for(int k=0;k<C;k++)
            {
                p1[k][i][j] = 0;
            }
        }
    }

//filter malloc
    f1 = (float****)malloc(F_num*sizeof(float***));
    for(int k=0; k<F_num; k++)
    {
        f1[k] = (float***)malloc(C*sizeof(float**));
        for(int i=0; i<C; i++)
        {
            f1[k][i] = (float**)malloc(F*sizeof(float*));
            for(int j=0; j<F; j++)
            {
                f1[k][i][j] = (float*)malloc(F*sizeof(float));
            }
        }
    }

//filter init
    for(int n=0; n<F_num; n++)
    {
        for(int k=0; k<C; k++)
        {
            for(int i=0; i<F; i++)
            {
                for(int j=0; j<F; j++)
                {
                    f1[n][k][i][j] = 0;
                }
            }
        }
    }

//filter value


///////////////////////////////////////////////////////////////////////////////

//result matrix malloc
    r1 = (int***)malloc(C*sizeof(int**));
    for(int i=0; i<C; i++)
    {
        r1[i] = (int**)malloc(resultsize_row*sizeof(int*));
        for(int j=0; j<resultsize_row; j++)
        {
            r1[i][j] = (int*)malloc(resultsize_col*sizeof(int));
        }
    }
//result matrix init    
    for(int k=0;k<C;k++)
    {
        for(int i=0;i<resultsize_row;i++)
        {
            for(int j=0;j<resultsize_col;j++)
            {
                r1[k][i][j] = 0;
            }
        }
    }

//////////////image padding
    for(int x=0;x<W_row+2*P;x++)
	{
		for(int y=0;y<W_col+2*P;y++)
		{
            for(int z=0;z<C;z++)
            {
                if(x <= P-1 || y <= P-1 || x >= W_row+P || y>= W_col+P)
                {
                    p1[z][x][y] = 0;
                }
                else
                {
                    p1[z][x][y] = a[z][x-P][y-P]; //이미지 입력을 a벡터로 받아서 대입
                }
            }
		}
	}
//////////////convolution
    for(int n=0; n<F_num;n++)
   {
        for(int k=0;k<C;k++)
        {
            for(int x=0;x<resultsize_row;x++)
            {
                for(int y=0;y<resultsize_col;y++)
                {   
                    for(int i=0;i<F;i++)
                    {
                        for(int j=0;j<F;j++)
                        {
                            r1[k][x][y] += p1[k][i+x*S][j+y*S] * f1[n][k][i][j];
                        }
                    }
                    
                }
            }
        }
   }

//saturation
    for(int k=0;k<C;k++)
    {
        for(int i=0;i<resultsize_row;i++)
        {
            for(int j=0;j<resultsize_col;j++)
            {
                if((r1[k][i][j]) > 255)
                {
                    r1[k][i][j] = 255;
                }
                else if(r1[k][i][j] < 0)
                {
                    r1[k][i][j] = 0;
                }
                else
                {
                     r1[k][i][j] =  r1[k][i][j];
                }
                
            }
        }
    }

// input free
    for(int i=0; i<C;i++)
    {
        for(int j=0; j<W_row+2*P; j++)
        {
            free(*(*(p1+i)+j));
        }
        free(*(p1+i));
    }
    free(p1);
// filter free
    for(int n=0; n<F_num;n++)
    {
        for(int i=0; i<C;i++)
        {
            for(int j=0; j<F; j++)
            {
                free(*(*(*(f1+n)+i)+j));
            }
            free(*(*(f1+n)+i));
        }
        free(*(f1+n));
    }
    free(f1);
// result matrix free
    for(int i=0; i<C;i++)
    {
        for(int j=0; j<resultsize_row; j++)
        {
            free(*(*(r1+i)+j));
        }
        free(*(r1+i));
    }
    free(r1);
}


int main()
{
    conv(1,1,3,32,696,696,3);
}