#include "sha256.h"

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define Ch(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define Maj(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define SIGMA0(x) (ROTR((x), 2) ^ ROTR((x), 13) ^ ROTR((x), 22))
#define SIGMA1(x) (ROTR((x), 6) ^ ROTR((x), 11) ^ ROTR((x), 25))
#define sigma0(x) (ROTR((x), 7) ^ ROTR((x), 18) ^ ((x) >> 3))
#define sigma1(x) (ROTR((x), 17) ^ ROTR((x), 19) ^ ((x) >> 10))

#ifdef WORDS_BIGENDIAN

#define BYTESWAP(x) (x)
#define BYTESWAP64(x) (x)

#else /* WORDS_BIGENDIAN */

#define BYTESWAP(x) ((ROTR((x), 8) & 0xff00ff00L) | \
	(ROTL((x), 8) & 0x00ff00ffL))
#define BYTESWAP64(x) _byteswap64(x)

static inline uint64_t _byteswap64(uint64_t x)
{
	uint32_t a = x >> 32;
	uint32_t b = (uint32_t) x;
	return ((uint64_t) BYTESWAP(b) << 32) | (uint64_t) BYTESWAP(a);
}
#endif /* WORDS_BIGENDIAN */

#define DO_ROUND() { \
	t1 = h + SIGMA1(e) + Ch(e, f, g) + *(Kp++) + *(W++); \
	t2 = SIGMA0(a) + Maj(a, b, c); \
	h = g; \
	g = f; \
	f = e; \
	e = d + t1; \
	d = c; \
	c = b; \
	b = a; \
	a = t1 + t2; \
}

static const uint32_t K[64] = {
	0x428a2f98L, 0x71374491L, 0xb5c0fbcfL, 0xe9b5dba5L,
	0x3956c25bL, 0x59f111f1L, 0x923f82a4L, 0xab1c5ed5L,
	0xd807aa98L, 0x12835b01L, 0x243185beL, 0x550c7dc3L,
	0x72be5d74L, 0x80deb1feL, 0x9bdc06a7L, 0xc19bf174L,
	0xe49b69c1L, 0xefbe4786L, 0x0fc19dc6L, 0x240ca1ccL,
	0x2de92c6fL, 0x4a7484aaL, 0x5cb0a9dcL, 0x76f988daL,
	0x983e5152L, 0xa831c66dL, 0xb00327c8L, 0xbf597fc7L,
	0xc6e00bf3L, 0xd5a79147L, 0x06ca6351L, 0x14292967L,
	0x27b70a85L, 0x2e1b2138L, 0x4d2c6dfcL, 0x53380d13L,
	0x650a7354L, 0x766a0abbL, 0x81c2c92eL, 0x92722c85L,
	0xa2bfe8a1L, 0xa81a664bL, 0xc24b8b70L, 0xc76c51a3L,
	0xd192e819L, 0xd6990624L, 0xf40e3585L, 0x106aa070L,
	0x19a4c116L, 0x1e376c08L, 0x2748774cL, 0x34b0bcb5L,
	0x391c0cb3L, 0x4ed8aa4aL, 0x5b9cca4fL, 0x682e6ff3L,
	0x748f82eeL, 0x78a5636fL, 0x84c87814L, 0x8cc70208L,
	0x90befffaL, 0xa4506cebL, 0xbef9a3f7L, 0xc67178f2L
};

