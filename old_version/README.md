rapidhashV1
====

**Fast**
Extremely fast for both short and large inputs.
The fastest hash function passing all tests in [SMHasher](https://github.com/rurban/smhasher?tab=readme-ov-file#smhasher).
The fastest hash function passing all tests in [SMHasher3](https://gitlab.com/fwojcik/smhasher3/-/bålob/main/results/README.md#passing-hashes).
About 6% higher throughput than wyhash according to SMHasher and SMHasher3 reports.

**Universal**
Optimized for both AMD64 and modern AArch64 systems.
Compatible with gcc, clang, icx and MSVC.
It does not use machine-specific vectorized or cryptographic instruction sets.
Prepared for both C and C++ compilation.

**Excellent**
Passes all tests in both [SMHasher](https://github.com/rurban/smhasher/blob/master/doc/rapidhash.txt) and [SMHasher3](https://gitlab.com/fwojcik/smhasher3/-/blob/main/results/raw/rapidhash.txt).
[Collision-based study](https://github.com/Nicoshev/rapidhash/tree/master?tab=readme-ov-file#collision-based-hash-quality-study) showed a collision probability lower than wyhash and close to ideal.
Outstanding collision ratio when tested with datasets of 16B and 66B keys:

| Input Len | Nb Hashes | Expected | Nb Collisions |
| --- | ---   | ---   | --- |
| 5   | 15 Gi |   7.0 |   8 |
| 6   | 15 Gi |   7.0 |   5 |
| 7   | 15 Gi |   7.0 |  11 |
| 8   | 15 Gi |   7.0 |   7 |
| 9   | 15 Gi |   7.0 |   6 |
| 10  | 15 Gi |   7.0 |   3 |
| 11  | 15 Gi |   7.0 |   9 |
| 12  | 15 Gi |   7.0 |   7 |
| 13  | 15 Gi |   7.0 |  10 |
| 14  | 15 Gi |   7.0 |   5 |
| 15  | 15 Gi |   7.0 |   3 |
| 16  | 15 Gi |   7.0 |  12 |
| 17  | 15 Gi |   7.0 |  10 |
| 18  | 15 Gi |   7.0 |   5 |
| 19  | 15 Gi |   7.0 |   8 |
| 20  | 15 Gi |   7.0 |  13 |
| 21  | 15 Gi |   7.0 |   5 |
| 22  | 15 Gi |   7.0 |   5 |
| 23  | 15 Gi |   7.0 |   8 |
| 24  | 15 Gi |   7.0 |   7 |
| 25  | 15 Gi |   7.0 |   8 |
| 26  | 15 Gi |   7.0 |   7 |
| 27  | 15 Gi |   7.0 |   5 |
| 28  | 15 Gi |   7.0 |   8 |
| 29  | 15 Gi |   7.0 |   2 |
| 30  | 15 Gi |   7.0 |   2 |
| 31  | 15 Gi |   7.0 |   6 |
| 32  | 15 Gi |   7.0 |  12 |
| 33  | 15 Gi |   7.0 |   4 |
| 34  | 15 Gi |   7.0 |  11 |
| 35  | 15 Gi |   7.0 |  10 |
| 36  | 15 Gi |   7.0 |  11 |
| 37  | 15 Gi |   7.0 |   4 |
| 38  | 15 Gi |   7.0 |   4 |
| 39  | 15 Gi |   7.0 |  12 |
| 40  | 15 Gi |   7.0 |   7 |
| 41  | 15 Gi |   7.0 |   9 |
| 42  | 15 Gi |   7.0 |   8 |
| 43  | 15 Gi |   7.0 |   7 |
| 44  | 15 Gi |   7.0 |  11 |
| 45  | 15 Gi |   7.0 |  10 |
| 46  | 15 Gi |   7.0 |   9 |
| 47  | 15 Gi |   7.0 |  10 |
| 48  | 15 Gi |   7.0 |   7 |
| 49  | 15 Gi |   7.0 |  11 |
| 50  | 15 Gi |   7.0 |  10 |
| 51  | 15 Gi |   7.0 |   6 |
| 52  | 15 Gi |   7.0 |  12 |
| 53  | 15 Gi |   7.0 |   8 |
| 54  | 15 Gi |   7.0 |   8 |
| 55  | 15 Gi |   7.0 |  13 |
| 56  | 15 Gi |   7.0 |  12 |
| 57  | 15 Gi |   7.0 |   4 |
| 58  | 15 Gi |   7.0 |   9 |
| 59  | 15 Gi |   7.0 |   4 |
| 60  | 15 Gi |   7.0 |   5 |
| 61  | 15 Gi |   7.0 |   9 |
| 62  | 15 Gi |   7.0 |   7 |
| 63  | 15 Gi |   7.0 |   9 |
| 64  | 15 Gi |   7.0 |   6 |
| 80  | 15 Gi |   7.0 |   9 |
| 96  | 15 Gi |   7.0 |   9 |
| 112 | 15 Gi |   7.0 |   5 |
| 128 | 15 Gi |   7.0 |  12 |
| 160 | 15 Gi |   7.0 |   9 |
| 192 | 15 Gi |   7.0 |   7 |
| 256 | 15 Gi |   7.0 |   4 |
| 384 | 15 Gi |   7.0 |   6 |
| 8   | 62 Gi | 120.1 |  99 |
| 12  | 62 Gi | 120.1 | 131 |
| 16  | 62 Gi | 120.1 | 127 |
| 24  | 62 Gi | 120.1 | 126 |
| 32  | 62 Gi | 120.1 | 133 |
| 40  | 62 Gi | 120.1 | 145 |
| 48  | 62 Gi | 120.1 | 123 |
| 56  | 62 Gi | 120.1 | 143 |
| 64  | 62 Gi | 120.1 | 192 |
| 80  | 62 Gi | 120.1 | 188 |
| 256 | 62 Gi | 120.1 | 181 |


Collision-based hash quality study
-------------------------

A perfect hash function distributes its domain uniformly onto the image.
When the domain's cardinality is a multiple of the image's cardinality, each potential output has the same probability of being produced.
A function producing 64-bit hashes should have a $p=1/2^{64}$ of generating each output.

If we compute $n$ hashes, the expected amount of collisions should be the number of unique input pairs times the probability of producing a given hash.
This should be $(n*(n-1))/2 * 1/2^{64}$, or simplified: $(n*(n-1))/2^{65}$.
In the case of hashing $15*2^{30}$ (~16.1B) different keys, we should expect to see $7.03$ collisions.

We present an experiment in which we use rapidhash to hash $68$ datasets of $15*2^{30}$ (15Gi) keys each.
For each dataset, the amount of collisions produced is recorded as measurement.
Ideally, the average among measurements should be $7.03$ and its histogram should approximate a binomial distribution.
We obtained a mean value of $7.72$, just $9.8$% over $7.03$.
The results histogram, depicted below, does resemble a slightly inclined binomial distribution:

![rapidhash, collisions, histogram](https://github.com/Nicoshev/rapidhash/assets/127915393/fc4c7c76-69b3-4d68-908b-f3e8723a32bb)

The same datasets were hashed using wyhash and its default seed $0$, yielding a higher mean collision value of $8.06$
The provided default seed was used to produce rapidhash results.

Ports
-------------------------
[Rust](https://github.com/hoxxep/rapidhash) by hoxxep
[Python](https://github.com/ryan-aoi/rapidhash) by ryan-aoi
[TypeScript](https://github.com/komiya-atsushi/rapidhash-js) by KOMIYA Atsushi
