#ifndef __NEARESR_VECTOR
#define __NEARESR_VECTOR

#include <stdlib.h>
#include <math.h>
#include "common.h"

// void rm_dec(float* y);
void init_decoding(int );
void recursive_decoding(float* y, int r1, int m1, int f, int l); 
void recursive_decoding_mod(float* y, int r1, int m1, int f, int l, uint16_t *perm); 

#endif
