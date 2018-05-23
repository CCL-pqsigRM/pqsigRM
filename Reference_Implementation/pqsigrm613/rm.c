#include "rm.h"
#include "nearest_vector.h"

const uint16_t rm_dim[7][14]  
={{1,1,1,1,1,1,1,1,1,1,1,1,1,1}, 
  {0,2,3,4,5,6,7,8,9,10,11,12,13,14}, 
  {0,0,4,7,11,16,22,29,37,46,56,67,79,92}, 
  {0,0,0,8,15,26,42,64,93,130,176,232,299,378}, 
  {0,0,0,0,16,31,57,99,163,256,386,562,794,1093}, 
  {0,0,0,0,0,32,63,120,219,382,638,1024,1586,2380}, 
  {0,0,0,0,0,0,64,127,247,466,848,1486,2510,4096}};

matrix* rm_gen(int r, int m, uint16_t row_f, uint16_t row_r, uint16_t col_f, uint16_t col_r, matrix* gen) {
	if (r == 0) {
		for (int i = 0; i < (1<<m); i++) {
			setElement(gen, row_f, col_f + i, 1);
		}
	} else if (m == r) {
		for (int i = 0; i < (1<<m); i++) {
			 setElement(gen, row_f + i, col_f + i, 1);
		}
	} else {
		uint16_t col_m = (col_f+col_r)/2;
		rm_gen(r,   m-1, 	row_f, 					row_f + rm_dim[r][m-1], col_f, 	col_m ,	gen);
		rm_gen(r,   m-1, 	row_f, 					row_f + rm_dim[r][m-1], col_m, 	col_r, 	gen);
		rm_gen(r-1, m-1,  	row_f + rm_dim[r][m-1], row_r,  				col_m, 	col_r, 	gen);
	}
	return gen;
}

matrix* rm_gen_mod(int r, int m, uint16_t row_f, uint16_t row_r, uint16_t col_f, uint16_t col_r, matrix* gen, uint16_t *perm) {

	rm_gen(r, m, row_f, row_r, col_f, col_r, gen);
	col_permute(gen, rm_dim[RM_R][RM_M-1] + rm_dim[RM_R-1][RM_M-2], 
		rm_dim[RM_R][RM_M-1]+ rm_dim[RM_R-1][RM_M-2]+ rm_dim[RM_R-2][RM_M-3] 
		, CODE_N*6/8, CODE_N*7/8, perm); 
	col_permute(gen, rm_dim[RM_R][RM_M-1] + rm_dim[RM_R-1][RM_M-2], 
		rm_dim[RM_R][RM_M-1]+ rm_dim[RM_R-1][RM_M-2]+ rm_dim[RM_R-2][RM_M-3]
		, CODE_N*7/8, CODE_N	, perm);
	
	return gen;
}

int rm_dec_mod(matrix* r, matrix* cwd, uint16_t* perm){
	if(cwd == 0 || r == 0) return -1;
	if(cwd->cols != r->cols) return -1;

	float *y = (float*)malloc(r->cols*sizeof(float));

	for(int c =0; c< r->cols; c++)
		y[c] = (getElement(r, 0, c) == 0)? 1: -1;
	init_decoding(r->cols);

	recursive_decoding_mod(y, 6, 12, 0, r->cols, perm);
	
	for(int c = 0; c < r->cols; c++)
		setElement(cwd, 0, c, (y[c] < 0)? 1 : 0);

	free(y);
	return 0;	
}
