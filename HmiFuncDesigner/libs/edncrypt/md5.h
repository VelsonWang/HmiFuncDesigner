#ifndef MD5_H1
#define MD5_H1

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

static const size_t MD5_HASH_SIZE   = 16;
static const size_t MD5_BLOCK_SIZE = 64;

typedef struct md5_ctx
{
	//处理的数据的长度
	uint64_t length_;
	//还没有处理的数据长度
	uint64_t unprocessed_;
	//取得的HASH结果（中间数据）
	uint32_t  hash_[4];
} md5_ctx;

class md5
{
public:
	md5();
	~md5();
	unsigned char *Encrypt(const unsigned char *buf,size_t size,unsigned char result[16]);
	
private:
	 void md5_init(md5_ctx *ctx);
	 void *swap_uint32_memcpy(void *to, const void *from, size_t length);
	 void md5_process_block(uint32_t state[4], const uint32_t block[MD5_BLOCK_SIZE / 4]);
	 void md5_update(md5_ctx *ctx, const unsigned char *buf, size_t size);
	 void md5_final(md5_ctx *ctx, const unsigned char *buf, size_t size, unsigned char *result);
};

#endif // MD5_H1