static const uint8_t padding[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

sha256::sha256()
{

}

sha256::~sha256()
{

}
static void	burnStack (int size)
{
	char buf[128];

	memset (buf, 0, sizeof (buf));
	size -= sizeof (buf);
	if (size > 0)
		burnStack (size);
}

static void	SHA1Guts(SHA1Context *sc, const uint32_t *cbuf)
{
	uint32_t buf[64];
	uint32_t *W, *W2, *W7, *W15, *W16;
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t t1, t2;
	const uint32_t *Kp;
	int i;

	W = buf;

	for (i = 15; i >= 0; i--) {
		*(W++) = BYTESWAP(*cbuf);
		cbuf++;
	}

	W16 = &buf[0];
	W15 = &buf[1];
	W7 = &buf[9];
	W2 = &buf[14];

	for (i = 47; i >= 0; i--) {
		*(W++) = sigma1(*W2) + *(W7++) + sigma0(*W15) + *(W16++);
		W2++;
		W15++;
	}

	a = sc->hash[0];
	b = sc->hash[1];
	c = sc->hash[2];
	d = sc->hash[3];
	e = sc->hash[4];
	f = sc->hash[5];
	g = sc->hash[6];
	h = sc->hash[7];

	Kp = K;
	W = buf;

	for (i = 63; i >= 0; i--)
		DO_ROUND();

	sc->hash[0] += a;
	sc->hash[1] += b;
	sc->hash[2] += c;
	sc->hash[3] += d;
	sc->hash[4] += e;
	sc->hash[5] += f;
	sc->hash[6] += g;
	sc->hash[7] += h;
}

void sha256::SHA256Init (SHA1Context *sc)
{
	sc->totalLength = 0LL;
	sc->hash[0] = 0x6a09e667L;
	sc->hash[1] = 0xbb67ae85L;
	sc->hash[2] = 0x3c6ef372L;
	sc->hash[3] = 0xa54ff53aL;
	sc->hash[4] = 0x510e527fL;
	sc->hash[5] = 0x9b05688cL;
	sc->hash[6] = 0x1f83d9abL;
	sc->hash[7] = 0x5be0cd19L;
	sc->bufferLength = 0L;
}

void sha256::SHA256Update (SHA1Context *sc, const void *vdata, uint32_t len)
{
	const uint8_t *data = (const uint8_t *)vdata;
	uint32_t bufferBytesLeft;
	uint32_t bytesToCopy;
	int needBurn = 0;

	while (len) {
		bufferBytesLeft = 64L - sc->bufferLength;

		bytesToCopy = bufferBytesLeft;
		if (bytesToCopy > len)
			bytesToCopy = len;

		memcpy (&sc->buffer.bytes[sc->bufferLength], data, bytesToCopy);

		sc->totalLength += bytesToCopy * 8L;

		sc->bufferLength += bytesToCopy;
		data += bytesToCopy;
		len -= bytesToCopy;

		if (sc->bufferLength == 64L) {
			SHA1Guts (sc, sc->buffer.words);
			needBurn = 1;
			sc->bufferLength = 0L;
		}
	}

	if (needBurn)
		burnStack (sizeof (uint32_t[74]) + sizeof (uint32_t *[6]) + sizeof (int));
}

void sha256::SHA256Final (SHA1Context *sc, uint8_t *hash)
{
	uint32_t bytesToPad;
	uint64_t lengthPad;
	int i;

	bytesToPad = 120L - sc->bufferLength;
	if (bytesToPad > 64L)
		bytesToPad -= 64L;

	lengthPad = BYTESWAP64(sc->totalLength);

	SHA256Update (sc, padding, bytesToPad);
	SHA256Update (sc, &lengthPad, 8L);

	if (hash) {
		for (i = 0; i < SHA1_HASH_WORDS; i++) {
			hash[0] = (uint8_t) (sc->hash[i] >> 24);
			hash[1] = (uint8_t) (sc->hash[i] >> 16);
			hash[2] = (uint8_t) (sc->hash[i] >> 8);
			hash[3] = (uint8_t) sc->hash[i];
			hash += 4;
		}
	}
}

//计算一个内存数据的SHA1值
unsigned char *sha256::Encrypt(const unsigned char *msg,size_t size,unsigned char *result)
{
	SHA1Context ctx;
	SHA256Init(&ctx);
	SHA256Update(&ctx, msg, size);
	SHA256Final(&ctx, result);
	return result;
}
