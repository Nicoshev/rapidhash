rapidhash - Very fast, high quality, platform-independent
====

rapidhash is based on [wyhash](https://github.com/wangyi-fudan/wyhash), with improved speed, quality and compatibility.

**Fast**:  
Extremely fast for both short and large inputs.

**Excellent**:  
Passes all tests in both SMHasher and SMHasher3.  
Outstanding collision ratio when tested with datasets of 16B and 66B keys (detailed results in 'collisions' folder).

**Universal**:  
Optimized for both AMD64 and modern AArch64 systems.  
Compatible with gcc, clang and MSVC.  
It does not use machine-specific vectorized or cryptographic instruction sets.
