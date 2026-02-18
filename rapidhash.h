/*
 * rapidhash V4 - Very fast, high quality, platform-independent hashing algorithm.
 *
 * Based on 'wyhash', by Wang Yi <godspeed_china@yeah.net>
 * 
 * Copyright (C) 2026 Nicolas De Carli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * You can contact the author at:
 *   - rapidhash source repository: https://github.com/Nicoshev/rapidhash
 */

 #pragma once
 
/*
 *  Includes.
 */
 #include <stdint.h>
 #include <string.h>
 #if defined(_MSC_VER)
 # include <intrin.h>
 # if defined(_M_X64) && !defined(_M_ARM64EC)
 #   pragma intrinsic(_umul128)
 # endif
 #endif
 
 /*
  *  C/C++ macros.
  */
 
 #ifdef _MSC_VER
 # define RAPIDHASH_ALWAYS_INLINE __forceinline
 #elif defined(__GNUC__)
 # define RAPIDHASH_ALWAYS_INLINE inline __attribute__((__always_inline__))
 #else
 # define RAPIDHASH_ALWAYS_INLINE inline
 #endif
 
 #ifdef __cplusplus
 # define RAPIDHASH_NOEXCEPT noexcept
 # define RAPIDHASH_CONSTEXPR constexpr
 # ifndef RAPIDHASH_INLINE
 #   define RAPIDHASH_INLINE RAPIDHASH_ALWAYS_INLINE
 # endif
 # if __cplusplus >= 201402L && !defined(_MSC_VER)
 #   define RAPIDHASH_INLINE_CONSTEXPR RAPIDHASH_ALWAYS_INLINE constexpr
 # else
 #   define RAPIDHASH_INLINE_CONSTEXPR RAPIDHASH_ALWAYS_INLINE
 # endif
 #else
 # define RAPIDHASH_NOEXCEPT
 # define RAPIDHASH_CONSTEXPR static const
 # ifndef RAPIDHASH_INLINE
 #   define RAPIDHASH_INLINE static RAPIDHASH_ALWAYS_INLINE
 # endif
 # define RAPIDHASH_INLINE_CONSTEXPR RAPIDHASH_INLINE
 #endif

 /*
  *  Unrolled macro for rapidhashJumbo.
  *  Improves large input speed of rapidhashJumbo, increasing code size and worsening small input speed.
  *  Unrolled should be used if hashing large files.
  *
  *  RAPIDHASH_JUMBO_COMPACT: Normal behavior, each iteration process 112 bytes.
  *  RAPIDHASH_JUMBO_UNROLLED: Each iteration process 224 bytes.
  *
  */
  #ifndef RAPIDHASH_JUMBO_UNROLLED
  # define RAPIDHASH_JUMBO_COMPACT
  #elif defined(RAPIDHASH_JUMBO_COMPACT)
  # error "cannot define RAPIDHASH_JUMBO_COMPACT and RAPIDHASH_JUMBO_UNROLLED simultaneously."
  #endif
 
 /*
  *  Likely and unlikely macros.
  */
 #if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
 # define _likely_(x)  __builtin_expect(x,1)
 # define _unlikely_(x)  __builtin_expect(x,0)
 #else
 # define _likely_(x) (x)
 # define _unlikely_(x) (x)
 #endif

 /*
  *  Likely macro, ignored on Apple compilation
  *  Unusual side effects are observed when using Apple clang.
  */
 #if !defined(__clang__) || !defined(__apple_build_version__)
 # define _maybe_likely_(x) _likely_(x) 
 #else
 # define _maybe_likely_(x) (x) 
 #endif
 
 /*
  *  Endianness macros.
  */
 #ifndef RAPIDHASH_LITTLE_ENDIAN
 # if defined(_WIN32) || defined(__LITTLE_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
 #   define RAPIDHASH_LITTLE_ENDIAN
 # elif defined(__BIG_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
 #   define RAPIDHASH_BIG_ENDIAN
 # else
 #   warning "could not determine endianness! Falling back to little endian."
 #   define RAPIDHASH_LITTLE_ENDIAN
 # endif
 #endif
 
 /*
  *  Default secret parameters.
  */
   RAPIDHASH_CONSTEXPR uint64_t rapid_secret[7] = {
     0x8bb84b93962eacc9ull,
     0x4b33a62ed433d4a3ull,
     0x2d358dccaa6c78a5ull,
     0x4d5a2da51de1aa47ull,
     0xa0761d6478bd642full,
     0xe7037ed1a0b428dbull,
     0x90ed1765281c388cull};
 
 /*
  *  64*64 -> 128bit multiply function.
  *
  *  @param A  Address of 64-bit number.
  *  @param B  Address of 64-bit number.
  *
  *  Calculates 128-bit C = *A * *B.
  *
  *  When RAPIDHASH_FAST is defined:
  *  Overwrites A contents with C's low 64 bits.
  *  Overwrites B contents with C's high 64 bits.
  *
  *  When RAPIDHASH_PROTECTED is defined:
  *  Xors and overwrites A contents with C's low 64 bits.
  *  Xors and overwrites B contents with C's high 64 bits.
  */
 RAPIDHASH_INLINE_CONSTEXPR void rapid_mum(uint64_t *A, uint64_t *B) RAPIDHASH_NOEXCEPT {
 #if defined(__SIZEOF_INT128__)
   __uint128_t r=*A; r*=*B;
   *A=(uint64_t)r; *B=(uint64_t)(r>>64);
 #elif defined(_MSC_VER) && (defined(_WIN64) || defined(_M_HYBRID_CHPE_ARM64))
   #if defined(_M_X64)
     *A=_umul128(*A,*B,B);
   #else
     uint64_t c = __umulh(*A, *B);
   #endif
 #else
   uint64_t ha=*A>>32, hb=*B>>32, la=(uint32_t)*A, lb=(uint32_t)*B;
   uint64_t rh=ha*hb, rm0=ha*lb, rm1=hb*la, rl=la*lb, t=rl+(rm0<<32), c=t<rl;
   uint64_t lo=t+(rm1<<32); 
   c+=lo<t; 
   uint64_t hi=rh+(rm0>>32)+(rm1>>32)+c;
   *A=lo;  *B=hi;
 #endif
 }
 
 /*
  *  Multiply and xor mix function.
  *
  *  @param A  64-bit number.
  *  @param B  64-bit number.
  *
  *  Calculates 128-bit C = A * B.
  *  Returns 64-bit xor between high and low 64 bits of C.
  */
  RAPIDHASH_INLINE_CONSTEXPR uint64_t rapid_mix(uint64_t A, uint64_t B) RAPIDHASH_NOEXCEPT { rapid_mum(&A,&B); return A^B; }
 
 /*
  *  Read functions.
  */
 #ifdef RAPIDHASH_LITTLE_ENDIAN
 RAPIDHASH_INLINE uint64_t rapid_read64(const uint8_t *p) RAPIDHASH_NOEXCEPT { uint64_t v; memcpy(&v, p, sizeof(uint64_t)); return v;}
 RAPIDHASH_INLINE uint64_t rapid_read32(const uint8_t *p) RAPIDHASH_NOEXCEPT { uint32_t v; memcpy(&v, p, sizeof(uint32_t)); return v;}
 #elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
 RAPIDHASH_INLINE uint64_t rapid_read64(const uint8_t *p) RAPIDHASH_NOEXCEPT { uint64_t v; memcpy(&v, p, sizeof(uint64_t)); return __builtin_bswap64(v);}
 RAPIDHASH_INLINE uint64_t rapid_read32(const uint8_t *p) RAPIDHASH_NOEXCEPT { uint32_t v; memcpy(&v, p, sizeof(uint32_t)); return __builtin_bswap32(v);}
 #elif defined(_MSC_VER)
 RAPIDHASH_INLINE uint64_t rapid_read64(const uint8_t *p) RAPIDHASH_NOEXCEPT { uint64_t v; memcpy(&v, p, sizeof(uint64_t)); return _byteswap_uint64(v);}
 RAPIDHASH_INLINE uint64_t rapid_read32(const uint8_t *p) RAPIDHASH_NOEXCEPT { uint32_t v; memcpy(&v, p, sizeof(uint32_t)); return _byteswap_ulong(v);}
 #else
 RAPIDHASH_INLINE uint64_t rapid_read64(const uint8_t *p) RAPIDHASH_NOEXCEPT {
   uint64_t v; memcpy(&v, p, 8);
   return (((v >> 56) & 0xff)| ((v >> 40) & 0xff00)| ((v >> 24) & 0xff0000)| ((v >>  8) & 0xff000000)| ((v <<  8) & 0xff00000000)| ((v << 24) & 0xff0000000000)| ((v << 40) & 0xff000000000000)| ((v << 56) & 0xff00000000000000));
 }
 RAPIDHASH_INLINE uint64_t rapid_read32(const uint8_t *p) RAPIDHASH_NOEXCEPT {
   uint32_t v; memcpy(&v, p, 4);
   return (((v >> 24) & 0xff)| ((v >>  8) & 0xff00)| ((v <<  8) & 0xff0000)| ((v << 24) & 0xff000000));
 }
 #endif

 /*
  *  Safer way to treat input.
  *  Protects from state reset if RAPIDHASH_RESILIENT is defined.
  *
  *  @param input   Key just read from input buffer.
  *  @param secret  Secret to mix with input
  *  @param seed    64-bit seed used to alter the hash result predictably.
  *
  *  Returns a 64-bit number.
  */
 RAPIDHASH_INLINE_CONSTEXPR uint64_t rapid_input(uint64_t input, uint64_t secret, uint64_t seed) RAPIDHASH_NOEXCEPT {
#ifdef RAPIDHASH_RESILIENT
  return input != secret ? input ^ secret : seed;
#else
  (void)seed;
  return input ^ secret;
#endif
 }
 
 /*
  *  rapidhash main function.
  *
  *  @param key     Buffer to be hashed.
  *  @param len     @key length, in bytes.
  *  @param seed    64-bit seed used to alter the hash result predictably.
  *  @param secret  Triplet of 64-bit secrets used to alter hash result predictably.
  *
  *  Returns a 64-bit hash.
  */
RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhashJumbo_internal(const void *key, size_t len, uint64_t seed, const uint64_t* secret) RAPIDHASH_NOEXCEPT {
  const uint8_t *p=(const uint8_t *)key;
  seed ^= rapid_mix(seed ^ secret[1], secret[0]);
  uint64_t a=0, b=0;
  if (_likely_(len <= 16)) {
    const uint8_t* pend = p + len;
    if (len >= 4) {
      seed ^= len;
      if (len > 8) {
        const uint8_t* plast = pend - 8;
        a = rapid_read64(p);
        b = rapid_read64(plast);
      } else {
        const uint8_t* plast = pend - 4;
        a = rapid_read32(p);
        b = rapid_read32(plast);
      }
    } else if (_maybe_likely_(len > 0)) {
      a = (((uint64_t)p[0])<<11) | *(pend - 1);
      b = p[len>>1];
    }
  } else {
    if (len > 112) {
#ifdef RAPIDHASH_RESILIENT
        seed ^= len;
#endif
      uint64_t see1 = seed, see2 = seed;
      uint64_t see3 = seed, see4 = seed;
      uint64_t see5 = seed, see6 = seed;
#ifdef RAPIDHASH_JUMBO_COMPACT
      do {
        seed = rapid_mix(rapid_input(rapid_read64(p), secret[0], seed), rapid_read64(p + 8) ^ seed);
        see1 = rapid_mix(rapid_input(rapid_read64(p + 16), secret[1], see1), rapid_read64(p + 24) ^ see1);
        see2 = rapid_mix(rapid_input(rapid_read64(p + 32), secret[2], see1), rapid_read64(p + 40) ^ see2);
        see3 = rapid_mix(rapid_input(rapid_read64(p + 48), secret[3], see1), rapid_read64(p + 56) ^ see3);
        see4 = rapid_mix(rapid_input(rapid_read64(p + 64), secret[4], see1), rapid_read64(p + 72) ^ see4);
        see5 = rapid_mix(rapid_input(rapid_read64(p + 80), secret[5], see1), rapid_read64(p + 88) ^ see5);
        see6 = rapid_mix(rapid_input(rapid_read64(p + 96), secret[6], see1), rapid_read64(p + 104) ^ see6);
        p += 112;
        len -= 112;
      } while(len > 112);
#else
      while (len > 224) {
        seed = rapid_mix(rapid_input(rapid_read64(p), secret[0], seed), rapid_read64(p + 8) ^ seed);
        see1 = rapid_mix(rapid_input(rapid_read64(p + 16), secret[1], see1), rapid_read64(p + 24) ^ see1);
        see2 = rapid_mix(rapid_input(rapid_read64(p + 32), secret[2], see1), rapid_read64(p + 40) ^ see2);
        see3 = rapid_mix(rapid_input(rapid_read64(p + 48), secret[3], see1), rapid_read64(p + 56) ^ see3);
        see4 = rapid_mix(rapid_input(rapid_read64(p + 64), secret[4], see1), rapid_read64(p + 72) ^ see4);
        see5 = rapid_mix(rapid_input(rapid_read64(p + 80), secret[5], see1), rapid_read64(p + 88) ^ see5);
        see6 = rapid_mix(rapid_input(rapid_read64(p + 96), secret[6], see1), rapid_read64(p + 104) ^ see6);
        seed = rapid_mix(rapid_input(rapid_read64(p + 112), secret[0], seed), rapid_read64(p + 120) ^ seed);
        see1 = rapid_mix(rapid_input(rapid_read64(p + 128), secret[1], see1), rapid_read64(p + 136) ^ see1);
        see2 = rapid_mix(rapid_input(rapid_read64(p + 144), secret[2], see1), rapid_read64(p + 152) ^ see2);
        see3 = rapid_mix(rapid_input(rapid_read64(p + 160), secret[3], see1), rapid_read64(p + 168) ^ see3);
        see4 = rapid_mix(rapid_input(rapid_read64(p + 176), secret[4], see1), rapid_read64(p + 184) ^ see4);
        see5 = rapid_mix(rapid_input(rapid_read64(p + 192), secret[5], see1), rapid_read64(p + 200) ^ see5);
        see6 = rapid_mix(rapid_input(rapid_read64(p + 208), secret[6], see1), rapid_read64(p + 216) ^ see6);
        p += 224;
        len -= 224;
      }
      if (len > 112) {
        seed = rapid_mix(rapid_input(rapid_read64(p), secret[0], seed), rapid_read64(p + 8) ^ seed);
        see1 = rapid_mix(rapid_input(rapid_read64(p + 16), secret[1], see1), rapid_read64(p + 24) ^ see1);
        see2 = rapid_mix(rapid_input(rapid_read64(p + 32), secret[2], see1), rapid_read64(p + 40) ^ see2);
        see3 = rapid_mix(rapid_input(rapid_read64(p + 48), secret[3], see1), rapid_read64(p + 56) ^ see3);
        see4 = rapid_mix(rapid_input(rapid_read64(p + 64), secret[4], see1), rapid_read64(p + 72) ^ see4);
        see5 = rapid_mix(rapid_input(rapid_read64(p + 80), secret[5], see1), rapid_read64(p + 88) ^ see5);
        see6 = rapid_mix(rapid_input(rapid_read64(p + 96), secret[6], see1), rapid_read64(p + 104) ^ see6);
        p += 112;
        len -= 112;
      }
#endif
      seed ^= see1;
      see2 ^= see3;
      see4 ^= see5;
      seed ^= see6;
      see2 ^= see4;
      seed ^= see2;
    }
    if (len > 16) {
      seed = rapid_mix(rapid_input(rapid_read64(p), secret[1], seed), rapid_read64(p + 8) ^ seed);
      if (len > 32) {
        seed = rapid_mix(rapid_input(rapid_read64(p + 16), secret[0], seed), rapid_read64(p + 24) ^ seed);
        if (len > 48) {
          seed = rapid_mix(rapid_input(rapid_read64(p + 32), secret[1], seed), rapid_read64(p + 40) ^ seed);
          if (len > 64) {
            seed = rapid_mix(rapid_input(rapid_read64(p + 48), secret[0], seed), rapid_read64(p + 56) ^ seed);
            if (len > 80) {
              seed = rapid_mix(rapid_input(rapid_read64(p + 64), secret[1], seed), rapid_read64(p + 72) ^ seed);
              if (len > 96) {
                seed = rapid_mix(rapid_input(rapid_read64(p + 80), secret[0], seed), rapid_read64(p + 88) ^ seed);
              }
            }
          }
        }
      }
    }
    b=rapid_read64(p+len-8); a=rapid_read64(p+len-16) ^ len;  
  }
  a ^= secret[0];
  seed ^= b;
  b = seed;
  rapid_mum(&a, &b);
  return rapid_mix(a ^ seed, b ^ secret[0] ^ len);
}

 /*
  *  rapidhashMicro main function.
  *
  *  @param key     Buffer to be hashed.
  *  @param len     @key length, in bytes.
  *  @param seed    64-bit seed used to alter the hash result predictably.
  *  @param secret  Triplet of 64-bit secrets used to alter hash result predictably.
  *
  *  Returns a 64-bit hash.
  */
  RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhash_internal(const void *key, size_t len, uint64_t seed, const uint64_t* secret) RAPIDHASH_NOEXCEPT {
    const uint8_t *p=(const uint8_t *)key;
    seed ^= rapid_mix(seed ^ secret[1], secret[0]);
    uint64_t a=0, b=0;
    if (_likely_(len <= 16)) {
      const uint8_t* pend = p + len;
      if (len >= 4) {
        seed ^= len;
        if (len > 8) {
          const uint8_t* plast = pend - 8;
          a = rapid_read64(p);
          b = rapid_read64(plast);
        } else {
          const uint8_t* plast = pend - 4;
          a = rapid_read32(p);
          b = rapid_read32(plast);
        }
      } else if (_likely_(len > 0)) {
        a = (((uint64_t)p[0])<<11) | *(pend - 1);
        b = p[len>>1];
      }
    } else {
      if (len > 80) {
#ifdef RAPIDHASH_RESILIENT
        seed ^= len;
#endif
        uint64_t see1 = seed, see2 = seed;
        uint64_t see3 = seed, see4 = seed;
        do {
          seed = rapid_mix(rapid_input(rapid_read64(p), secret[0], seed), rapid_read64(p + 8) ^ seed);
          see1 = rapid_mix(rapid_input(rapid_read64(p + 16), secret[1], see1), rapid_read64(p + 24) ^ see1);
          see2 = rapid_mix(rapid_input(rapid_read64(p + 32), secret[2], see1), rapid_read64(p + 40) ^ see2);
          see3 = rapid_mix(rapid_input(rapid_read64(p + 48), secret[3], see1), rapid_read64(p + 56) ^ see3);
          see4 = rapid_mix(rapid_input(rapid_read64(p + 64), secret[4], see1), rapid_read64(p + 72) ^ see4);
          p += 80;
          len -= 80;
        } while(len > 80);
        seed ^= see1;
        see2 ^= see3;
        seed ^= see4;
        seed ^= see2;
      }
      if (len > 16) {
        seed = rapid_mix(rapid_input(rapid_read64(p), secret[1], seed), rapid_read64(p + 8) ^ seed);
        if (len > 32) {
          seed = rapid_mix(rapid_input(rapid_read64(p + 16), secret[0], seed), rapid_read64(p + 24) ^ seed);
          if (len > 48) {
              seed = rapid_mix(rapid_input(rapid_read64(p + 32), secret[1], seed), rapid_read64(p + 40) ^ seed);
              if (len > 64) {
                  seed = rapid_mix(rapid_input(rapid_read64(p + 48), secret[0], seed), rapid_read64(p + 56) ^ seed);
              }
          }
        }
      }
      b=rapid_read64(p+len-8); a=rapid_read64(p+len-16) ^ len;
    }
    a ^= secret[0];
    seed ^= b;
    b = seed;
    rapid_mum(&a, &b);
    return rapid_mix(a ^ seed, b ^ secret[0] ^ len);
  }

  /*
  *  rapidhashNano main function.
  *
  *  @param key     Buffer to be hashed.
  *  @param len     @key length, in bytes.
  *  @param seed    64-bit seed used to alter the hash result predictably.
  *  @param secret  Triplet of 64-bit secrets used to alter hash result predictably.
  *
  *  Returns a 64-bit hash.
  */
  RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhashNano_internal(const void *key, size_t len, uint64_t seed, const uint64_t* secret) RAPIDHASH_NOEXCEPT {
    const uint8_t *p=(const uint8_t *)key;
    seed ^= rapid_mix(seed ^ secret[1], secret[0]);
    uint64_t a=0, b=0;
    if (_likely_(len <= 16)) {
      const uint8_t* pend = p + len;
      if (len >= 4) {
        seed ^= len;
        if (len > 8) {
          const uint8_t* plast = pend - 8;
          a = rapid_read64(p);
          b = rapid_read64(plast);
        } else {
          const uint8_t* plast = pend - 4;
          a = rapid_read32(p);
          b = rapid_read32(plast);
        }
      } else if (_maybe_likely_(len > 0)) {
        a = (((uint64_t)p[0])<<11) | *(pend - 1);
        b = p[len>>1];
      }
    } else {
      if (len > 48) {
#ifdef RAPIDHASH_RESILIENT
        seed ^= len;
#endif
        uint64_t see1 = seed, see2 = seed;
        do {
          seed = rapid_mix(rapid_input(rapid_read64(p), secret[0], seed), rapid_read64(p + 8) ^ seed);
          see1 = rapid_mix(rapid_input(rapid_read64(p + 16), secret[1], see1), rapid_read64(p + 24) ^ see1);
          see2 = rapid_mix(rapid_input(rapid_read64(p + 32), secret[2], see1), rapid_read64(p + 40) ^ see2);
          p += 48;
          len -= 48;
        } while(len > 48);
        seed ^= see1;
        seed ^= see2;
      }
      if (len > 16) {
        seed = rapid_mix(rapid_input(rapid_read64(p), secret[1], seed), rapid_read64(p + 8) ^ seed);
        if (len > 32) {
          seed = rapid_mix(rapid_input(rapid_read64(p + 16), secret[0], seed), rapid_read64(p + 24) ^ seed);
        }
      }
      b=rapid_read64(p+len-8); a=rapid_read64(p+len-16) ^ len;
      
    }
    a ^= secret[0];
    seed ^= b;
    b = seed;
    rapid_mum(&a, &b);
    return rapid_mix(a ^ seed, b ^ secret[0] ^ len);
  }
 
