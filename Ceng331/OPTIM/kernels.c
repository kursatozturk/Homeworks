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
    "2171874",              /* Student ID */

    "Kürşat Öztürk",     /* full name */
    "kursatozturkcs@gmail.com",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * CONVOLUTION KERNEL
 ***************/

/******************************************************
 * Your different versions of the convolution functions  go here
 ******************************************************/

char naive_conv_descr[] = "naive_conv: first try";
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

char naive_conv_descr15[] = "vectorize kernel";
void naive_conv15(int dim,int *src, int *ker,int *dst) {
    int i, k,l;
    int jdim, ldim, jdimi, jldimi, val;
    int kernel_vector[64];
    for(k=0;k<8;k++)for(l=0;l<8;l++)kernel_vector[k*8+l]=ker[k*dim + l];
    int kernel_idx;
    int board_l = dim * 8;//it does shifting, it is faster than addition
    int bord_j = dim * dim - board_l + dim;
    for(jdim = 0; jdim < bord_j; jdim+=dim){ 
    jdimi = jdim;
    for(i = 0; i < dim-7; i++){
        val = 0;
        jldimi = jdimi; //jldimi = j * dim + l * dim + i
        kernel_idx = 0;
        for(ldim = 0; ldim < board_l; ldim+=dim){
            val = val +
                    src[jldimi]     * kernel_vector[kernel_idx]     + 
                    src[jldimi + 1] * kernel_vector[kernel_idx + 1] +
                    src[jldimi + 2] * kernel_vector[kernel_idx + 2] +
                    src[jldimi + 3] * kernel_vector[kernel_idx + 3] +
                    src[jldimi + 4] * kernel_vector[kernel_idx + 4] +
                    src[jldimi + 5] * kernel_vector[kernel_idx + 5] +
                    src[jldimi + 6] * kernel_vector[kernel_idx + 6] +
                    src[jldimi + 7] * kernel_vector[kernel_idx + 7];
                    kernel_idx += 8;
            jldimi += dim;
            }
        dst[jdimi] = val;
        jdimi += 1;
        }
    }
}

