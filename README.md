pqsigRM v1.11
========

This package includes supporting documents, reference implementation, optimized implementation, and KAT for pqsigRM.
This reference implementation and optimized implementation are the implementation of a post-quantum digital signature algorithm, pqsigRM.
The source code is tested under intel x64 cpu, ubuntu 16.04
There are three variants of pqsigRM.

You can find KAT(Known Answer Tests) [here](https://1drv.ms/f/s!AhBZmnuD2ssygtcf8G3izibzY4Ydmg).

Our website is <https://sites.google.com/view/pqsigrm>

# PRINCIPAL SUBMITTER
- Wijik Lee, Seoul National University
- Young-Sik Kim, Chosun University
- Yong-Woo Lee, Seoul National University
- Jong-Seon No, Seoul National University

E-mail address: <jsno@snu.ac.kr>

# SOFTWARE REQUIREMENTS

The source requires ANSI C compiler, and OpenSSL library for C.

# COMPILE
This source can be compiled with gcc
Put your own main file in directory pqsigrm**	

	$ make

# SIMULATION RESULTS

The following measurements are CPU cycles for run pqsigRM-4-12, pqsigRM-6-12, pqsigRM-6-13 at Intel(R) Xeon(R) CPU E5-2698 v4 2.20GHz. The measurements are given in Table below.

|              | security | key generation | singing    | verification |
|--------------|----------|----------------|------------|--------------|
| pqsigRM-4-12 | 128      | 14639777783    | 3971208456 | 139814898    |
| pqsigRM-6-12 | 196      | 6395769782     | 3275234719 | 198607502    |
| pqsigRM-6-13 | 256      | 72162115384    | 1087667252 | 956410761    |

# DOCUMENTATION


## pqsigRM-5-11
A digital signature algorithm with 128-bit security (Category 1)

## pqsigRM-6-12
A digital signature algorithm with 198-bit security (Category 3)

## pqsigRM-6-13
A digital signature algorithm with 256-bit security (Category 5)

* * *

*/Reference_Implementation* & */Optimized_Implementation*  
- */pqsigrm511*  
  - */api.h*					key sizes and algorithm name are defined here  
  - */keypair.c* 			crypto_sign_keypair() is defined here  
  - */sign.c* 				crypto_sign() is defined here  
  - */open.c* 				crypto_sign_open() is defined here  
  - */parm.h* 				basic parameters for pqsigRM- 5- 11  
  - */common.h* 				common variables and functions for signing and verifications   
  - */common.c*  
  - */matrix.h* 				basic matrix operations for pqsigRM.  
  - */matrix.c* 				modified mat.c and matrix.h in mceliece implementation in supercop- 20171020  
  - */nearest_vector.c* 		RM code decoder  
  - */nearest_vector.h*  
  - */parity_check.pqsigrm*	Information of reduced echelon form of the parity check matrix of RM(5, 11), not key, publicly known value  
  - */lead_diff.pqsigrm* 		difference set of the leading coefficient of the generator matrix of RM(5, 11) in reduced echelon form  
  - */rng.h* 					random number generator published by NIST	  
  - */rng.c*  
- */pqsigrm612*  
  - */api.h*					key sizes and algorithm name are defined here  
  - */keypair.c* 				crypto_sign_keypair() is defined here   
  - */sign.c* 				crypto_sign() is defined here  
  - */open.c* 				crypto_sign_open() is defined here  
  - */parm.h* 				basic parameters for pqsigRM-6-12  
  - */common.h* 				common variables and functions for signing and verifications   
  - */common.c*  
  - */matrix.h* 				basic matrix operations for pqsigRM  
  - */matrix.c* 				modified mat.c and matrix.h in mceliece implementation in supercop- 20171020  
  - */nearest_vector.c* 		RM code decoder  
  - */nearest_vector.h*  
  - */parity_check.pqsigrm* 	Information of reduced echelon form of the parity check matrix of RM(6,12), not key, publicly known value  
  - */lead_diff.pqsigrm* 		difference set of the leading coefficient of the generator matrix of RM(6,12) in reduced echelon form  
  - */rng.h* 					random number generator published by NIST	  
  - */rng.c*  
- */pqsigrm613*  
  - */api.h*					key sizes and algorithm name are defined here  
  - */keypair.c* 				crypto_sign_keypair() is defined here  
  - */sign.c* 				crypto_sign() is defined here
  - */open.c* 				crypto_sign_open() is defined here  
  - */parm.h* 				basic parameters for pqsigRM-6-13
  - */common.h* 				common variables and functions for signing and verifications   
  - */common.c*  
  - */matrix.h* 				basic matrix operations for pqsigRM  
  - */matrix.c* 				modified mat.c and matrix.h in mceliece implementation in supercop- 20171020  
  - */nearest_vector.c* 		RM code decoder  
  - */nearest_vector.h*  
  - */parity_check.pqsigrm* 	Information of reduced echelon form of the parity check matrix of RM(6,13), not key, publicly known value  
  - */lead_diff.pqsigrm* 		difference set of the leading coefficient of the generator matrix of RM(6,13) in reduced echelon form  
  - */rng.h* 					random number generator published by NIST	  
  - */rng.c*  
  
*/KAT*  
- */pqsigrm511* 				Known Answer Test for pqsigRM-5-11  
- */pqsigrm612* 				Known Answer Test for pqsigRM-6-12  
- */pqsigrm613* 				Known Answer Test for pqsigRM-6-13

*/Supporting_Documentation* 		Supporting documentation for pqsigRM

