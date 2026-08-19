/*
 * rapidhash V3 - Very fast, high quality, platform-independent hashing algorithm.
 *
 * Based on 'wyhash', by Wang Yi <godspeed_china@yeah.net>
 * 
 * Copyright (C) 2025 Nicolas De Carli
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
 # define RAPIDHASH_CT_ALWAYS_INLINE __forceinline
 #elif defined(__GNUC__)
 # define RAPIDHASH_CT_ALWAYS_INLINE inline __attribute__((__always_inline__))
 #else
 # define RAPIDHASH_CT_ALWAYS_INLINE inline
 #endif
 
 #ifdef __cplusplus
 # define RAPIDHASH_CT_NOEXCEPT noexcept
 # if defined(_MSC_VER) && !defined(__clang__)
 #   define RAPIDHASH_CT_CONSTEVAL constexpr
 # else
 #   define RAPIDHASH_CT_CONSTEVAL consteval
 # endif
  # ifndef RAPIDHASH_CT_INLINE
  #   define RAPIDHASH_CT_INLINE RAPIDHASH_CT_ALWAYS_INLINE
  # endif
  # if __cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L)
 #   if defined(_MSC_VER) && !defined(__clang__)
 #     define RAPIDHASH_CT_INLINE_CONSTEVAL RAPIDHASH_CT_ALWAYS_INLINE constexpr
 #   else
 #     define RAPIDHASH_CT_INLINE_CONSTEVAL RAPIDHASH_CT_ALWAYS_INLINE consteval
 #   endif
  # else
  #   define RAPIDHASH_CT_INLINE_CONSTEVAL RAPIDHASH_CT_ALWAYS_INLINE
  # endif
 #else
  # define RAPIDHASH_CT_NOEXCEPT
  # define RAPIDHASH_CT_CONSTEVAL static const
  # ifndef RAPIDHASH_CT_INLINE
  #   define RAPIDHASH_CT_INLINE static RAPIDHASH_CT_ALWAYS_INLINE
  # endif
  # define RAPIDHASH_CT_INLINE_CONSTEVAL RAPIDHASH_CT_INLINE
 #endif

 /*
  *  Unrolled macro.
  *  Improves large input speed, but increases code size and worsens small input speed.
  *
  *  RAPIDHASH_CT_COMPACT: Normal behavior.
  *  RAPIDHASH_CT_UNROLLED: 
  *
  */
  #ifndef RAPIDHASH_CT_UNROLLED
  # define RAPIDHASH_CT_COMPACT
  #elif defined(RAPIDHASH_CT_COMPACT)
  # error "cannot define RAPIDHASH_CT_COMPACT and RAPIDHASH_CT_UNROLLED simultaneously."
  #endif
 
 /*
  *  Protection macro, alters behaviour of rapid_ct_mum multiplication function.
  *
  *  RAPIDHASH_CT_FAST: Normal behavior, max speed.
  *  RAPIDHASH_CT_PROTECTED: Extra protection against entropy loss.
  */
 #ifndef RAPIDHASH_CT_PROTECTED
 # define RAPIDHASH_CT_FAST
 #elif defined(RAPIDHASH_CT_FAST)
 # error "cannot define RAPIDHASH_CT_PROTECTED and RAPIDHASH_CT_FAST simultaneously."
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
  *  Endianness macros.
  */
 #ifndef RAPIDHASH_CT_LITTLE_ENDIAN
 # if defined(_WIN32) || defined(__LITTLE_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
 #   define RAPIDHASH_CT_LITTLE_ENDIAN
 # elif defined(__BIG_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
 #   define RAPIDHASH_CT_BIG_ENDIAN
 # else
 #   warning "could not determine endianness! Falling back to little endian."
 #   define RAPIDHASH_CT_LITTLE_ENDIAN
 # endif
 #endif
 
 /*
  *  Default secret parameters.
  */
   constexpr uint64_t rapid_ct_secret[8] = {
     0x2d358dccaa6c78a5ull,
     0x8bb84b93962eacc9ull,
     0x4b33a62ed433d4a3ull,
     0x4d5a2da51de1aa47ull,
     0xa0761d6478bd642full,
     0xe7037ed1a0b428dbull,
     0x90ed1765281c388cull,
     0xaaaaaaaaaaaaaaaaull};
 
 /*
  *  64*64 -> 128bit multiply function.
  *
  *  @param A  Address of 64-bit number.
  *  @param B  Address of 64-bit number.
  *
  *  Calculates 128-bit C = *A * *B.
  *
  *  When RAPIDHASH_CT_FAST is defined:
  *  Overwrites A contents with C's low 64 bits.
  *  Overwrites B contents with C's high 64 bits.
  *
  *  When RAPIDHASH_CT_PROTECTED is defined:
  *  Xors and overwrites A contents with C's low 64 bits.
  *  Xors and overwrites B contents with C's high 64 bits.
  */
