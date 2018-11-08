pqsigRM v1.11
========

This package includes supporting documents, reference implementation, optimized implementation, and KAT for pqsigRM.
This reference implementation and optimized implementation are the implementation of a post-quantum digital signature algorithm, pqsigRM.
The source code is tested under x64 cpu, ubuntu 16.04
There are three variants of pqsigRM.

You can find KAT(Known Answer Tests) [here](https://www.google.com/url?q=https%3A%2F%2Fmysnu-my.sharepoint.com%2F%3Af%3A%2Fg%2Fpersonal%2Fyongwool_seoul_ac_kr%2FEtLLefDJab5MqZadPydyvMwB55L13vKODHUPD8pS3UCm8Q&sa=D&sntz=1&usg=AFQjCNGFSprr7bPDZ6bDUQjbA2M9ZxrhQw).

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

# PERFORMANCE ANALYSIS 

The following measurements are CPU cycles for run pqsigRM-5-11, pqsigRM-6-12, pqsigRM-6-13 at AMD Ryzen Threadripper 1950X 3.4GHz. The measurements are given in Table below.

|              | security  | key generation | singing    | verification |
|--------------|-----------|----------------|------------|--------------|
| pqsigRM-5-11 | category 1| 36939368279	| 11397250   | 2235511 	    |
| pqsigRM-6-12 | category 3| 174485898177   | 15066562   | 6825856      |
| pqsigRM-6-13 | category 5| 2406221215789  | 102879688  | 36915068     |

# DOCUMENTATION


## pqsigRM-5-11
A digital signature algorithm with 128-bit security (Category 1)

## pqsigRM-6-12
A digital signature algorithm with 198-bit security (Category 3)

## pqsigRM-6-13
A digital signature algorithm with 256-bit security (Category 5)

* * *

