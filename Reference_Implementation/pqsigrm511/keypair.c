#include "api.h"
#include "common.h"

void generateRandomInsertionMtx(matrix* R){
	
	unsigned char randombytesstream[SECRET_R_BYTES];
	
	randombytes(randombytesstream,SECRET_R_BYTES);
	importMatrix(R, randombytesstream);
}
int copy_columns(matrix *dest, matrix *src, uint16_t *lead ){
	size_t row, col;
	
	for(row=0; row <dest->rows; ++row)
		for(col=0; col < dest->cols; ++col)
			setElement(dest, row, col, getElement(src, row, lead[col]));

	return 0;
}

void export_sk(unsigned char *sk, matrix *Sinv, uint16_t *Q, uint16_t *Qp, uint16_t *s_lead){
	//export private in order: Sinv, R, Qinv
	exportMatrix(sk        			, Sinv);
	
	memcpy		(sk+Sinv->alloc_size, 
									Q, sizeof(uint16_t)*CODE_N);
	memcpy		(sk+Sinv->alloc_size+sizeof(uint16_t)*CODE_N, 
								   Qp, sizeof(uint16_t)*CODE_N/4);
	memcpy		(sk+Sinv->alloc_size+sizeof(uint16_t)*CODE_N+sizeof(uint16_t)*CODE_N/4, 
								   s_lead, sizeof(uint16_t)*(CODE_N-CODE_K));
}
void export_pk(unsigned char *pk, matrix *H_info, uint16_t *p_lead){
	exportMatrix(pk 			 , H_info);
	memcpy		(pk+H_info->alloc_size, 
						       p_lead,(sizeof(uint16_t)*(CODE_N-CODE_K)));
}
int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk){
	matrix *gen = newMatrix(CODE_K, CODE_N);

	uint16_t *Q = (uint16_t*)malloc(sizeof(uint16_t)*CODE_N);
	uint16_t *Qp = (uint16_t*)malloc(sizeof(uint16_t)*CODE_N/4);

	matrix *H = newMatrix(CODE_N-CODE_K, CODE_N);
	matrix *H_pub = newMatrix(CODE_N - CODE_K, CODE_N);
	matrix *H_info = newMatrix(CODE_N - CODE_K, CODE_K);

	matrix *Sinv = newMatrix(CODE_N - CODE_K, CODE_N - CODE_K);

	uint16_t *s_lead = (uint16_t*)malloc(sizeof(uint16_t)*(CODE_N-CODE_K));
	uint16_t *s_diff = (uint16_t*)malloc(sizeof(uint16_t)*CODE_K);

	uint16_t *p_lead = (uint16_t*)malloc(sizeof(uint16_t)*(CODE_N-CODE_K));
	uint16_t *p_diff = (uint16_t*)malloc(sizeof(uint16_t)*CODE_K);

	// generate secret permutations
	permutation(Qp, CODE_N/4);
	permutation(Q, CODE_N);
	// A partially permute generator matrix Gm
	rm_gen_mod(RM_R, RM_M, 0, CODE_K, 0, CODE_N, gen, Qp);

	// Parity check matrix of the modified RM code
	dual(gen, H, 0, 0);
	rref(H); getLeadingCoeff(H, s_lead, s_diff);
	uint16_t *Qinv = (uint16_t*)malloc(CODE_N*sizeof(uint16_t));
	for(int i =0; i< CODE_N; i++){
		Qinv[Q[i]] = i;
	}
	col_permute(H, 0, CODE_N-CODE_K, 0, CODE_N, Qinv);

	matrixcpy(H_pub, H); // copy of permuted matrix
	
	rref(H_pub); getLeadingCoeff(H_pub, p_lead, p_diff);

	// Generate a Scrambling matrix and its inverse. 
	// To Be Implemented Precisely
	
	copy_columns(Sinv, H, p_lead);
	//export public matrix and its H_leading set to pk
	copy_columns(H_info, H_pub, p_diff);

	export_sk(sk, Sinv, Q, Qp, s_lead);
	export_pk(pk, H_info, p_lead);

	deleteMatrix(gen);
	free(Q);free(Qp); free(Qinv);
	deleteMatrix(H); deleteMatrix(H_pub); deleteMatrix(H_info);
	deleteMatrix(Sinv);
	free(s_lead);free(s_diff); free(p_lead); free(p_diff);
	
	return 0;
}
