rapidhash - Very fast, high quality, platform-independent
====

The fastest recommended hash function by [SMHasher](https://github.com/rurban/smhasher?tab=readme-ov-file#summary). 

rapidhash is based on [wyhash](https://github.com/wangyi-fudan/wyhash), with improved speed, quality and compatibility.

**Fast**  
Extremely fast for both short and large inputs.  
The fastest hash function passing all tests in [SMHasher](https://github.com/rurban/smhasher?tab=readme-ov-file#smhasher). 

**Universal**  
Optimized for both AMD64 and modern AArch64 systems.  
Compatible with gcc, clang, icx and MSVC.  
It does not use machine-specific vectorized or cryptographic instruction sets.

**Excellent**  
Passes all tests in both [SMHasher](https://github.com/rurban/smhasher/blob/master/doc/rapidhash.txt) and SMHasher3.  
Outstanding collision ratio when tested with datasets of 16B and 66B keys: 

| Input Len | Nb Hashes | Expected | Nb Collisions | 
| --- | ---   | ---   | --- | 
| 12  | 15 Gi |   7.0 |   7 | 
| 16  | 15 Gi |   7.0 |  12 | 
| 24  | 15 Gi |   7.0 |   7 | 
| 32  | 15 Gi |   7.0 |  12 | 
| 48  | 15 Gi |   7.0 |   7 | 
| 64  | 15 Gi |   7.0 |   6 | 
| 96  | 15 Gi |   7.0 |   9 | 
| 128 | 15 Gi |   7.0 |  12 | 
| 192 | 15 Gi |   7.0 |   7 | 
| 256 | 15 Gi |   7.0 |   4 | 
| 384 | 15 Gi |   7.0 |   6 | 
| 12  | 62 Gi | 120.1 | 131 | 
| 16  | 62 Gi | 120.1 | 127 | 
| 24  | 62 Gi | 120.1 | 126 | 
| 32  | 62 Gi | 120.1 | 133 | 
| 48  | 62 Gi | 120.1 | 123 | 
