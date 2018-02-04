//
// Common functions for pqsigRM
// 
#ifndef __PQSIGRM_COMMON_H
#define __PQSIGRM_COMMON_H

#include "matrix.h"
#include "rng.h"
#include "parm.h"
#include <openssl/sha.h>
#include <string.h>

unsigned char* hashMsg(unsigned char *s, const unsigned char *m, 
	unsigned long long mlen, unsigned long long i);

int hammingWgt(matrix* e);

unsigned randomunsigned(int mod);
#endif 
