//
// Code for RM code Generation, Decoding.
//

#ifndef __RM_H
#define __RM_H

#include "common.h"

extern const uint16_t rm_dim[6][12]; 

matrix* rm_gen_mod(int r, int m, uint16_t row_f, uint16_t row_r, uint16_t col_f, uint16_t col_r, matrix* gen, uint16_t *perm) ;

#endif
