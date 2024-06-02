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
| 40  | 15 Gi |   7.0 |   7 | 
| 48  | 15 Gi |   7.0 |   7 |
| 56  | 15 Gi |   7.0 |  12 | 
| 64  | 15 Gi |   7.0 |   6 | 
| 256 | 15 Gi |   7.0 |   4 | 
| 12  | 62 Gi | 120.1 | 131 | 
| 16  | 62 Gi | 120.1 | 127 | 
| 24  | 62 Gi | 120.1 | 126 | 
| 32  | 62 Gi | 120.1 | 133 |
| 40  | 62 Gi | 120.1 | 145 | 
| 48  | 62 Gi | 120.1 | 123 | 
| 56  | 62 Gi | 120.1 | 143 | 
| 64  | 62 Gi | 120.1 | 192 |
| 256 | 62 Gi | 120.1 | 181 | 

More results can be found in the [collisions folder](https://github.com/Nicoshev/rapidhash/tree/master/collisions)
