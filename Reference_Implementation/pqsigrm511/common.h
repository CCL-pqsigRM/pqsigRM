//
// Common functions for pqsigRM
// 
#ifndef __PQSIGRM_COMMON_H
#define __PQSIGRM_COMMON_H

#include "matrix.h"
#include "rng.h"
#include "parm.h"
#include "rm.h"
#include <openssl/sha.h>
#include <string.h>

unsigned char* hashMsg(unsigned char *s, const unsigned char *m, 
	unsigned long long mlen, unsigned long long i);

int hammingWgt(matrix* e);

void swap(uint16_t *Q, const int i, const int j);

void permutation(uint16_t *Q, int len);
uint16_t random16(uint16_t n);

void col_permute(matrix* G, const int rf, const int rr, const int cf, const int cr, uint16_t* Q);


#endif 
