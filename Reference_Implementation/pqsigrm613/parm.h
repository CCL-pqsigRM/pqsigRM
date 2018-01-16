// parm.h 
// 
// This header includes parameters for pqsigRM-6-13
// 
#ifndef __PARM_H
#define __PARM_H
	
#define RM_R 6
#define RM_M 13

#define CODE_K 4096
#define CODE_N 8192

#define WEIGHT_PUB 1441
#define HASHSIZEBYTES 64

#define NUMOFPUNCTURE 16 

#define PUBLIC_H_INFO_BYTES 2097152
#define PUBLIC_LEADING_SET_BYTES 8192

#define SECRET_SINV_BYTES 2097152
#define SECRET_R_BYTES 16352 
#define SECTER_QINV_BYTES 16384

#define ERRORSIZEBYTES 1024
#define SYNDROMESIZEBYTES 512

#define H_BYTE_FILESIZE 4194304
#define LEAD_DIFF_FILESIZE 8192

#define SIGN_FAILURE -1
#define VERIF_REJECT -1

#define ERROR_NO_FILE -1

#endif
