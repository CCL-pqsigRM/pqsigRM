//
// Code for RM code Generation, Decoding.
//

#ifndef __RM_H
#define __RM_H

#include "common.h"

extern const uint16_t rm_dim[7][13]; 

matrix *G;
uint16_t *lead;
uint16_t *lead_diff;
matrix *H_sys;
int p;

matrix* rm_augment(matrix * mtx, matrix * ntx); 
matrix* RMgen(int r, int m); 
matrix* rm_gen(int r, int m, uint16_t row_f, uint16_t row_r, uint16_t col_f, uint16_t col_r, matrix* gen);
matrix* rm_gen_mod(int r, int m, uint16_t row_f, uint16_t row_r, uint16_t col_f, uint16_t col_r, matrix* gen, uint16_t *perm) ;

int rm_dec_mod(matrix* r, matrix* cwd, uint16_t *perm);
int rm_dec(matrix* r, matrix* cwd);
void rm_init(int r, int m);
int getInsertedParityCheckMtx(matrix *H_m, matrix* R);

#endif