RAPIDHASH_CT_INLINE_CONSTEVAL void rapid_ct_mum(uint64_t& A, uint64_t& B) RAPIDHASH_CT_NOEXCEPT {
 #if defined(__SIZEOF_INT128__)
  __uint128_t r=A; r*=B;
   #ifdef RAPIDHASH_CT_PROTECTED
  A^=(uint64_t)r; B^=(uint64_t)(r>>64);
   #else
  A=(uint64_t)r; B=(uint64_t)(r>>64);
   #endif
 #else
  uint64_t ha=A>>32, hb=B>>32, la=(uint32_t)A, lb=(uint32_t)B;
   uint64_t rh=ha*hb, rm0=ha*lb, rm1=hb*la, rl=la*lb, t=rl+(rm0<<32), c=t<rl;
   uint64_t lo=t+(rm1<<32); 
   c+=lo<t; 
   uint64_t hi=rh+(rm0>>32)+(rm1>>32)+c;
   #ifdef RAPIDHASH_CT_PROTECTED
  A^=lo;  B^=hi;
   #else
  A=lo;  B=hi;
   #endif
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
  RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapid_ct_mix(uint64_t A, uint64_t B) RAPIDHASH_CT_NOEXCEPT { rapid_ct_mum(A,B); return A^B; }
 
 /*
  *  Read functions.
  */
 template<size_t S> struct rapid_ct_mask;
 template<> struct rapid_ct_mask<1> { static constexpr uint64_t value = 0xFFULL; };
 template<> struct rapid_ct_mask<2> { static constexpr uint64_t value = 0xFFFFULL; };
 template<> struct rapid_ct_mask<4> { static constexpr uint64_t value = 0xFFFFFFFFULL; };
 template<> struct rapid_ct_mask<8> { static constexpr uint64_t value = 0xFFFFFFFFFFFFFFFFULL; };

 #ifdef RAPIDHASH_CT_LITTLE_ENDIAN
 template<typename type>
 RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapid_ct_read64(const type *p) RAPIDHASH_CT_NOEXCEPT  {
    static_assert(sizeof(uint64_t) % sizeof(type) == 0, "type incompatible");
    uint64_t v = 0;
    for (size_t i = 0; i < sizeof(uint64_t) / sizeof(type); ++i) {
        v |= (static_cast<uint64_t>(p[i]) & rapid_ct_mask<sizeof(type)>::value) << (i * sizeof(type) * 8);
    }
    return v;
}
 template<typename type>
 RAPIDHASH_CT_INLINE_CONSTEVAL uint32_t rapid_ct_read32(const type *p) RAPIDHASH_CT_NOEXCEPT {
    static_assert(sizeof(uint32_t) % sizeof(type) == 0, "type incompatible");
    uint32_t v = 0;
    for (size_t i = 0; i < sizeof(uint32_t) / sizeof(type); ++i) {
        v |= static_cast<uint32_t>((static_cast<uint64_t>(p[i]) & rapid_ct_mask<sizeof(type)>::value)) << (i * sizeof(type) * 8);
    }
    return v;
}
 #elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
 template<typename type>
 RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapid_ct_read64(const type *p) RAPIDHASH_CT_NOEXCEPT {
    static_assert(sizeof(uint64_t) % sizeof(type) == 0, "type incompatible");
    uint64_t v = 0;
    for (size_t i = 0; i < sizeof(uint64_t) / sizeof(type); ++i) {
        v |= (static_cast<uint64_t>(p[i]) & rapid_ct_mask<sizeof(type)>::value) << ((sizeof(uint64_t) / sizeof(type) - 1 - i) * sizeof(type) * 8);
    }
    return __builtin_bswap64(v);
}
 template<typename type>
 RAPIDHASH_CT_INLINE_CONSTEVAL uint32_t rapid_ct_read32(const type *p) RAPIDHASH_CT_NOEXCEPT {
    static_assert(sizeof(uint32_t) % sizeof(type) == 0, "type incompatible");
    uint32_t v = 0;
    for (size_t i = 0; i < sizeof(uint32_t) / sizeof(type); ++i) {
        v |= static_cast<uint32_t>((static_cast<uint64_t>(p[i]) & rapid_ct_mask<sizeof(type)>::value)) << ((sizeof(uint32_t) / sizeof(type) - 1 - i) * sizeof(type) * 8);
    }
    return __builtin_bswap32(v);
}
 #else
 template<typename type>
 RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapid_ct_read64(const type *p) RAPIDHASH_CT_NOEXCEPT {
    static_assert(sizeof(uint64_t) % sizeof(type) == 0, "type incompatible");
    uint64_t v = 0;
    for (size_t i = 0; i < sizeof(uint64_t) / sizeof(type); ++i) {
        v |= (static_cast<uint64_t>(p[i]) & rapid_ct_mask<sizeof(type)>::value) << ((sizeof(uint64_t) / sizeof(type) - 1 - i) * sizeof(type) * 8);
    }
   return (((v >> 56) & 0xff)| ((v >> 40) & 0xff00)| ((v >> 24) & 0xff0000)| ((v >>  8) & 0xff000000)| ((v <<  8) & 0xff00000000)| ((v << 24) & 0xff0000000000)| ((v << 40) & 0xff000000000000)| ((v << 56) & 0xff00000000000000));
 }
 template<typename type>
 RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapid_ct_read32(const type *p) RAPIDHASH_CT_NOEXCEPT {
    static_assert(sizeof(uint32_t) % sizeof(type) == 0, "type incompatible");
    uint32_t v = 0;
    for (size_t i = 0; i < sizeof(uint32_t) / sizeof(type); ++i) {
        v |= static_cast<uint32_t>((static_cast<uint64_t>(p[i]) & rapid_ct_mask<sizeof(type)>::value)) << ((sizeof(uint32_t) / sizeof(type) - 1 - i) * sizeof(type) * 8);
    }
   return (((v >> 24) & 0xff)| ((v >>  8) & 0xff00)| ((v <<  8) & 0xff0000)| ((v << 24) & 0xff000000));
 }
 #endif
 
template<typename type>
RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapid_ct_get_byte(const type *p, size_t byte_idx) RAPIDHASH_CT_NOEXCEPT {
    size_t elem_idx = byte_idx / sizeof(type);
#ifdef RAPIDHASH_CT_LITTLE_ENDIAN
    size_t shift = (byte_idx % sizeof(type)) * 8;
#else
    size_t shift = (sizeof(type) - 1 - (byte_idx % sizeof(type))) * 8;
#endif
    return ((static_cast<uint64_t>(p[elem_idx]) & rapid_ct_mask<sizeof(type)>::value) >> shift) & 0xFF;
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
template<typename type>
RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhash_ct_internal(const type *key, size_t len, uint64_t seed, const uint64_t* secret) RAPIDHASH_CT_NOEXCEPT {
  const type *p=key;
  seed ^= rapid_ct_mix(seed ^ secret[2], secret[1]);
  uint64_t a=0, b=0;
  size_t i = len;
  if (_likely_(len <= 16)) {
    if (len >= 4) {
      seed ^= len;
      if (len >= 8) {
        const type* plast = p + (len - 8) / sizeof(type);
        a = rapid_ct_read64(p);
        b = rapid_ct_read64(plast);
      } else {
        const type* plast = p + (len - 4) / sizeof(type);
        a = rapid_ct_read32(p);
        b = rapid_ct_read32(plast);
      }
    } else if (len > 0) {
        a = (rapid_ct_get_byte(p, 0) << 45) | rapid_ct_get_byte(p, len - 1);
        b = rapid_ct_get_byte(p, len >> 1);
    } else
      a = b = 0;
  } else {
    if (len > 112) {
      uint64_t see1 = seed, see2 = seed;
      uint64_t see3 = seed, see4 = seed;
      uint64_t see5 = seed, see6 = seed;
#ifdef RAPIDHASH_CT_COMPACT
      do {
        seed = rapid_ct_mix(rapid_ct_read64(p) ^ secret[0], rapid_ct_read64(p + 8 / sizeof(type)) ^ seed);
        see1 = rapid_ct_mix(rapid_ct_read64(p + 16 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 24 / sizeof(type)) ^ see1);
        see2 = rapid_ct_mix(rapid_ct_read64(p + 32 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 40 / sizeof(type)) ^ see2);
        see3 = rapid_ct_mix(rapid_ct_read64(p + 48 / sizeof(type)) ^ secret[3], rapid_ct_read64(p + 56 / sizeof(type)) ^ see3);
        see4 = rapid_ct_mix(rapid_ct_read64(p + 64 / sizeof(type)) ^ secret[4], rapid_ct_read64(p + 72 / sizeof(type)) ^ see4);
        see5 = rapid_ct_mix(rapid_ct_read64(p + 80 / sizeof(type)) ^ secret[5], rapid_ct_read64(p + 88 / sizeof(type)) ^ see5);
        see6 = rapid_ct_mix(rapid_ct_read64(p + 96 / sizeof(type)) ^ secret[6], rapid_ct_read64(p + 104 / sizeof(type)) ^ see6);
        p += 112 / sizeof(type);
        i -= 112;
      } while(i > 112);
#else
      while (i > 224) {
        seed = rapid_ct_mix(rapid_ct_read64(p) ^ secret[0], rapid_ct_read64(p + 8 / sizeof(type)) ^ seed);
        see1 = rapid_ct_mix(rapid_ct_read64(p + 16 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 24 / sizeof(type)) ^ see1);
        see2 = rapid_ct_mix(rapid_ct_read64(p + 32 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 40 / sizeof(type)) ^ see2);
        see3 = rapid_ct_mix(rapid_ct_read64(p + 48 / sizeof(type)) ^ secret[3], rapid_ct_read64(p + 56 / sizeof(type)) ^ see3);
        see4 = rapid_ct_mix(rapid_ct_read64(p + 64 / sizeof(type)) ^ secret[4], rapid_ct_read64(p + 72 / sizeof(type)) ^ see4);
        see5 = rapid_ct_mix(rapid_ct_read64(p + 80 / sizeof(type)) ^ secret[5], rapid_ct_read64(p + 88 / sizeof(type)) ^ see5);
        see6 = rapid_ct_mix(rapid_ct_read64(p + 96 / sizeof(type)) ^ secret[6], rapid_ct_read64(p + 104 / sizeof(type)) ^ see6);
        seed = rapid_ct_mix(rapid_ct_read64(p + 112 / sizeof(type)) ^ secret[0], rapid_ct_read64(p + 120 / sizeof(type)) ^ seed);
        see1 = rapid_ct_mix(rapid_ct_read64(p + 128 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 136 / sizeof(type)) ^ see1);
        see2 = rapid_ct_mix(rapid_ct_read64(p + 144 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 152 / sizeof(type)) ^ see2);
        see3 = rapid_ct_mix(rapid_ct_read64(p + 160 / sizeof(type)) ^ secret[3], rapid_ct_read64(p + 168 / sizeof(type)) ^ see3);
        see4 = rapid_ct_mix(rapid_ct_read64(p + 176 / sizeof(type)) ^ secret[4], rapid_ct_read64(p + 184 / sizeof(type)) ^ see4);
        see5 = rapid_ct_mix(rapid_ct_read64(p + 192 / sizeof(type)) ^ secret[5], rapid_ct_read64(p + 200 / sizeof(type)) ^ see5);
        see6 = rapid_ct_mix(rapid_ct_read64(p + 208 / sizeof(type)) ^ secret[6], rapid_ct_read64(p + 216 / sizeof(type)) ^ see6);
        p += 224 / sizeof(type);
        i -= 224;
      }
      if (i > 112) {
        seed = rapid_ct_mix(rapid_ct_read64(p) ^ secret[0], rapid_ct_read64(p + 8 / sizeof(type)) ^ seed);
        see1 = rapid_ct_mix(rapid_ct_read64(p + 16 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 24 / sizeof(type)) ^ see1);
        see2 = rapid_ct_mix(rapid_ct_read64(p + 32 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 40 / sizeof(type)) ^ see2);
        see3 = rapid_ct_mix(rapid_ct_read64(p + 48 / sizeof(type)) ^ secret[3], rapid_ct_read64(p + 56 / sizeof(type)) ^ see3);
        see4 = rapid_ct_mix(rapid_ct_read64(p + 64 / sizeof(type)) ^ secret[4], rapid_ct_read64(p + 72 / sizeof(type)) ^ see4);
        see5 = rapid_ct_mix(rapid_ct_read64(p + 80 / sizeof(type)) ^ secret[5], rapid_ct_read64(p + 88 / sizeof(type)) ^ see5);
        see6 = rapid_ct_mix(rapid_ct_read64(p + 96 / sizeof(type)) ^ secret[6], rapid_ct_read64(p + 104 / sizeof(type)) ^ see6);
        p += 112 / sizeof(type);
        i -= 112;
      }
#endif
      seed ^= see1;
      see2 ^= see3;
      see4 ^= see5;
      seed ^= see6;
      see2 ^= see4;
      seed ^= see2;
    }
    if (i > 16) {
      seed = rapid_ct_mix(rapid_ct_read64(p) ^ secret[2], rapid_ct_read64(p + 8 / sizeof(type)) ^ seed);
      if (i > 32) {
          seed = rapid_ct_mix(rapid_ct_read64(p + 16 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 24 / sizeof(type)) ^ seed);
          if (i > 48) {
              seed = rapid_ct_mix(rapid_ct_read64(p + 32 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 40 / sizeof(type)) ^ seed);
              if (i > 64) {
                  seed = rapid_ct_mix(rapid_ct_read64(p + 48 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 56 / sizeof(type)) ^ seed);
                  if (i > 80) {
                      seed = rapid_ct_mix(rapid_ct_read64(p + 64 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 72 / sizeof(type)) ^ seed);
                      if (i > 96) {
                          seed = rapid_ct_mix(rapid_ct_read64(p + 80 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 88 / sizeof(type)) ^ seed);
                      }
                  }
              }
          }
      }
    }
    a=rapid_ct_read64(p+(i-16)/sizeof(type)) ^ i;  b=rapid_ct_read64(p+(i-8)/sizeof(type));
  }
  a ^= secret[1];
  b ^= seed;
  rapid_ct_mum(a, b);
  return rapid_ct_mix(a ^ secret[7], b ^ secret[1] ^ i);
}

 /*
  *  rapidhash_ct_Micro main function.
  *
  *  @param key     Buffer to be hashed.
  *  @param len     @key length, in bytes.
  *  @param seed    64-bit seed used to alter the hash result predictably.
  *  @param secret  Triplet of 64-bit secrets used to alter hash result predictably.
  *
  *  Returns a 64-bit hash.
  */
  template<typename type>
  RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhash_ct_Micro_internal(const type *key, size_t len, uint64_t seed, const uint64_t* secret) RAPIDHASH_CT_NOEXCEPT {
    const type *p=(const type *)key;
    seed ^= rapid_ct_mix(seed ^ secret[2], secret[1]);
    uint64_t a=0, b=0;
    size_t i = len;
    if (_likely_(len <= 16)) {
      if (len >= 4) {
        seed ^= len;
        if (len >= 8) {
          const type* plast = p + (len - 8) / sizeof(type);
          a = rapid_ct_read64(p);
          b = rapid_ct_read64(plast);
        } else {
          const type* plast = p + (len - 4) / sizeof(type);
          a = rapid_ct_read32(p);
          b = rapid_ct_read32(plast);
        }
      } else if (len > 0) {
        a = (rapid_ct_get_byte(p, 0) << 45) | rapid_ct_get_byte(p, len - 1);
        b = rapid_ct_get_byte(p, len >> 1);
      } else
        a = b = 0;
    } else {
      if (i > 80) {
        uint64_t see1 = seed, see2 = seed;
        uint64_t see3 = seed, see4 = seed;
        do {
          seed = rapid_ct_mix(rapid_ct_read64(p) ^ secret[0], rapid_ct_read64(p + 8 / sizeof(type)) ^ seed);
          see1 = rapid_ct_mix(rapid_ct_read64(p + 16 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 24 / sizeof(type)) ^ see1);
          see2 = rapid_ct_mix(rapid_ct_read64(p + 32 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 40 / sizeof(type)) ^ see2);
          see3 = rapid_ct_mix(rapid_ct_read64(p + 48 / sizeof(type)) ^ secret[3], rapid_ct_read64(p + 56 / sizeof(type)) ^ see3);
          see4 = rapid_ct_mix(rapid_ct_read64(p + 64 / sizeof(type)) ^ secret[4], rapid_ct_read64(p + 72 / sizeof(type)) ^ see4);
          p += 80 / sizeof(type);
          i -= 80;
        } while(i > 80);
        seed ^= see1;
        see2 ^= see3;
        seed ^= see4;
        seed ^= see2;
      }
      if (i > 16) {
        seed = rapid_ct_mix(rapid_ct_read64(p) ^ secret[2], rapid_ct_read64(p + 8 / sizeof(type)) ^ seed);
        if (i > 32) {
            seed = rapid_ct_mix(rapid_ct_read64(p + 16 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 24 / sizeof(type)) ^ seed);
            if (i > 48) {
                seed = rapid_ct_mix(rapid_ct_read64(p + 32 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 40 / sizeof(type)) ^ seed);
                if (i > 64) {
                    seed = rapid_ct_mix(rapid_ct_read64(p + 48 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 56 / sizeof(type)) ^ seed);
                }
            }
        }
      }
      a=rapid_ct_read64(p+(i-16)/sizeof(type)) ^ i;  b=rapid_ct_read64(p+(i-8)/sizeof(type));
    }
    a ^= secret[1];
    b ^= seed;
    rapid_ct_mum(a, b);
    return rapid_ct_mix(a ^ secret[7], b ^ secret[1] ^ i);
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
  template<typename type>
  RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhashNano_internal(const type *key, size_t len, uint64_t seed, const uint64_t* secret) RAPIDHASH_CT_NOEXCEPT {
    const type *p=(const type *)key;
    seed ^= rapid_ct_mix(seed ^ secret[2], secret[1]);
    uint64_t a=0, b=0;
    size_t i = len;
    if (_likely_(len <= 16)) {
      if (len >= 4) {
        seed ^= len;
        if (len >= 8) {
          const type* plast = p + (len - 8) / sizeof(type);
          a = rapid_ct_read64(p);
          b = rapid_ct_read64(plast);
        } else {
          const type* plast = p + (len - 4) / sizeof(type);
          a = rapid_ct_read32(p);
          b = rapid_ct_read32(plast);
        }
      } else if (len > 0) {
        a = (rapid_ct_get_byte(p, 0) << 45) | rapid_ct_get_byte(p, len - 1);
        b = rapid_ct_get_byte(p, len >> 1);
      } else
        a = b = 0;
    } else {
      if (i > 48) {
        uint64_t see1 = seed, see2 = seed;
        do {
          seed = rapid_ct_mix(rapid_ct_read64(p) ^ secret[0], rapid_ct_read64(p + 8 / sizeof(type)) ^ seed);
          see1 = rapid_ct_mix(rapid_ct_read64(p + 16 / sizeof(type)) ^ secret[1], rapid_ct_read64(p + 24 / sizeof(type)) ^ see1);
          see2 = rapid_ct_mix(rapid_ct_read64(p + 32 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 40 / sizeof(type)) ^ see2);
          p += 48 / sizeof(type);
          i -= 48;
        } while(i > 48);
        seed ^= see1;
        seed ^= see2;
      }
      if (i > 16) {
        seed = rapid_ct_mix(rapid_ct_read64(p) ^ secret[2], rapid_ct_read64(p + 8 / sizeof(type)) ^ seed);
        if (i > 32) {
            seed = rapid_ct_mix(rapid_ct_read64(p + 16 / sizeof(type)) ^ secret[2], rapid_ct_read64(p + 24 / sizeof(type)) ^ seed);
        }
      }
      a=rapid_ct_read64(p+(i-16)/sizeof(type)) ^ i;  b=rapid_ct_read64(p+(i-8)/sizeof(type));
    }
    a ^= secret[1];
    b ^= seed;
    rapid_ct_mum(a, b);
    return rapid_ct_mix(a ^ secret[7], b ^ secret[1] ^ i);
  }
 
/*
 *  rapidhash seeded hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *
 *  Calls rapidhash_ct_internal using provided parameters and default secrets.
 *
 *  Returns a 64-bit hash.
 */
template<typename type>
RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhash_ct_withSeed(const type *key, size_t len, uint64_t seed) RAPIDHASH_CT_NOEXCEPT {
  return rapidhash_ct_internal(key, len, seed, rapid_ct_secret);
}
 
/*
 *  rapidhash general purpose hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *
 *  Calls rapidhash_ct_withSeed using provided parameters and the default seed.
 *
 *  Returns a 64-bit hash.
 */
template<typename type>
RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhash_ct(const type *key, size_t len) RAPIDHASH_CT_NOEXCEPT {
  return rapidhash_ct_withSeed(key, len, 0);
}

/*
 *  rapidhash_ct_Micro seeded hash function.
 *
 *  Designed for HPC and server applications, where cache misses make a noticeable performance detriment.
 *  Clang-18+ compiles it to ~140 instructions without stack usage, both on x86-64 and aarch64.
 *  Faster for sizes up to 512 bytes, just 15%-20% slower for inputs above 1kb.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *
 *  Calls rapidhash_ct_internal using provided parameters and default secrets.
 *
 *  Returns a 64-bit hash.
 */
 template<typename type>
 RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhash_ct_Micro_withSeed(const type *key, size_t len, uint64_t seed) RAPIDHASH_CT_NOEXCEPT {
  return rapidhash_ct_Micro_internal(key, len, seed, rapid_ct_secret);
}
 
/*
 *  rapidhash_ct_Micro hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *
 *  Calls rapidhash_ct_withSeed using provided parameters and the default seed.
 *
 *  Returns a 64-bit hash.
 */
template<typename type>
RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhash_ct_Micro(const type *key, size_t len) RAPIDHASH_CT_NOEXCEPT {
  return rapidhash_ct_Micro_withSeed(key, len, 0);
}

/*
 *  rapidhashNano seeded hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *
 *  Calls rapidhash_ct_internal using provided parameters and default secrets.
 *
 *  Returns a 64-bit hash.
 */
 template<typename type>
 RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhash_ct_Nano_withSeed(const type *key, size_t len, uint64_t seed) RAPIDHASH_CT_NOEXCEPT {
  return rapidhash_ct_Nano_internal(key, len, seed, rapid_ct_secret);
}
 
/*
 *  rapidhashNano hash function.
 *
 *  Designed for Mobile and embedded applications, where keeping a small code size is a top priority.
 *  Clang-18+ compiles it to less than 100 instructions without stack usage, both on x86-64 and aarch64.
 *  The fastest for sizes up to 48 bytes, but may be considerably slower for larger inputs.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *
 *  Calls rapidhash_ct_withSeed using provided parameters and the default seed.
 *
 *  Returns a 64-bit hash.
 */
template<typename type>
RAPIDHASH_CT_INLINE_CONSTEVAL uint64_t rapidhash_ct_Nano(const type *key, size_t len) RAPIDHASH_CT_NOEXCEPT {
  return rapidhash_ct_Nano_withSeed(key, len, 0);
}