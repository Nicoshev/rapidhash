
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

Here are some results for rapidhash, using its default seed:

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
