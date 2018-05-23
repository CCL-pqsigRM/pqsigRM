// parm.h 
// 
// This header includes parameters for pqsigRM-6-12
// 
#ifndef __PARM_H
#define __PARM_H
	
#define RM_R 6
#define RM_M 12

#define CODE_K 2510
#define CODE_N 4096

#define WEIGHT_PUB 480 
#define HASHSIZEBYTES 64

#define NUMOFPUNCTURE 8 

#define PUBLIC_H_INFO_BYTES ((CODE_N-CODE_K)*(CODE_N-CODE_K)/8)

#define SECRET_SINV_BYTES ((CODE_N-CODE_K)*(CODE_N-CODE_K)/8)
#define SECTER_QINV_BYTES (2*CODE_N)

#define ERRORSIZEBYTES (CODE_N/8)
#define SYNDROMESIZEBYTES 256

#define SIGN_FAILURE -1
#define VERIF_REJECT -1

#define ERROR_NO_FILE -1

#endif
