rapidhash - Very fast, high quality, platform-independent
====

rapidhash is based on [wyhash](https://github.com/wangyi-fudan/wyhash), with improved speed, quality and compatibility.

**Fast**  
Extremely fast for both short and large inputs.

**Excellent**  
Passes all tests in both SMHasher and SMHasher3.  
Outstanding collision ratio when tested with datasets of 16B and 66B keys: 

| Algorithm | Input Len | Nb Hashes | Expected | Nb Collisions | 
| ---           | --- | ---   | ---   | --- | 
| __rapidhash__ | 16  | 15 Gi |   7.0 |  12 | 
| __rapidhash__ | 24  | 15 Gi |   7.0 |   7 | 
| __rapidhash__ | 32  | 15 Gi |   7.0 |  12 | 
| __rapidhash__ | 48  | 15 Gi |   7.0 |   7 | 
| __rapidhash__ | 64  | 15 Gi |   7.0 |   6 | 
| __rapidhash__ | 96  | 15 Gi |   7.0 |   9 | 
| __rapidhash__ | 128 | 15 Gi |   7.0 |  12 | 
| __rapidhash__ | 192 | 15 Gi |   7.0 |   7 | 
| __rapidhash__ | 256 | 15 Gi |   7.0 |   4 | 
| __rapidhash__ | 384 | 15 Gi |   7.0 |   6 | 
| __rapidhash__ | 12  | 62 Gi | 120.1 | 131 | 
| __rapidhash__ | 16  | 62 Gi | 120.1 | 127 | 
| __rapidhash__ | 24  | 62 Gi | 120.1 | 126 | 
| __rapidhash__ | 32  | 62 Gi | 120.1 | 133 | 
| __rapidhash__ | 48  | 62 Gi | 120.1 | 123 | 

**Universal**  
Optimized for both AMD64 and modern AArch64 systems.  
Compatible with gcc, clang and MSVC.  
It does not use machine-specific vectorized or cryptographic instruction sets.
