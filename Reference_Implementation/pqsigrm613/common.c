// common.c for pqsigRM
#include "common.h"

unsigned char* hashMsg(unsigned char *s, const unsigned char *m, 
	unsigned long long mlen, unsigned long long sign_i){
	// Hash the given message
	// syndrome s = h(h(M)|i) | (h(h(M)|i)) | ...

	SHA512(m, mlen, s);
	*(unsigned long long*)(s+HASHSIZEBYTES) = sign_i;// concatenate i i.e. h(M)|i
	SHA512(s, HASHSIZEBYTES+sizeof(unsigned long long), s); //h(h(M)|i)
	
	size_t idx;
	for(idx=1; idx < 8; ++idx)
		SHA512(s+HASHSIZEBYTES*(idx-1), HASHSIZEBYTES, s+HASHSIZEBYTES*idx);//(h(h(M)|i))
	
	return s;
}

int hammingWgt(matrix* error){
	int wgt=0;
	int i=0;
	for(i=0; i < error->cols; i++)
		wgt += getElement(error, 0, i);
	return wgt;
}

void swap16(uint16_t *Q, const int i, const int j){
	uint16_t temp;
	temp = Q[i];
	Q[i] = Q[j];
	Q[j] = temp;
}

void permutation_gen(uint16_t *Q, int len){
	int i,j; 
	for(i=0; i<len; i++)
		Q[i] = i;
	for(i=0; i<len; i++)
		swap16(Q, i, random16(len));
}

int static compare(const void* first, const void* second){
	
	return (*(uint16_t*)first > *(uint16_t*)second)?1:-1;
}

void partial_permutation_gen(uint16_t* Q){
	permutation_gen(Q, CODE_N/4);
	uint16_t* partial_elem = (uint16_t*)malloc(sizeof(uint16_t)*PARM_P);
	uint16_t* partial_perm = (uint16_t*)malloc(sizeof(uint16_t)*PARM_P);
		

	memcpy(partial_perm, Q, sizeof(uint16_t)*PARM_P);
	memcpy(partial_elem, Q, sizeof(uint16_t)*PARM_P);

	qsort(partial_elem, PARM_P, sizeof(uint16_t), compare);
	qsort(Q, CODE_N/4, sizeof(uint16_t), compare);

	int i;
	for (i = 0; i < PARM_P; ++i)
	{
		Q[partial_elem[i]] = partial_perm[i];
	}

	free(partial_elem);free(partial_perm);
}




uint16_t random16(uint16_t n){
	uint16_t r;
	randombytes((unsigned char*)&r, 2);
	return r%n;
}

void col_permute(matrix* m, const int rf, const int rr, const int cf, 
	const int cr, uint16_t* Q)
{
	matrix* mcpy = newMatrix(m->rows, m->cols); 
	memcpy(mcpy->elem, m->elem, m->alloc_size);
	int r, c;
	for(c = cf; c < cr; c++)
		for(r = rf; r < rr; r++)
			setElement(m, r, c, getElement(mcpy, r, cf + Q[c-cf]));
	deleteMatrix(mcpy);
}



