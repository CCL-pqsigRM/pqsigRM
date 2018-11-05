#include "rm.h"

const uint16_t rm_dim[6][12]  
={{1,1,1,1,1,1,1,1,1,1,1,1}, 
{0,2,3,4,5,6,7,8,9,10,11,12}, 
{0,0,4,7,11,16,22,29,37,46,56,67}, 
{0,0,0,8,15,26,42,64,93,130,176,232}, 
{0,0,0,0,16,31,57,99,163,256,386,562}, 
{0,0,0,0,0,32,63,120,219,382,638,1024}};

const unsigned char elem_block[4][8]
={
	{0xFF,},
	{0xaa, 0x55, 0x33, 0x0F},
	{0x88, 0x44, 0x22, 0x11, 0x0a, 0x05, 0x03},
	{0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}
};

matrix* rm_gen(int r, int m, uint16_t row_f, uint16_t row_r, uint16_t col_f, 
	uint16_t col_r, matrix* gen) {

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

		rm_gen(r,   m-1, 	row_f, 					row_f + rm_dim[r][m-1], 
			col_f, 	col_m ,	gen);
		rm_gen(r,   m-1, 	row_f, 					row_f + rm_dim[r][m-1], 
			col_m, 	col_r, 	gen);
		rm_gen(r-1, m-1,  	row_f + rm_dim[r][m-1], row_r,
			col_m, 	col_r, 	gen);
	}
	return gen;
}

matrix* rm_gen_opt(int r, int m, uint16_t row_f, uint16_t row_r, uint16_t col_f, 
	int col_r, matrix* gen) {
	if( m == 3){
		int idx = gen->rwdcnt*row_f + col_f/8;
		int row;
		for(row = 0; row < rm_dim[r][m]; ++row){
			idx += gen->rwdcnt*row;
			gen->elem[idx] = elem_block[r][row];
		}
	}
	else if (r == 0) {
		for (int i = 0; i < (1<<m); i++) {
			setElement(gen, row_f, col_f + i, 1);
		}
	} 
	else if (m == r) {
		for (int i = 0; i < (1<<m); i++) {
			setElement(gen, row_f + i, col_f + i, 1);
		}
	} 
	else {
		int col_m = (col_f+col_r)/2;
		int row;

		rm_gen(r,   m-1, row_f, row_f + rm_dim[r][m-1], 
			col_f, 	col_m ,	gen);

		for(row = row_f; row < row_f + rm_dim[r][m-1]; row++)
			memcpy(gen->elem + (gen->rwdcnt*row + col_m/8), 
				gen->elem + (gen->rwdcnt*row + col_f/8), 1<<(m-1));

		rm_gen(r-1, m-1,  	row_f + rm_dim[r][m-1], row_r,
			col_m, 	col_r, 	gen);
		
		
	}
	return gen;
}

matrix* rm_gen_mod(matrix* gen, uint16_t *part_perm1, uint16_t *part_perm2) {

	rm_gen(RM_R, RM_M, 0, CODE_K, 0, CODE_N, gen);

	int i =0;
	for (i = 0; i < 4; ++i)
	{
		col_permute(gen, 0, rm_dim[RM_R][RM_M -2], 
			i*(CODE_N/4),(i+1)*(CODE_N/4), part_perm1);
	}
	
	col_permute(gen, CODE_K - rm_dim[RM_R-2][RM_M-2], CODE_K, 
		3*CODE_N/4, CODE_N, part_perm2);
	
	return gen;
}
