#include "nearest_vector.h"

float *temp;

void reculsive_decoding(float* y, int r1, int m1, int f, int l) {
	if (r1 == 0) {
		//Calculate Euclidean distance
		int a1 = 0;
		int a2 = 0;

		for (int i = f; i < l; i++) {
			a1 = a1 + pow(y[i] - 1, 2);
			a2 = a2 + pow(y[i] + 1, 2);
		}
		if (a1 <= a2) {
			for (int i = f; i < l; i++) {
				y[i] = 1;
			}
		}
		else if (a1 > a2) {
			for (int i = f; i < l; i++) {
				y[i] = -1;
			}
		}
		return;
	}
	
	if (r1 == m1) {
		for (int i = f; i < l; i++) {
			if (y[i] >= 0) { y[i] = 1; }
			else if (y[i] < 0) { y[i] = -1; }
		}
		return;
	}
	
	
	for (int i = 0; i < (l - f) / 2; i++) {
		temp[f + i] = y[i + (l + f) / 2];
	}

	for (int i = 0; i < (l - f) / 2; i++) {
		y[i + (l + f) / 2] = y[i + (l + f) / 2] * y[i + f];
	}

	reculsive_decoding(y, r1 - 1, m1 - 1, (l + f) / 2, l);

	for (int i = 0; i < (l - f) / 2; i++) {
		y[f + i] = (y[f + i] + y[i + (l + f) / 2] * temp[f + i]) / 2;
	}

	reculsive_decoding(y, r1, m1 - 1, f, (l + f) / 2);

	for (int i = 0; i < (l - f) / 2; i++) {
		y[i + (l + f) / 2] = y[i + (l + f) / 2] * y[i + f];
	}

}

void nearest_vector(float* y){
	//
	// For the detailed description, see
	// I. Dumer, “Recursive Decoding and Its Performance for Low-Rate 
	// Reed-Muller Codes,” IEEE Trans. Inform. Theory, may 2004.
	// 
	temp = (float*)malloc(CODE_N * sizeof(float));
	reculsive_decoding(y, RM_R, RM_M, 0, CODE_N);
	free(temp);
}
