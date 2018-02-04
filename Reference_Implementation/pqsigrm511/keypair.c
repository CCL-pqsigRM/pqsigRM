#include "api.h"
#include "common.h"

int readParityCheck(matrix *H){
	
	unsigned char *hbytestream = (unsigned char*)malloc(H_BYTE_FILESIZE);
	FILE *patity_binary = fopen("parity_check.pqsigrm", "rb");
	
	if(patity_binary == NULL)	
		return ERROR_NO_FILE;
	fread(hbytestream, 1, H_BYTE_FILESIZE, patity_binary);

	fclose(patity_binary);

	importMatrix(H, hbytestream);
	free(hbytestream);
	return 0;
}

void getInsertedParityCheckMtx(matrix* H, matrix *R){
	size_t row, col;
	for(row=0; row < NUMOFPUNCTURE; ++row)
		for(col = 0; col < CODE_N - NUMOFPUNCTURE ;++col)
			setElement(H, (CODE_N - CODE_K - NUMOFPUNCTURE) + row, col
				, getElement(R, row, col));
}

int copy_columns(matrix *dest, matrix *src, uint16_t *lead ){
	size_t row, col;
	
	for(row=0; row <dest->rows; ++row)
		for(col=0; col < dest->cols; ++col)
			setElement(dest, row, col, getElement(src, row, lead[col]));

	return 0;
}

void generateRandomInsertionMtx(matrix* R){
	
	unsigned char randombytesstream[SECRET_R_BYTES];
	
	randombytes(randombytesstream,SECRET_R_BYTES);
	importMatrix(R, randombytesstream);
}

void swap(uint16_t* Q, uint16_t i, uint16_t j){
	uint16_t temp = Q[i];
	Q[i] = Q[j];
	Q[j] = temp;
}

void generatePermutation(uint16_t* Q, uint16_t *Qinv){
	uint16_t i;
	uint16_t* randomidxstream
		=malloc(sizeof(uint16_t)*CODE_N);
	
	for (i = 0; i < CODE_N; i++) {
		Q[i] = i;
		Qinv[i] = i;
	}

	// Generate Permutation by Knuth shuffles
	randombytes((unsigned char*)randomidxstream,sizeof(uint16_t)*CODE_N);
	uint16_t idx;
	for (i = 0; i < CODE_N; i++) {
		idx = randomidxstream[i]%CODE_N;
		swap(Q, i, idx);
	}

	// Set Qinv
	for (i = 0; i < CODE_N; i++) 
		Qinv[Q[i]] = i;
	
	free(randomidxstream);
}

int permutation(matrix* H, uint16_t* Q, matrix* H_pub){
	size_t col, row;
	
	for(col=0; col < H->cols; ++col)
		for(row=0; row < H->rows; ++row)
			setElement(H_pub, row, col, getElement(H, row, Q[col]));

	return 0;
}
unsigned char in(unsigned elem, unsigned *set, int size){
    for(int i=0; i< size; i++)
        if(set[i] == elem) return 1;
    return 0;
}

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk){
	matrix *H = newMatrix(CODE_N-CODE_K, CODE_N);
	matrix *Sinv = newMatrix(CODE_N - CODE_K, CODE_N - CODE_K);

	uint16_t *Q = (uint16_t*)malloc(CODE_N*sizeof(uint16_t));
	uint16_t *Qinv = (uint16_t*)malloc(CODE_N*sizeof(uint16_t));

	matrix *R = newMatrix(NUMOFPUNCTURE, CODE_N - NUMOFPUNCTURE);

	matrix *H_pub = newMatrix(CODE_N - CODE_K, CODE_N);

	uint16_t *H_lead = (uint16_t*)malloc(sizeof(uint16_t)*(CODE_N-CODE_K));
	uint16_t *H_lead_diff = (uint16_t*)malloc(sizeof(uint16_t)*CODE_K);

	// Get parity check matrix of punctured RM code with random insertion,
	readParityCheck(H);
	// do puncturing
	getLeadingCoeff(H, H_lead, H_lead_diff);
	unsigned int punc[NUMOFPUNCTURE];
	unsigned elem;
	unsigned i, j, k;
	for(i=0; i<NUMOFPUNCTURE; i++){
		do{
			 elem = randomunsigned(H->rows);
		}while(in(elem, punc, i));
		punc[i] = elem;
		printf("%u ", elem);
	}	
	// 	sort punc[]
	for( i=0 ; i<NUMOFPUNCTURE; i++)
		for(j=i; j < NUMOFPUNCTURE - i -1; j++)
			if(punc[i] < punc[i+1]){
				unsigned temp = punc[i];
				punc[i] = punc[i+1];
				punc[i+1] = temp;
			}
	// do bit flip
	unsigned flip;
	for(i=0; i<NUMOFPUNCTURE; i++){
		flip = randomunsigned(CODE_K);
		setElement(H, punc[i], H_lead_diff[flip], !getElement(H, punc[i], H_lead_diff[flip]));
		printf("%u ", flip);
	}
	// build R 
	unsigned char temp = 0;
	for(i=0; i< NUMOFPUNCTURE; i++){
		for(j=0 ; j < CODE_K; j++){
			setElement(R, i, H_lead_diff[j], getElement(H, punc[i], H_lead_diff[j]));
		}
	}
	// shift unpunctured rows
	// shift	
	unsigned pidx;
	for(pidx = 0; pidx < NUMOFPUNCTURE; pidx++){
		for(i=punc[pidx]; i < H->rows - 1; i++){
			for(j =  0; j < CODE_K; j++){
				setElement(H, i, H_lead_diff[j], getElement(H, i+1, H_lead_diff[j]));
			}
		}
	}

	getInsertedParityCheckMtx(H, R);

	// get Permutation
	generatePermutation(Q, Qinv);

	permutation(H, Q, H_pub);
	free(Q);

	matrixcpy(H, H_pub); // copy of scrambled matrix, for Sinv

	reducedEchelon(H_pub);

	getLeadingCoeff(H_pub, H_lead, H_lead_diff);

	// Generate a Scrambling matrix and its inverse.
	copy_columns(Sinv, H, H_lead);
	deleteMatrix(H);
	//export public matrix and its H_leading set to pk
	matrix *H_info = newMatrix(CODE_N - CODE_K, CODE_K);
	copy_columns(H_info, H_pub, H_lead_diff);
	free(H_lead_diff);
	deleteMatrix(H_pub);

	exportMatrix(pk 			 , H_info);
	memcpy(pk+PUBLIC_H_INFO_BYTES, (unsigned char*)H_lead , PUBLIC_LEADING_SET_BYTES);
	free(H_lead);
	deleteMatrix(H_info);

	//export private in order: Sinv, R, Qinv
	exportMatrix(sk        					  , Sinv);
	exportMatrix(sk+SECRET_SINV_BYTES		  , R);
	memcpy(sk+SECRET_SINV_BYTES+SECRET_R_BYTES, Qinv , SECTER_QINV_BYTES);

	//free matrices

	deleteMatrix(Sinv);
	deleteMatrix(R);
	free(Qinv);

	return 0;
}
