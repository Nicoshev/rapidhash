rapidhash - Very fast, high quality, platform-independent
====

The fastest recommended hash function by [SMHasher](https://github.com/rurban/smhasher?tab=readme-ov-file#summary).

The fastest passing hash in [SMHasher3](https://gitlab.com/fwojcik/smhasher3/-/blob/main/results/README.md#passing-hashes).

rapidhash is [wyhash](https://github.com/wangyi-fudan/wyhash)' official successor, with improved speed, quality and compatibility.

**Fast**  
Extremely fast for both short and large inputs.  
Surpasses [70GB/s](https://github.com/Nicoshev/rapidhash/tree/master?tab=readme-ov-file#outstanding-performance) on Apple's M4 cpus.  
The fastest hash function passing all tests in [SMHasher](https://github.com/rurban/smhasher?tab=readme-ov-file#smhasher).  
The fastest hash function passing all tests in [SMHasher3](https://gitlab.com/fwojcik/smhasher3/-/blob/main/results/README.md#passing-hashes).

**Universal**  
Optimized for both AMD64 and AArch64 systems.  
Compatible with gcc, clang, icx and MSVC.  
It does not use machine-specific vectorized or cryptographic instruction sets.  
Prepared for both C and C++ compilation.

**Excellent**  
Passes all tests in both [SMHasher](https://github.com/rurban/smhasher/blob/master/doc/rapidhash.txt) and [SMHasher3](https://gitlab.com/fwojcik/smhasher3/-/blob/main/results/raw/rapidhash.txt).  
[Collision-based study](https://github.com/Nicoshev/rapidhash/tree/master?tab=readme-ov-file#collision-based-hash-quality-study) showed a collision probability lower than wyhash and close to ideal.  
Outstanding collision ratio when tested with datasets of 16B and 66B keys:

| Input Len | Nb Hashes | Expected | Nb Collisions |
| ---  | ---   | ---   | --- |
|    8 | 15 Gi |   7.0 |   3 |
|   12 | 15 Gi |   7.0 |   4 |
|   16 | 15 Gi |   7.0 |   8 |
|   24 | 15 Gi |   7.0 |   7 |
|   32 | 15 Gi |   7.0 |   3 |
|   40 | 15 Gi |   7.0 |   3 |
|   64 | 15 Gi |   7.0 |   8 |
|   80 | 15 Gi |   7.0 |   6 |
|  120 | 15 Gi |   7.0 |   5 |
|  128 | 15 Gi |   7.0 |   6 |
|  160 | 15 Gi |   7.0 |   9 |
| 1024 | 15 Gi |   7.0 |   7 |
| 4096 | 15 Gi |   7.0 |   8 |
|    8 | 62 Gi | 120.1 | 123 |
|   12 | 62 Gi | 120.1 | 108 |
|   16 | 62 Gi | 120.1 | 102 |
|   24 | 62 Gi | 120.1 | 112 |
|   32 | 62 Gi | 120.1 | 137 |
|   48 | 62 Gi | 120.1 | 149 |
|   64 | 62 Gi | 120.1 | 161 |
|  120 | 62 Gi | 120.1 | 172 |
|  128 | 62 Gi | 120.1 | 187 |
|  160 | 62 Gi | 120.1 | 183 |

More results can be found in the [collisions folder](https://github.com/Nicoshev/rapidhash/tree/master/collisions)

Outstanding performance
-------------------------

Average latency when hashing keys of 4, 8 and 16 bytes

| Hash      | M1 Pro | Core 5 226V | Ryzen 9700X | Neoverse V2 | AMD Turin |
| ---       | ---    | ---         | ---         | ---         | ---       |
| rapidhash | 1.75us |             |             | 1.73us      | 2.21us    |
| xxh3      | 1.90us |             |             | 1.99us      | 2.30us    |

Peak throughput when hashing files of 16Kb-2Mb

| Hash      | M1 Pro | M3 Pro | M3 Ultra | M4     | Neoverse V2 |
| ---       | ---    | ---    | ---      | ---    | ---         |
| rapidhash | 47GB/s | 57GB/s | 61GB/s   | 71GB/s | 38GB/s      |
| xxh3      | 37GB/s | 43GB/s | 47GB/s   | 49GB/s | 34GB/s      |


Folly's hash benchmark was used to measure short-input performance.  
Large-input speed benchmarking program can be found in the [bench folder](https://github.com/Nicoshev/rapidhash/tree/master/bench)

Collision-based hash quality study
-------------------------

A perfect hash function distributes its domain uniformly onto the image.  
When the domain's cardinality is a multiple of the image's cardinality, each potential output has the same probability of being produced.  
A function producing 64-bit hashes should have a $p=1/2^{64}$ of generating each output.

If we compute $n$ hashes, the expected amount of collisions should be the number of unique input pairs times the probability of producing a given hash.  
This should be $(n*(n-1))/2 * 1/2^{64}$, or simplified: $(n*(n-1))/2^{65}$.  
In the case of hashing $15*2^{30}$ (~16.1B) different keys, we should expect to see $7.03$ collisions.

We present an experiment in which we use rapidhash to hash $77$ datasets of $15*2^{30}$ (15Gi) keys each.  
For each dataset, the amount of collisions produced is recorded as measurement.  
Ideally, the average among measurements should be $7.031$ and the results collection should be a binomial distribution.  
We obtained a mean value of $8.026$, just $14$% over $7.031$.  
Each dataset individual result and the collisions test program can be found in the [collisions folder](https://github.com/Nicoshev/rapidhash/tree/master/collisions).  
The default seed $0$ was used in all experiments.
