#include <stdio.h>
#include "matrix.h"

int main(){
	matrix *m1 = newMatrix(9,9);
	matrix *vec = newMatrix(1,9);
	matrix *res = newMatrix(1,9);

	unsigned int bit;
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			scanf("%u", &bit);
			printf("%u ", bit);
			setElement(m1, i,j,bit);
		}putchar('\n');
	}

	for(int i=0; i<9; i++){
		scanf("%u", &bit);
		printf("%u", bit);
		setElement(vec, 0,i,bit);
	}

	printf("vec: %02x\n", vec->elem[0]);
	printf("vec: %02x\n", vec->elem[1]);
	
	vector_mtx_product(res, m1, vec);

	printf("res: %02x\n", res->elem[0]);
	
	printf("res: %02x\n", res->elem[1]);
	return 0;
}