/*
 *  rapidhashJumbo seeded hash function.
 *
 *  General purpose hash function, amazing performance across all sizes.
 *  Faster than rapidhash for input sizes greater than 1kb.
 *  Can be made even faster for inputs above 4k using RAPIDHASH_JUMBO_UNROLLED.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *
 *  Calls rapidhash_internal using provided parameters and default secrets.
 *
 *  Returns a 64-bit hash.
 */
RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhashJumbo_withSeed(const void *key, size_t len, uint64_t seed) RAPIDHASH_NOEXCEPT {
  return rapidhashJumbo_internal(key, len, seed, rapid_secret);
}
 
/*
 *  rapidhashJumbo hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *
 *  Calls rapidhash_withSeed using provided parameters and the default seed.
 *
 *  Returns a 64-bit hash.
 */
RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhashJumbo(const void *key, size_t len) RAPIDHASH_NOEXCEPT {
  return rapidhashJumbo_withSeed(key, len, 0);
}

/*
 *  rapidhash seeded hash function.
 *
 *  Designed for HPC and server applications.
 *  Clang-18+ compiles it to ~140 instructions, without stack usage on aarch64.
 *  Faster than rapidhashJumbo for sizes up to 512 bytes, just 15%-20% slower for inputs above 1kb.
 *  Faster than rapidhashNano for sizes larger than 256 bytes, slightly slower for inputs smaller than 256 bytes.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *
 *  Calls rapidhash_internal using provided parameters and default secrets.
 *
 *  Returns a 64-bit hash.
 */
 RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhash_withSeed(const void *key, size_t len, uint64_t seed) RAPIDHASH_NOEXCEPT {
  return rapidhash_internal(key, len, seed, rapid_secret);
}
 