char naive_conv_descr2[] = "naive_conv: vectorize the kernel";
void naive_conv2(int dim,int *src, int *ker,int *dst) {
    int i,j,k,l;
    int kernel_vector[64];
    int source_vector[64];
    int kdim;
    int bord_j=dim*dim -8 * dim + dim;
    for(k=0;k<8;k++)for(l=0;l<8;l++)kernel_vector[k*8+l]=ker[k*dim + l];
    
    for(i = 0; i < dim-8+1; i++)
        for(j = 0; j < bord_j; j+=dim){
            kdim = i + j;
            for(k=0;k<64;k+=8){
                for(l=0;l<8;l++){
                    source_vector[k + l]=src[kdim + l];
                }
                kdim += dim;
            }
        dst[j + i] =    source_vector[0] * kernel_vector[0] + source_vector[8] * kernel_vector[8] +
                        source_vector[1] * kernel_vector[1] + source_vector[9] * kernel_vector[9] +
                        source_vector[2] * kernel_vector[2] + source_vector[10] * kernel_vector[10] +
                        source_vector[3] * kernel_vector[3] + source_vector[11] * kernel_vector[11] +
                        source_vector[4] * kernel_vector[4] + source_vector[12] * kernel_vector[12] +
                        source_vector[5] * kernel_vector[5] + source_vector[13] * kernel_vector[13] +
                        source_vector[6] * kernel_vector[6] + source_vector[14] * kernel_vector[14] +
                        source_vector[7] * kernel_vector[7] + source_vector[15] * kernel_vector[15] +
                        source_vector[16] * kernel_vector[16] + source_vector[44] * kernel_vector[44] +
                        source_vector[17] * kernel_vector[17] + source_vector[45] * kernel_vector[45] +
                        source_vector[18] * kernel_vector[18] + source_vector[46] * kernel_vector[46] +
                        source_vector[19] * kernel_vector[19] + source_vector[47] * kernel_vector[47] +
                        source_vector[20] * kernel_vector[20] + source_vector[48] * kernel_vector[48] +
                        source_vector[21] * kernel_vector[21] + source_vector[49] * kernel_vector[49] +
                        source_vector[22] * kernel_vector[22] + source_vector[50] * kernel_vector[50] +
                        source_vector[23] * kernel_vector[23] + source_vector[51] * kernel_vector[51] +
                        source_vector[24] * kernel_vector[24] + source_vector[52] * kernel_vector[52] +
                        source_vector[25] * kernel_vector[25] + source_vector[53] * kernel_vector[53] +
                        source_vector[26] * kernel_vector[26] + source_vector[54] * kernel_vector[54] +
                        source_vector[27] * kernel_vector[27] + source_vector[55] * kernel_vector[55] +
                        source_vector[28] * kernel_vector[28] + source_vector[56] * kernel_vector[56] +
                        source_vector[29] * kernel_vector[29] + source_vector[57] * kernel_vector[57] +
                        source_vector[30] * kernel_vector[30] + source_vector[58] * kernel_vector[58] +
                        source_vector[31] * kernel_vector[31] + source_vector[59] * kernel_vector[59] +
                        source_vector[32] * kernel_vector[32] + source_vector[60] * kernel_vector[60] +
                        source_vector[33] * kernel_vector[33] + source_vector[61] * kernel_vector[61] +
                        source_vector[34] * kernel_vector[34] + source_vector[62] * kernel_vector[62] +
                        source_vector[35] * kernel_vector[35] + source_vector[63] * kernel_vector[63] +
                        source_vector[36] * kernel_vector[36] + source_vector[37] * kernel_vector[37] + 
                        source_vector[38] * kernel_vector[38] + source_vector[39] * kernel_vector[39] + 
                        source_vector[40] * kernel_vector[40] + source_vector[41] * kernel_vector[41] + 
                        source_vector[42] * kernel_vector[42] + source_vector[43] * kernel_vector[43];
    
        }
            
        
}
/* 
 * naive_conv - The naive baseline version of convolution 
 */

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
    add_conv_function(&naive_conv15, naive_conv_descr15); 
    add_conv_function(&naive_conv2, naive_conv_descr2);   
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
char naive_matrix_multiplication_descr[] = "Transpose";
void naive_matrix_multiplication(int dim,int *src, int *src2,int *dst) {
    int i,j,k;
    int val, jk;
    int tidx, kdim;
    int *t = malloc(dim * sizeof(int)); //transpose 
    int j_b = dim * dim;
    int dim2=2*dim;
    int dim4=4*dim;
    int dim8=8*dim;
    int dim16=16*dim;
    int dim32=32*dim;
    for(i = 0; i < dim; i++){
        kdim=i;
        for(k=0;k<dim;k+=32){
            t[k] = src2[kdim]; // taking the transpose of ith column of src2
            t[k+1] = src2[kdim+dim]; // to achieve blocking, and using cache efficiently(incresing hit ratio in inner loop)
            t[k+2] = src2[kdim+dim2];
            t[k+3] = src2[kdim+dim2+dim];
            t[k+4] = src2[kdim+dim4];
            t[k+5] = src2[kdim+dim4+dim];
            t[k+6] = src2[kdim+dim4+dim2];
            t[k+7] = src2[kdim+dim8 - dim];
            t[k+8] = src2[kdim+dim8];
            t[k+9] = src2[kdim+dim8+dim];
            t[k+10] = src2[kdim+dim8+dim2];
            t[k+11] = src2[kdim+dim8+dim2+dim];
            t[k+12] = src2[kdim+dim8+dim4];
            t[k+13] = src2[kdim+dim8 +dim4+dim];
            t[k+14] = src2[kdim+dim16-dim2];
            t[k+15] = src2[kdim+dim16-dim];
            t[k+16] = src2[kdim+dim16];
            t[k+17] = src2[kdim+dim16+dim];
            t[k+18] = src2[kdim+dim16+dim2];
            t[k+19] = src2[kdim+dim16 +dim4 - dim];
            t[k+20] = src2[kdim+dim16+dim4];
            t[k+21] = src2[kdim+dim16+dim4+dim];
            t[k+22] = src2[kdim+dim16+dim4+dim2];
            t[k+23] = src2[kdim+dim16+dim8-dim];
            t[k+24] = src2[kdim+dim16+dim8];
            t[k+25] = src2[kdim+dim16 +dim8+dim];
            t[k+26] = src2[kdim+dim16+dim8+dim2];
            t[k+27] = src2[kdim+dim32-dim4-dim];
            t[k+28] = src2[kdim+dim32-dim4];
            t[k+29] = src2[kdim+dim32-dim4+dim];
            t[k+30] = src2[kdim+dim32-dim2];
            t[k+31] = src2[kdim+dim32 - dim];
            kdim += dim32;}
        for(j = 0; j < j_b; j+=dim) {
            val = 0;
            tidx = 0;
            for(jk = j; jk < j + dim; jk+=32){ 
                val = val + src[jk]*t[tidx] + 
                            src[jk + 1]*t[tidx + 1] + 
                            src[jk + 2]*t[tidx + 2] + 
                            src[jk + 3]*t[tidx + 3] + 
                            src[jk + 4]*t[tidx + 4] + 
                            src[jk + 5]*t[tidx + 5] + 
                            src[jk + 6]*t[tidx + 6] + 
                            src[jk + 7]*t[tidx + 7] + 
                            src[jk + 8]*t[tidx + 8] + 
                            src[jk + 9]*t[tidx + 9] + 
                            src[jk + 10]*t[tidx + 10] + 
                            src[jk + 11]*t[tidx + 11] + 
                            src[jk + 12]*t[tidx + 12] + 
                            src[jk + 13]*t[tidx + 13] + 
                            src[jk + 14]*t[tidx + 14] + 
                            src[jk + 15]*t[tidx + 15] + 
                            src[jk + 16]*t[tidx + 16] + 
                            src[jk + 17]*t[tidx + 17] + 
                            src[jk + 18]*t[tidx + 18] + 
                            src[jk + 19]*t[tidx + 19] + 
                            src[jk + 20]*t[tidx + 20] + 
                            src[jk + 21]*t[tidx + 21] + 
                            src[jk + 22]*t[tidx + 22] + 
                            src[jk + 23]*t[tidx + 23] + 
                            src[jk + 24]*t[tidx + 24] + 
                            src[jk + 25]*t[tidx + 25] + 
                            src[jk + 26]*t[tidx + 26] + 
                            src[jk + 27]*t[tidx + 27] + 
                            src[jk + 28]*t[tidx + 28] + 
                            src[jk + 29]*t[tidx + 29] + 
                            src[jk + 30]*t[tidx + 30] + 
                            src[jk + 31]*t[tidx + 31];
                tidx += 32;

            }
            dst[j + i] = val;
        }
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

