// parm.h 
// 
// This header includes parameters for pqsigRM-5-11
// 
#ifndef __PARM_H
#define __PARM_H
	
#define RM_R 5
#define RM_M 11

#define CODE_K 1024
#define CODE_N 2048

#define WEIGHT_PUB 312 
#define HASHSIZEBYTES 64

#define PARM_P 129

#define PUBLIC_H_INFO_BYTES 131072
#define PUBLIC_LEADING_SET_BYTES 2048

#define SECRET_SINV_BYTES 131072 
#define SECRET_R_BYTES 2040 
#define SECTER_QINV_BYTES 4096 

#define ERRORSIZEBYTES 256 
#define SYNDROMESIZEBYTES 128 

#define H_BYTE_FILESIZE 262144 
#define LEAD_DIFF_FILESIZE 2048 

#define SIGN_FAILURE -1
#define VERIF_REJECT -1

#define ERROR_NO_FILE -1

#endif