/*
 *  rapidhas hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *
 *  Calls rapidhash_withSeed using provided parameters and the default seed.
 *
 *  Returns a 64-bit hash.
 */
RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhash(const void *key, size_t len) RAPIDHASH_NOEXCEPT {
  return rapidhash_withSeed(key, len, 0);
}

/*
 *  rapidhashNano seeded hash function.
 *
 *  Designed for maximum speed on small inputs, excellent choice for hashmaps.
 *  Good on mobile and embedded applications, where keeping a small code size is a top priority.
 *  Clang-18+ compiles it to less than 100 instructions, without stack usage on aarch64.
 *  The fastest for sizes up to 240 bytes, may be considerably slower for larger inputs.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *
 *  Calls rapidhash_internal using provided parameters and default secrets.
 *
 *  Returns a 64-bit hash.
 */
 RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhashNano_withSeed(const void *key, size_t len, uint64_t seed) RAPIDHASH_NOEXCEPT {
  return rapidhashNano_internal(key, len, seed, rapid_secret);
}
 
/*
 *  rapidhashNano hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *
 *  Calls rapidhash_withSeed using provided parameters and the default seed.
 *
 *  Returns a 64-bit hash.
 */
RAPIDHASH_INLINE_CONSTEXPR uint64_t rapidhashNano(const void *key, size_t len) RAPIDHASH_NOEXCEPT {
  return rapidhashNano_withSeed(key, len, 0);
}
