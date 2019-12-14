/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/* 
 * Please fill in the following student struct 
 */
team_t team = {
    "",              /* Student ID */

    "",     /* full name */
    "",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * CONVOLUTION KERNEL
 ***************/

/******************************************************
 * Your different versions of the convolution functions  go here
 ******************************************************/

/* 
 * naive_conv - The naive baseline version of convolution 
 */
char naive_conv_descr[] = "naive_conv: Naive baseline implementation 4.6-4.9";
void naive_conv(int dim,int *src, int *ker,int *dst) {
    int i;
    int jdim, ldim, jdimi, jldimi, val;

    int board_l = dim * 8;//it does shifting, it is faster than addition
    int bord_j = dim * dim - board_l + dim;
    for(jdim = 0; jdim < bord_j; jdim+=dim){ 
    jdimi = jdim;
    for(i = 0; i < dim-7; i++){
        val = 0;
        jldimi = jdimi; //jldimi = j * dim + l * dim + i
        for(ldim = 0; ldim < board_l; ldim+=dim){
            val = val +
                    src[jldimi]     * ker[ldim]     + 
                    src[jldimi + 1] * ker[ldim + 1] +
                    src[jldimi + 2] * ker[ldim + 2] +
                    src[jldimi + 3] * ker[ldim + 3] +
                    src[jldimi + 4] * ker[ldim + 4] +
                    src[jldimi + 5] * ker[ldim + 5] +
                    src[jldimi + 6] * ker[ldim + 6] +
                    src[jldimi + 7] * ker[ldim + 7];
            jldimi += dim;
            }
        dst[jdimi] = val;
        jdimi += 1;
        }
    }
}


/* 
 * convolution - Your current working version of convolution
 * IMPORTANT: This is the version you will be graded on
 */
char convolution_descr[] = "Dot product: Current working version";
void convolution(int dim,int *src, int *ker,int *dst) 
{

        naive_conv(dim,src,ker,dst);

}

/*********************************************************************
 * register_conv_functions - Register all of your different versions
 *     of the convolution functions  with the driver by calling the
 *     add_conv_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_conv_functions() {
    add_conv_function(&naive_conv, naive_conv_descr);   
    add_conv_function(&convolution, convolution_descr);   
    /* ... Register additional test functions here */
}




/***************
 * MATRIX MULTIP KERNEL
 ***************/

/******************************************************
 * Your different versions of the matrix multiplications  go here
 ******************************************************/

/* 
 * naive_matrix_multiplication - The naive baseline version of matrix multiplication 
 */
char naive_matrix_multiplication_descr[] = "Naive_matrix_multiplication: Naive baseline implementation";
void naive_matrix_multiplication(int dim,int *src, int *src2,int *dst) {
    int i,j,k;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) {
            dst[j*dim+i]=0;
            for(k = 0; k < dim; k++) 
                dst[j*dim+i] = dst[j*dim+i] + src[j*dim+k]*src2[i+k*dim];
        }    
}


/* 
 * matrix_multiplication - Your current working version of matrix_multiplication
 * IMPORTANT: This is the version you will be graded on
 */
char matrix_multiplication_descr[] = "Matrix multiplications: Current working version";
void matrix_multiplication(int dim,int *src, int *src2,int *dst) 
{

        naive_matrix_multiplication(dim,src,src2,dst);

}

/*********************************************************************
 * register_matrix_multiplication_functions - Register all of your different versions
 *     of the matrix multiplication  with the driver by calling the
 *     add_matrix_multiplication_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_matrix_multiplication_functions() {
    add_matrix_multiplication_function(&naive_matrix_multiplication, naive_matrix_multiplication_descr);   
    add_matrix_multiplication_function(&matrix_multiplication, matrix_multiplication_descr);   
    /* ... Register additional test functions here */
}

