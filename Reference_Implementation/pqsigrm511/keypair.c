#include "api.h"
#include "common.h"

int copy_columns(matrix *dest, matrix *src, uint16_t *lead ){
	int row, col;
	
	for(row=0; row <dest->rows; ++row)
		for(col=0; col < dest->cols; ++col)
			setElement(dest, row, col, getElement(src, row, lead[col]));

	return 0;
}

void export_sk(unsigned char *sk, matrix *Sinv, uint16_t *Q, 
	uint16_t *part_perm1, uint16_t* part_perm2, uint16_t *s_lead){
	//export private in order: Sinv, Q, part_perm1, pert_perm2, pivot of H_M
	exportMatrix(sk        			, Sinv);
	
	memcpy		(sk+Sinv->alloc_size, 
									Q, sizeof(uint16_t)*CODE_N);
	memcpy		(sk+Sinv->alloc_size+sizeof(uint16_t)*CODE_N, 
								   part_perm1, sizeof(uint16_t)*CODE_N/4);

	memcpy		(sk+Sinv->alloc_size+sizeof(uint16_t)*CODE_N
		+sizeof(uint16_t)*CODE_N/4, 
								   part_perm2, sizeof(uint16_t)*(CODE_N-CODE_K));
	memcpy		(sk+Sinv->alloc_size+sizeof(uint16_t)*CODE_N
		+(sizeof(uint16_t)*CODE_N/4)*2, 
								   s_lead, sizeof(uint16_t)*(CODE_N-CODE_K));
}

void export_pk(unsigned char *pk, matrix *H_info, uint16_t *p_lead){
	exportMatrix(pk 			 , H_info);
	memcpy		(pk+H_info->alloc_size, 
						       p_lead,(sizeof(uint16_t)*(CODE_N-CODE_K)));
}

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk){
	matrix *G_M = newMatrix(CODE_K, CODE_N);

	uint16_t *part_perm1 = (uint16_t*)malloc(sizeof(uint16_t)*CODE_N/4);
	uint16_t *part_perm2 = (uint16_t*)malloc(sizeof(uint16_t)*CODE_N/4);
	///////////
	uint16_t *Q = (uint16_t*)malloc(sizeof(uint16_t)*CODE_N);
	matrix *H_M = newMatrix(CODE_N-CODE_K, CODE_N);
	matrix *H_pub = newMatrix(CODE_N - CODE_K, CODE_N);
	matrix *H_info = newMatrix(CODE_N - CODE_K, CODE_K);

	matrix *Sinv = newMatrix(CODE_N - CODE_K, CODE_N - CODE_K);

	uint16_t *s_lead = (uint16_t*)malloc(sizeof(uint16_t)*(CODE_N-CODE_K));
	uint16_t *s_diff = (uint16_t*)malloc(sizeof(uint16_t)*CODE_K);

	uint16_t *p_lead = (uint16_t*)malloc(sizeof(uint16_t)*(CODE_N-CODE_K));
	uint16_t *p_diff = (uint16_t*)malloc(sizeof(uint16_t)*CODE_K);

	// generate secret parital permutations
	partial_permutation_gen(part_perm1);
	partial_permutation_gen(part_perm2);
	/*for (int i = 0; i < CODE_N/4; ++i)
	{
		part_perm1[i] = i;
		part_perm2[i] = i;
	}*/
	// Generate a partially permute generator matrix G_M
	rm_gen_mod(G_M, part_perm1, part_perm2);
	
	
	// Parity check matrix of the modified RM code
	dual(G_M, H_M, 0, 0);
	rref(H_M); getLeadingCoeff(H_M, s_lead, s_diff);

/*	permutation_gen(Q, CODE_N);
*/	for (int i = 0; i < CODE_N; ++i)
	{
		Q[i] = i;
	}
	col_permute(H_M, 0, CODE_N-CODE_K, 0, CODE_N, Q);
	/*for (int i = 0; i < CODE_N; ++i)
	{
		printf("%d, ", Q[i]);
	}printf("\n");*/
	matrixcpy(H_pub, H_M); // copy of permuted matrix
	
	rref(H_pub); getLeadingCoeff(H_pub, p_lead, p_diff);

	// Generate a Scrambling matrix and its inverse. 
	// To Be Implemented Precisely
	
	copy_columns(Sinv, H_M, p_lead);
	//export public matrix and its H_leading set to pk
	copy_columns(H_info, H_pub, p_diff);

	export_sk(sk, Sinv, Q, part_perm1, part_perm2, s_lead);
	export_pk(pk, H_info, p_lead);

	deleteMatrix(G_M);
	free(Q);free(part_perm1); free(part_perm2);/*free(Qinv);*/
	deleteMatrix(H_M); deleteMatrix(H_pub); deleteMatrix(H_info);
	deleteMatrix(Sinv);
	free(s_lead);free(s_diff); 
	free(p_lead); free(p_diff);
	
	return 0;
}
