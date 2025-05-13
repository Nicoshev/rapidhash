
__collisionsTest__ is a brute force hash analyzer originally published within [xxHash](https://github.com/Cyan4973/xxHash/tree/dev/tests/collisions).
It will measure a 64-bit hash algorithm's collision rate by generating billions of hashes and counting collisions.

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

#### Measurements

Here are some results for rapidhash, using $0$ as seed:

| Input Len | Nb Hashes | Expected | Nb Collisions |
| ---  | ---   | ---   | --- |
|    5 | 15 Gi |   7.0 |   7 |
|    6 | 15 Gi |   7.0 |   8 |
|    7 | 15 Gi |   7.0 |   2 |
|    8 | 15 Gi |   7.0 |   3 |
|    9 | 15 Gi |   7.0 |   7 |
|   10 | 15 Gi |   7.0 |   8 |
|   11 | 15 Gi |   7.0 |   8 |
|   12 | 15 Gi |   7.0 |   4 |
|   13 | 15 Gi |   7.0 |   7 |
|   14 | 15 Gi |   7.0 |   6 |
|   15 | 15 Gi |   7.0 |   8 |
|   16 | 15 Gi |   7.0 |   8 |
|   17 | 15 Gi |   7.0 |  11 |
|   18 | 15 Gi |   7.0 |   9 |
|   19 | 15 Gi |   7.0 |   5 |
|   20 | 15 Gi |   7.0 |   8 |
|   21 | 15 Gi |   7.0 |  10 |
|   22 | 15 Gi |   7.0 |   6 |
|   23 | 15 Gi |   7.0 |  10 |
|   24 | 15 Gi |   7.0 |   7 |
|   25 | 15 Gi |   7.0 |  13 |
|   26 | 15 Gi |   7.0 |   7 |
|   27 | 15 Gi |   7.0 |   5 |
|   28 | 15 Gi |   7.0 |   5 |
|   29 | 15 Gi |   7.0 |   6 |
|   30 | 15 Gi |   7.0 |  13 |
|   31 | 15 Gi |   7.0 |   9 |
|   32 | 15 Gi |   7.0 |   3 |
|   33 | 15 Gi |   7.0 |   7 |
|   34 | 15 Gi |   7.0 |   7 |
|   35 | 15 Gi |   7.0 |   9 |
|   36 | 15 Gi |   7.0 |   7 |
|   37 | 15 Gi |   7.0 |   9 |
|   38 | 15 Gi |   7.0 |   6 |
|   39 | 15 Gi |   7.0 |   4 |
|   40 | 15 Gi |   7.0 |   3 |
|   41 | 15 Gi |   7.0 |   9 |
|   42 | 15 Gi |   7.0 |   9 |
|   43 | 15 Gi |   7.0 |   6 |
|   44 | 15 Gi |   7.0 |  12 |
|   45 | 15 Gi |   7.0 |   8 |
|   46 | 15 Gi |   7.0 |  14 |
|   47 | 15 Gi |   7.0 |  12 |
|   48 | 15 Gi |   7.0 |  13 |
|   49 | 15 Gi |   7.0 |  10 |
|   50 | 15 Gi |   7.0 |   8 |
|   51 | 15 Gi |   7.0 |   5 |
|   52 | 15 Gi |   7.0 |   6 |
|   53 | 15 Gi |   7.0 |   9 |
|   54 | 15 Gi |   7.0 |   8 |
|   55 | 15 Gi |   7.0 |   7 |
|   56 | 15 Gi |   7.0 |  10 |
|   57 | 15 Gi |   7.0 |   8 |
|   58 | 15 Gi |   7.0 |  16 |
|   59 | 15 Gi |   7.0 |  10 |
|   60 | 15 Gi |   7.0 |  12 |
|   61 | 15 Gi |   7.0 |  13 |
|   62 | 15 Gi |   7.0 |   7 |
|   63 | 15 Gi |   7.0 |   5 |
|   64 | 15 Gi |   7.0 |   7 |
|   80 | 15 Gi |   7.0 |   6 |
|   96 | 15 Gi |   7.0 |  11 |
|  112 | 15 Gi |   7.0 |   6 |
|  120 | 15 Gi |   7.0 |   5 |
|  128 | 15 Gi |   7.0 |   6 |
|  160 | 15 Gi |   7.0 |   9 |
|  256 | 15 Gi |   7.0 |   8 |
|  320 | 15 Gi |   7.0 |   9 |
|  336 | 15 Gi |   7.0 |  10 |
|  384 | 15 Gi |   7.0 |  10 |
|  448 | 15 Gi |   7.0 |   9 |
|  512 | 15 Gi |   7.0 |   9 |
|  784 | 15 Gi |   7.0 |  10 |
|  888 | 15 Gi |   7.0 |  10 |
|  896 | 15 Gi |   7.0 |   6 |
| 1024 | 15 Gi |   7.0 |   7 |
| 4096 | 15 Gi |   7.0 |   8 |
|    8 | 62 Gi | 120.1 | 123 |
|   12 | 62 Gi | 120.1 | 108 |
|   16 | 62 Gi | 120.1 | 102 |
|   24 | 62 Gi | 120.1 | 112 |
|   32 | 62 Gi | 120.1 | 137 |
|   40 | 62 Gi | 120.1 | 168 |
|   48 | 62 Gi | 120.1 | 149 |
|   64 | 62 Gi | 120.1 | 161 |
|   80 | 62 Gi | 120.1 | 186 |
|   96 | 62 Gi | 120.1 | 172 |
|  112 | 62 Gi | 120.1 | 201 |
|  120 | 62 Gi | 120.1 | 172 |
|  128 | 62 Gi | 120.1 | 187 |
|  160 | 62 Gi | 120.1 | 183 |
