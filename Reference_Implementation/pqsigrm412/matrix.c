#include "matrix.h"

matrix* newMatrix (int rows, int cols)
{
  matrix* A;

  A = (matrix*) malloc (sizeof (matrix));
  A->cols = cols;
  A->rows = rows;  
  A->rwdcnt = (1 + (cols - 1) / ELEMBLOCKSIZE);
  A->alloc_size = rows * A->rwdcnt * sizeof (unsigned char);
  A->elem = (unsigned char *)malloc(A->alloc_size);
  initZero(A);
  return A;
}

void deleteMatrix(matrix* A)
{
  free(A->elem);
  free(A);
}

int product(matrix * mtx1, matrix * mtx2, matrix * prod) {
	int row, col, k;
	int val;
	for (col = 0; col < mtx2->cols; col++)
		for (row = 0; row < mtx1->rows; row++) {
			val = 0;
			for (k = 0; k < mtx1->cols; k++)
				val ^= getElement(mtx1, row, k) & getElement(mtx2, k, col);
			setElement(prod, row, col, val);
		}
	return 0;
}

//assume vector is transposed
void vector_mtx_product(matrix *dest, matrix* m, matrix *vec){
	unsigned char bit = 0;
	unsigned char offset;
	int row, col;
	for(row = 0; row < m->rows; row++){
		bit = 0;
		for(col=0; col < m->rwdcnt - 1; col++)
			bit ^= m->elem[row*m->rwdcnt + col] & vec->elem[col];
	
		offset = 0xff << (ELEMBLOCKSIZE*m->rwdcnt - m->cols);
		bit ^= (m->elem[row*m->rwdcnt + col] & vec->elem[col])&offset;

		bit ^= (bit >> 4);
		bit ^= (bit >> 2);
		bit ^= (bit >> 1);
		bit &= (unsigned char)1;
		
		setElement(dest, 0, row, bit);
	}
}

void rowInterchanging(matrix* A, int row_idx1, int row_idx2){
	int col_idx;
	unsigned char temp;
	for(col_idx=0; col_idx<A->rwdcnt; ++col_idx){
		temp 	 								= A->elem[row_idx1 * A->rwdcnt + col_idx];
		A->elem[row_idx1 * A->rwdcnt + col_idx] = A->elem[row_idx2 * A->rwdcnt + col_idx];
		A->elem[row_idx2 * A->rwdcnt + col_idx] = temp;
	}
}

void rowAddition(matrix* A, int dest_row_idx, int adding_row_idx){
	int col_idx;
	for(col_idx=0; col_idx<A->rwdcnt; ++col_idx){
		A->elem[dest_row_idx * A->rwdcnt + col_idx] 
			^= A->elem[adding_row_idx * A->rwdcnt + col_idx];
	}
}

matrix * reducedEchelon(matrix* A)
{
	// Considering column is longer than row
	int succ_row_idx=0;
	int col_idx, row_idx=0;
	int i;
	for (col_idx = 0; col_idx < (A->cols); ++col_idx) {
		
		// finding first row s.t. i th elem of the row is 1
		for(; row_idx < A->rows; ++row_idx)
			if(getElement(A, row_idx, col_idx) == 1) 
				break;
		// When reaches the last row,
		// increase column index and search again
		if (row_idx == A->rows){ 
			row_idx=succ_row_idx;
			continue;
		}
		// if row_idx is not succ_row_idx, 
		// interchange between:
		// <succ_row_idx> th row <-> <row_idx> th row
		if(row_idx != succ_row_idx){
			rowInterchanging(A, succ_row_idx, row_idx);
		}
				
		// By adding <succ_row_idx> th row in the other rows 
		// s.t. A(i, <succ_row_idx>) == 1,
		// making previous columns as element row.
		for(i=0; i<A->rows; ++i){
			if(i == succ_row_idx) continue;

			if(getElement(A, i, col_idx) == 1){
				rowAddition(A, i, succ_row_idx);
			}
		}
		row_idx = ++succ_row_idx;
	}
	//Gaussian elimination is finished. So return A.
	return A;
}

matrix* matrixcpy(matrix* dest, matrix* src){
	if(dest->rows != src->rows || dest->cols!=src->cols) return MATRIX_NULL;
	
	memcpy(dest->elem, src->elem, dest->alloc_size);
	return dest;
}

matrix* transpose(matrix *dest, matrix *src){
	if((dest->rows != src->cols) || (dest->cols != src->rows))
		return MATRIX_NULL;
	size_t row, col;
	for(row=0; row < dest->rows; ++row)
		for(col=0; col < dest->cols; ++col)
			setElement(dest, row, col, getElement(src, col, row));
	return dest;
}

// Exports a matrix into unsigned char destination.
int exportMatrix(unsigned char* dest, matrix* src_mtx){
	memcpy(dest, src_mtx->elem, src_mtx->alloc_size);
	return src_mtx->alloc_size;
}

matrix* importMatrix(matrix* dest_mtx, const unsigned char* src){
	memcpy(dest_mtx->elem, src, dest_mtx->alloc_size);

	return dest_mtx;
}


void getLeadingCoeff(matrix* mtx, uint16_t *lead, uint16_t *lead_diff){
	int row=0, col;
	int lead_idx=0, diff_idx=0;

	for(col=0; col<mtx->cols; col++){
		if(getElement(mtx, row, col) == 0) {
			lead_diff[diff_idx++] = col;
		}
		else {
			lead[lead_idx++] = col; 
			row++;
		}
		if(row == mtx->rows){
			while(col < mtx->cols-1){
				lead_diff[diff_idx++] = ++col;
			}
			break;
		}
	}
}
