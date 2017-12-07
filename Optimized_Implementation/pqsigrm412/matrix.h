#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MATRIX_NULL 0 
#define ELEMBLOCKSIZE 8

#define getElement(A, i, j) 		(!!((A)->elem[(i) * A->rwdcnt + (j) / ELEMBLOCKSIZE] & (0x80 >> (j % ELEMBLOCKSIZE))))
#define flipElement(A, i, j) 	((A)->elem[(i) * A->rwdcnt + (j) / ELEMBLOCKSIZE] ^= (0x80 >> ((j) % ELEMBLOCKSIZE)))
#define setElement(A, i, j, val) 	((getElement(A, i, j) == val)? 0 : flipElement(A, i,j))
#define initZero(R) 			memset((R)->elem,0,(R)->alloc_size)

typedef struct {
   int rows;//number of rows.
   int cols;//number of columns.
   int rwdcnt;//number of words in a row
   int alloc_size;//number of allocated bytes
   unsigned char *elem;//row index.
} matrix;

matrix* newMatrix(int rows, int cols) ;
void deleteMatrix(matrix * mtx) ;

matrix* reducedEchelon(matrix* A);
matrix* transpose(matrix *dest, matrix *src);

void getLeadingCoeff(matrix* mtx, uint16_t *lead, uint16_t *lead_diff);

matrix* matrixcpy(matrix* dest, matrix* src);

int product(matrix * mtx1, matrix * mtx2, matrix * prod); 

int exportMatrix(unsigned char* dest, matrix* mtx);
matrix* importMatrix(matrix* dest_mtx, const unsigned char* src);

#endif
