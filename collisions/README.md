
__collisionsTest__ is a brute force hash analyzer
which will measure a 64-bit hash algorithm's collision rate
by generating billions of hashes,
and comparing the result to an "ideal" target.

The test requires a very large amount of memory.
By default, it will generate 24 billion of 64-bit hashes,
requiring __192 GB of RAM__ for their storage.
The number of hashes can be modified using command `--nbh=`.
Be aware that testing the collision ratio of 64-bit hashes
requires a very large amount of hashes (several billion) for meaningful measurements.

To reduce RAM usage, an optional filter can be requested, with `--filter`.
It reduces the nb of candidates to analyze, hence associated RAM budget.
Note that the filter itself requires a lot of RAM
(32 GB by default, can be modified using `--filterlog=`,
a too small filter will not be efficient, aim at ~2 bytes per hash),
and reading and writing into filter cost a significant CPU budget,
so this method is slower.
It also doesn't allow advanced analysis of partial bitfields,
since most hashes will be discarded and not stored.

When using the filter, the RAM budget consists of the filter and a list of candidates,
which will be a fraction of the original hash list.
Using default settings (24 billion hashes, 32 GB filter),
the number of potential candidates should be reduced to less than 2 billion,
requiring ~14 GB for their storage.
Such a result also depends on hash algorithm's efficiency.
The number of effective candidates is likely to be lower, at ~ 1 billion,
but storage must allocate an upper bound.

For the default test, the expected "optimal" collision rate for a 64-bit hash function is ~18 collisions.

#### How to build
```
make
```

Note: the code is a mix of C99 and C++14,
it's not compatible with a C90-only compiler.

#### Build modifier

- `SLAB5`: use alternative pattern generator, friendlier for weak hash algorithms
- `POOL_MT`: if `=0`, disable multi-threading code (enabled by default)

#### How to integrate any hash in the tester

The build script will compile files found in `./allcodecs`.
Put the source code here.
This also works if the hash is a single `*.h` file.

The glue happens in `hashes.h`.
In this file, there are 2 sections:
- Adds the required `#include "header.h"`, and creates a wrapper
to respect the format expected by the function pointer.
- Adds the wrapper, along with the name and an indication of the output width,
to the table, at the end of `hashes.h`

Build with `make`. Locate your new hash with `./collisionsTest -h`,
it should be listed.


#### Usage

```
usage: ./collisionsTest [hashName] [opt]

list of hashNames: (...)

Optional parameters:
  --nbh=NB       Select nb of hashes to generate (25769803776 by default)
  --filter       Enable the filter. Slower, but reduces memory usage for same nb of hashes.
  --threadlog=NB Use 2^NB threads
  --len=NB       Select length of input (255 bytes by default)
```

#### Some advises on how to setup a collisions test

Most tests are primarily driven by the amount of RAM available.
Here's a method to decide the size of the test.

Presuming that RAM budget is not plentiful, for this example 32 GB,
the `--filter` mode is actually compulsory to measure anything meaningful.
Let's plan 50% of memory for the filter, that's 16 GB.
This will be good enough to filter about 10% less hashes than this size.
Let's round down to 14 G.

By requesting 14G, the expectation is that the program will automatically
size the filter to 16 GB, and expect to store ~1G candidates,
leaving enough room to breeze for the system.

The command line becomes:
```
./collisionsTest --nbh=14G --filter NameOfHash
```

#### Examples:

Here are a few results produced with this tester:

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

