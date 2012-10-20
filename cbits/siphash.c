/* Almost a verbatim copy of the reference implementation. */

#include <stddef.h>
#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#define ROTL(x,b) (u64)(((x) << (b)) | ((x) >> (64 - (b))))

#define SIPROUND \
    do { \
	v0 += v1; v1=ROTL(v1,13); v1 ^= v0; v0=ROTL(v0,32); \
	v2 += v3; v3=ROTL(v3,16); v3 ^= v2; \
	v0 += v3; v3=ROTL(v3,21); v3 ^= v0; \
	v2 += v1; v1=ROTL(v1,17); v1 ^= v2; v2=ROTL(v2,32); \
    } while(0)

#if defined(__i386)
# define _siphash24 plain_siphash24
#endif

static inline u64 _siphash(int c, int d, u64 k0, u64 k1,
			   const u8 *str, size_t len)
{
    u64 v0 = 0x736f6d6570736575ull ^ k0;
    u64 v1 = 0x646f72616e646f6dull ^ k1;
    u64 v2 = 0x6c7967656e657261ull ^ k0;
    u64 v3 = 0x7465646279746573ull ^ k1;
    u64 b = ((u64) len) << 56;
    const u8 *end, *p;
    int i;

    for (p = str, end = str + (len & ~7); p < end; p += 8) {
	u64 m = *(u64 *) p;
	v3 ^= m;
	if (c == 2) {
	    SIPROUND;
	    SIPROUND;
	} else {
	    for (i = 0; i < c; i++)
		SIPROUND;
	}
	v0 ^= m;
    }

    switch (len & 7) {
    case 7: b |= ((u64)p[6]) << 48;
    case 6: b |= ((u64)p[5]) << 40;
    case 5: b |= ((u64)p[4]) << 32;
    case 4: b |= ((u64)p[3]) << 24;
    case 3: b |= ((u64)p[2]) << 16;
    case 2: b |= ((u64)p[1]) <<  8;
    case 1: b |= ((u64)p[0]);
    }

    v3 ^= b;
    if (c == 2) {
	SIPROUND;
	SIPROUND;
    } else {
	for (i = 0; i < c; i++)
	    SIPROUND;
    }
    v0 ^= b;

    v2 ^= 0xff;
    if (d == 4) {
	SIPROUND;
	SIPROUND;
	SIPROUND;
	SIPROUND;
    } else {
	for (i = 0; i < d; i++)
	    SIPROUND;
    }
    b = v0 ^ v1 ^ v2  ^ v3;
    return b;
}


static inline u64 _siphash24(u64 k0, u64 k1, const u8 *str, size_t len)
{
    return _siphash(2, 4, k0, k1, str, len);
}

#if defined(__i386)
# undef _siphash24

static u64 (*_siphash24)(u64 k0, u64 k1, const u8 *, size_t);

u64 hashable_siphash24_sse41(u64 k0, u64 k1, const u8 *, size_t);

static void maybe_use_sse41()
    __attribute__((constructor));

static void maybe_use_sse41()
{
    uint32_t eax = 1, ebx, ecx, edx;

    __asm volatile
	("mov %%ebx, %%edi;" /* 32bit PIC: don't clobber ebx */
	 "cpuid;"
	 "mov %%ebx, %%esi;"
	 "mov %%edi, %%ebx;"
	 :"+a" (eax), "=S" (ebx), "=c" (ecx), "=d" (edx)
	 : :"edi");

    if (ecx & (1 << 19))
	_siphash24 = hashable_siphash24_sse41;
    else
	_siphash24 = plain_siphash24;
}

#endif

u64 hashable_siphash(int c, int d, u64 k0, u64 k1, const u8 *str, size_t len)
{
    return _siphash(c, d, k0, k1, str, len);
}

u64 hashable_siphash24(u64 k0, u64 k1, const u8 *str, size_t len)
{
    return _siphash24(k0, k1, str, len);
}

/* Used for ByteArray#s. We can't treat them like pointers in
   native Haskell, but we can in unsafe FFI calls.
 */
u64 hashable_siphash24_offset(u64 k0, u64 k1,
			      const u8 *str, size_t off, size_t len)
{
    return _siphash24(k0, k1, str + off, len);
}

u64 hashable_siphash24_u8(u64 k0, u64 k1, u8 key)
{
    return _siphash24(k0, k1, (const u8 *) &key, sizeof(u8));
}

u16 hashable_siphash24_u16(u64 k0, u64 k1, u16 key)
{
    return _siphash24(k0, k1, (const u8 *) &key, sizeof(u16));
}

u16 hashable_siphash24_u32(u64 k0, u64 k1, u32 key)
{
    return _siphash24(k0, k1, (const u8 *) &key, sizeof(u32));
}

u64 hashable_siphash24_u64(u64 k0, u64 k1, u64 key)
{
    return _siphash24(k0, k1, (const u8 *) &key, sizeof(u64));
}
