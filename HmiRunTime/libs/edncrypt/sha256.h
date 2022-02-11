#ifndef SHA256_H
#define SHA256_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define SHA1_HASH_SIZE 32
#define SHA1_HASH_WORDS 8

typedef struct _SHA1Context {
	uint64_t totalLength;
	uint32_t hash[SHA1_HASH_WORDS];
	uint32_t bufferLength;
	union {
		uint32_t words[16];
		uint8_t bytes[64];
	} buffer;
}SHA1Context;

class sha256
{
public:
	sha256();
	~sha256();
	unsigned char *Encrypt(const unsigned char *buf,size_t size,unsigned char *result);
private:
	

	void SHA256Init (SHA1Context *sc);
	void SHA256Update (SHA1Context *sc, const void *data, uint32_t len);
	void SHA256Final (SHA1Context *sc, uint8_t *hash);
};

#endif // SHA256_H
