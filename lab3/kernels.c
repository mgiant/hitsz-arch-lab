/********************************************************
 * Performance Lab: kenels.c
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "李峥岑",     /* 姓名 */
    "SZ170110225",  /* 学号 */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
	        dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst)
{
	int i, j, k;
	k = dim;
	dst += (k - 1)*k;
	for (i = k; i > 0; i -= 16) {
		for (j = k; j > 0; j--) {
			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src += k;
			dst += 1;

			*dst = *src;
			src++;
			src -= (k << 4) - k;
			dst -= 15 + k;

		}
		dst += k * k;
		dst += 16;
		src += (k << 4) - k;
	}

}

/*********************************************************************
 * register_rotate_functions - 通过为每一个测试函数调用add_rotate_function(),
 * 登记你所有不同版本的旋转代码到评测程序driver中，
 * 当你运行driver程序时，它将测试并且给出每一个已经登记的测试函数的性能。
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++)
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
	int i, j, k, p1, p2;
	int mid_red, mid_green, mid_blue, up_red, up_green, up_blue, down_red, down_green, down_blue;
	p1 = dim - 1;
	p2 = dim + 1;

    // 整个图片第一个像素(角像素)
	(*dst).red = ((*src).red + (*(src + 1)).red + (*(src + dim)).red + (*(src + p2)).red) >> 2;
	(*dst).green = ((*src).green + (*(src + 1)).green + (*(src + dim)).green + (*(src + p2)).green) >> 2;
	(*dst).blue = ((*src).blue + (*(src + 1)).blue + (*(src + dim)).blue + (*(src + p2)).blue) >> 2;

    // 第一行像素
	for (j = 1; j < dim - 1; j++)
	{
		(*(dst + j)).red = ((*(src + j - 1)).red + (*(src + j)).red + (*(src + j + 1)).red + (*(src + j + p1)).red + (*(src + j + dim)).red + (*(src + j + p2)).red) / 6;
		(*(dst + j)).green = ((*(src + j - 1)).green + (*(src + j)).green + (*(src + j + 1)).green + (*(src + j + p1)).green + (*(src + j + dim)).green + (*(src + j + p2)).green) / 6;
		(*(dst + j)).blue = ((*(src + j - 1)).blue + (*(src + j)).blue + (*(src + j + 1)).blue + (*(src + j + p1)).blue + (*(src + j + dim)).blue + (*(src + j + p2)).blue) / 6;
	}

    // 第一行最后一个像素
	(*(dst + j)).red = ((*(src + j)).red + (*(src + j - 1)).red + (*(src + j + dim)).red + (*(src + j + p1)).red) >> 2;
	(*(dst + j)).green = ((*(src + j)).green + (*(src + j - 1)).green + (*(src + j + dim)).green + (*(src + j + p1)).green) >> 2;
	(*(dst + j)).blue = ((*(src + j)).blue + (*(src + j - 1)).blue + (*(src + j + dim)).blue + (*(src + j + p1)).blue) >> 2;

	k = dim;
	for (i = 1; i < dim - 1; i++)
	{
        // 第一列元素(每行第一个)
		mid_red = (*(src + k - dim)).red + (*(src + k)).red + (*(src + k + dim)).red;
		mid_green = (*(src + k - dim)).green + (*(src + k)).green + (*(src + k + dim)).green;
		mid_blue = (*(src + k - dim)).blue + (*(src + k)).blue + (*(src + k + dim)).blue;
		down_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
		down_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
		down_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
		(*(dst + k)).red = (mid_red + down_red) / 6;
		(*(dst + k)).green = (mid_green + down_green) / 6;
		(*(dst + k)).blue = (mid_blue + down_blue) / 6;
		k++;
		for (j = 1; j < dim - 6; j += 6)   
		{
			up_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			up_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			up_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			mid_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			mid_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			mid_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			down_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			down_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			down_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			up_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			up_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			up_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			mid_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			mid_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			mid_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			down_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			down_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			down_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
		}
		if (j == dim - 3)
		{
			up_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			up_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			up_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			mid_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			mid_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			mid_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			(*(dst + k)).red = (up_red + mid_red) / 6;
			(*(dst + k)).green = (up_green + mid_green) / 6;
			(*(dst + k)).blue = (up_blue + mid_blue) / 6;
			k++;
		}
		else if (j == dim - 5)
		{
			up_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			up_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			up_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			mid_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			mid_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			mid_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			down_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			down_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			down_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			up_red = (*(src + k - p1)).red + (*(src + k + 1)).red + (*(src + k + p2)).red;
			up_green = (*(src + k - p1)).green + (*(src + k + 1)).green + (*(src + k + p2)).green;
			up_blue = (*(src + k - p1)).blue + (*(src + k + 1)).blue + (*(src + k + p2)).blue;
			(*(dst + k)).red = (up_red + mid_red + down_red) / 9;
			(*(dst + k)).green = (up_green + mid_green + down_green) / 9;
			(*(dst + k)).blue = (up_blue + mid_blue + down_blue) / 9;
			k++;
			(*(dst + k)).red = (up_red + down_red) / 6;
			(*(dst + k)).green = (up_green + down_green) / 6;
			(*(dst + k)).blue = (up_blue + down_blue) / 6;
			k++;
		}
		else
		{
			(*(dst + k)).red = (mid_red + down_red) / 6;
			(*(dst + k)).green = (mid_green + down_green) / 6;
			(*(dst + k)).blue = (mid_blue + down_blue) / 6;
			k++;
		}
	}
    
    // 最后一行第一个像素
	(*(dst + k)).red = ((*(src + k)).red + (*(src + k + 1)).red + (*(src + k - dim)).red + (*(src + k - p1)).red) >> 2;
	(*(dst + k)).green = ((*(src + k)).green + (*(src + k + 1)).green + (*(src + k - dim)).green + (*(src + k - p1)).green) >> 2;
	(*(dst + k)).blue = ((*(src + k)).blue + (*(src + k + 1)).blue + (*(src + k - dim)).blue + (*(src + k - p1)).blue) >> 2;
	k++;

    // 最后一行
	for (j = 1; j < dim - 1; j++)
	{
		(*(dst + k)).red = ((*(src + k - 1)).red + (*(src + k)).red + (*(src + k + 1)).red + (*(src + k - p2)).red + (*(src + k - dim)).red + (*(src + k - p1)).red) / 6;
		(*(dst + k)).green = ((*(src + k - 1)).green + (*(src + k)).green + (*(src + k + 1)).green + (*(src + k - p2)).green + (*(src + k - dim)).green + (*(src + k - p1)).green) / 6;
		(*(dst + k)).blue = ((*(src + k - 1)).blue + (*(src + k)).blue + (*(src + k + 1)).blue + (*(src + k - p2)).blue + (*(src + k - dim)).blue + (*(src + k - p1)).blue) / 6;
		k++;
	}

    // 最后一行最后一个像素
	(*(dst + k)).red = ((*(src + k - 1)).red + (*(src + k)).red + (*(src + k - p2)).red + (*(src + k - dim)).red) >> 2;
	(*(dst + k)).green = ((*(src + k - 1)).green + (*(src + k)).green + (*(src + k - p2)).green + (*(src + k - dim)).green) >> 2;
	(*(dst + k)).blue = ((*(src + k - 1)).blue + (*(src + k)).blue + (*(src + k - p2)).blue + (*(src + k - dim)).blue) >> 2;
}


/********************************************************************* 
 * register_smooth_functions - 通过为每一个测试函数调用add_smooth_funtion(),
 * 登记所有不同版本的smooth代码到评测程序driver中。
 * 当你运行driver程序时，它将测试并且给出每一个已经登记的测试函数的性能。
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

