#ifndef SHA1_H
#define SHA1_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define LITTLE_ENDIAN  0x0123
#define BIG_ENDIAN     0x3210

#ifndef BYTES_ORDER
#define BYTES_ORDER    LITTLE_ENDIAN
#endif

#ifndef SWAP_UINT16
#define SWAP_UINT16(x)  ((((x) & 0xff00) >>  8) | (((x) & 0x00ff) <<  8))
#endif
#ifndef SWAP_UINT32
#define SWAP_UINT32(x)  ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
	(((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#endif
#ifndef SWAP_UINT64
#define SWAP_UINT64(x)  ((((x) & 0xff00000000000000) >> 56) | (((x) & 0x00ff000000000000) >>  40) | \
	(((x) & 0x0000ff0000000000) >> 24) | (((x) & 0x000000ff00000000) >>  8) | \
	(((x) & 0x00000000ff000000) << 8 ) | (((x) & 0x0000000000ff0000) <<  24) | \
	(((x) & 0x000000000000ff00) << 40 ) | (((x) & 0x00000000000000ff) <<  56))
#endif

static const size_t SHA1_HASH_SIZE  = 20;
static const size_t SHA1_BLOCK_SIZE = 64;
typedef struct sha1_ctx
{
	//处理的数据的长度
	uint64_t length_;
	//还没有处理的数据长度
	uint64_t unprocessed_;
	/* 160-bit algorithm internal hashing state */
	uint32_t hash_[5];
} sha1_ctx;

class sha1
{
public:
	sha1();
	~sha1();
	unsigned char *Encrypt(const unsigned char *buf,size_t size,unsigned char *result);

private:
	void sha1_init(sha1_ctx *ctx);
	void *swap_uint32_memcpy(void *to, const void *from, size_t length);
	void sha1_process_block(uint32_t hash[5],const uint32_t block[SHA1_BLOCK_SIZE / 4]);
	void sha1_update(sha1_ctx *ctx,const unsigned char *buf, size_t size);
	void sha1_final(sha1_ctx *ctx,const unsigned char *msg,size_t size,unsigned char *result);
};

#endif // SHA1_H
