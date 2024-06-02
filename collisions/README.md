
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

| Algorithm | Input Len | Nb Hashes | Expected | Nb Collisions | 
| ---           | --- | ---   | ---   | --- |
| __rapidhash__ | 5   | 15 Gi |   7.0 |   8 |
| __rapidhash__ | 6   | 15 Gi |   7.0 |   5 |
| __rapidhash__ | 7   | 15 Gi |   7.0 |  11 |
| __rapidhash__ | 8   | 15 Gi |   7.0 |   7 | 
| __rapidhash__ | 9   | 15 Gi |   7.0 |   6 |
| __rapidhash__ | 10  | 15 Gi |   7.0 |   3 |
| __rapidhash__ | 11  | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 12  | 15 Gi |   7.0 |   7 |
| __rapidhash__ | 13  | 15 Gi |   7.0 |  10 |
| __rapidhash__ | 14  | 15 Gi |   7.0 |   5 |
| __rapidhash__ | 15  | 15 Gi |   7.0 |   3 |
| __rapidhash__ | 16  | 15 Gi |   7.0 |  12 |
| __rapidhash__ | 17  | 15 Gi |   7.0 |  10 |
| __rapidhash__ | 18  | 15 Gi |   7.0 |   5 |
| __rapidhash__ | 19  | 15 Gi |   7.0 |   8 |
| __rapidhash__ | 20  | 15 Gi |   7.0 |  13 |
| __rapidhash__ | 21  | 15 Gi |   7.0 |   5 |
| __rapidhash__ | 22  | 15 Gi |   7.0 |   5 |
| __rapidhash__ | 23  | 15 Gi |   7.0 |   8 |
| __rapidhash__ | 24  | 15 Gi |   7.0 |   7 |
| __rapidhash__ | 25  | 15 Gi |   7.0 |   8 |
| __rapidhash__ | 26  | 15 Gi |   7.0 |   7 |
| __rapidhash__ | 27  | 15 Gi |   7.0 |   5 |
| __rapidhash__ | 28  | 15 Gi |   7.0 |   8 |
| __rapidhash__ | 29  | 15 Gi |   7.0 |   2 |
| __rapidhash__ | 30  | 15 Gi |   7.0 |   2 |
| __rapidhash__ | 31  | 15 Gi |   7.0 |   6 |
| __rapidhash__ | 32  | 15 Gi |   7.0 |  12 |
| __rapidhash__ | 33  | 15 Gi |   7.0 |   4 |
| __rapidhash__ | 34  | 15 Gi |   7.0 |  11 |
| __rapidhash__ | 35  | 15 Gi |   7.0 |  10 |
| __rapidhash__ | 36  | 15 Gi |   7.0 |  11 |
| __rapidhash__ | 37  | 15 Gi |   7.0 |   4 |
| __rapidhash__ | 38  | 15 Gi |   7.0 |   4 |
| __rapidhash__ | 39  | 15 Gi |   7.0 |  12 |
| __rapidhash__ | 40  | 15 Gi |   7.0 |   7 |
| __rapidhash__ | 41  | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 42  | 15 Gi |   7.0 |   8 |
| __rapidhash__ | 43  | 15 Gi |   7.0 |   7 |
| __rapidhash__ | 44  | 15 Gi |   7.0 |  11 |
| __rapidhash__ | 45  | 15 Gi |   7.0 |  10 |
| __rapidhash__ | 46  | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 47  | 15 Gi |   7.0 |  10 |
| __rapidhash__ | 48  | 15 Gi |   7.0 |   7 |
| __rapidhash__ | 49  | 15 Gi |   7.0 |  11 |
| __rapidhash__ | 50  | 15 Gi |   7.0 |  10 |
| __rapidhash__ | 51  | 15 Gi |   7.0 |   6 |
| __rapidhash__ | 52  | 15 Gi |   7.0 |  12 |
| __rapidhash__ | 53  | 15 Gi |   7.0 |   8 |
| __rapidhash__ | 54  | 15 Gi |   7.0 |   8 |
| __rapidhash__ | 55  | 15 Gi |   7.0 |  13 |
| __rapidhash__ | 56  | 15 Gi |   7.0 |  12 |
| __rapidhash__ | 57  | 15 Gi |   7.0 |   4 |
| __rapidhash__ | 58  | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 59  | 15 Gi |   7.0 |   4 |
| __rapidhash__ | 60  | 15 Gi |   7.0 |   5 |
| __rapidhash__ | 61  | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 62  | 15 Gi |   7.0 |   7 |
| __rapidhash__ | 63  | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 64  | 15 Gi |   7.0 |   6 |
| __rapidhash__ | 80  | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 96  | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 112 | 15 Gi |   7.0 |   5 |
| __rapidhash__ | 128 | 15 Gi |   7.0 |  12 |
| __rapidhash__ | 160 | 15 Gi |   7.0 |   9 |
| __rapidhash__ | 192 | 15 Gi |   7.0 |   7 |
| __rapidhash__ | 256 | 15 Gi |   7.0 |   4 |
| __rapidhash__ | 384 | 15 Gi |   7.0 |   6 |
| __rapidhash__ | 8   | 62 Gi | 120.1 |  99 |
| __rapidhash__ | 12  | 62 Gi | 120.1 | 131 |
| __rapidhash__ | 16  | 62 Gi | 120.1 | 127 |
| __rapidhash__ | 24  | 62 Gi | 120.1 | 126 |
| __rapidhash__ | 32  | 62 Gi | 120.1 | 133 |
| __rapidhash__ | 40  | 62 Gi | 120.1 | 145 |
| __rapidhash__ | 48  | 62 Gi | 120.1 | 123 |
| __rapidhash__ | 56  | 62 Gi | 120.1 | 143 |
| __rapidhash__ | 64  | 62 Gi | 120.1 | 192 |
| __rapidhash__ | 80  | 62 Gi | 120.1 | 188 |
| __rapidhash__ | 256 | 62 Gi | 120.1 | 181 |
