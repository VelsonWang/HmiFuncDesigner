#include "sha1.h"

#define ROTL32(dword, n) ((dword) << (n) ^ ((dword) >> (32 - (n))))
#define ROTR32(dword, n) ((dword) >> (n) ^ ((dword) << (32 - (n))))
#define ROTL64(qword, n) ((qword) << (n) ^ ((qword) >> (64 - (n))))
#define ROTR64(qword, n) ((qword) >> (n) ^ ((qword) << (64 - (n))))

sha1::sha1()
{

}

sha1::~sha1()
{

}
//SHA1算法的上下文的初始化
void sha1::sha1_init(sha1_ctx *ctx)
 {
     ctx->length_ = 0;
     ctx->unprocessed_ = 0;
     // 初始化算法的几个常量，魔术数
     ctx->hash_[0] = 0x67452301;
     ctx->hash_[1] = 0xefcdab89;
     ctx->hash_[2] = 0x98badcfe;
     ctx->hash_[3] = 0x10325476;
     ctx->hash_[4] = 0xc3d2e1f0;
 }
 
void * sha1::swap_uint32_memcpy(void *to, const void *from, size_t length)
{
	memcpy(to, from, length);
	size_t remain_len =  (4 - (length & 3)) & 3;

	//数据不是4字节的倍数,补充0
	if (remain_len)
	{
		for (size_t i = 0; i < remain_len; ++i)
		{
			*((char *)(to) + length + i) = 0;
		}
		//调整成4的倍数
		length += remain_len;
	}

	//所有的数据反转
	for (size_t i = 0; i < length / 4; ++i)
	{
		((uint32_t *)to)[i] = SWAP_UINT32(((uint32_t *)to)[i]);
	}

	return to;
}
 /*!
内部函数，对一个64bit内存块进行摘要(杂凑)处理，
hash  存放计算hash结果的的数组
block 要计算的处理得内存块
 */
void sha1::sha1_process_block(uint32_t hash[5],const uint32_t block[SHA1_BLOCK_SIZE / 4])
 {
     size_t        t;
     uint32_t      wblock[80];
     register uint32_t      a, b, c, d, e, temp;
 
     //SHA1算法处理的内部数据要求是大头党的，在小头的环境转换
 #if BYTES_ORDER == LITTLE_ENDIAN
     swap_uint32_memcpy(wblock, block, SHA1_BLOCK_SIZE);
 #else
     ::memcpy(wblock, block, ZEN_SHA1_BLOCK_SIZE);
 #endif
 
     //处理
     for (t = 16; t < 80; t++)
     {
         wblock[t] = ROTL32(wblock[t - 3] ^ wblock[t - 8] ^ wblock[t - 14] ^ wblock[t - 16], 1);
     }
 
     a = hash[0];
     b = hash[1];
     c = hash[2];
     d = hash[3];
     e = hash[4];
 
     for (t = 0; t < 20; t++)
     {
         /* the following is faster than ((B & C) | ((~B) & D)) */
         temp =  ROTL32(a, 5) + (((c ^ d) & b) ^ d)
                 + e + wblock[t] + 0x5A827999;
         e = d;
         d = c;
         c = ROTL32(b, 30);
         b = a;
         a = temp;
     }
 
     for (t = 20; t < 40; t++)
     {
         temp = ROTL32(a, 5) + (b ^ c ^ d) + e + wblock[t] + 0x6ED9EBA1;
         e = d;
         d = c;
         c = ROTL32(b, 30);
         b = a;
         a = temp;
     }
 
     for (t = 40; t < 60; t++)
     {
         temp = ROTL32(a, 5) + ((b & c) | (b & d) | (c & d))
                + e + wblock[t] + 0x8F1BBCDC;
         e = d;
         d = c;
         c = ROTL32(b, 30);
         b = a;
         a = temp;
     }
 
     for (t = 60; t < 80; t++)
     {
         temp = ROTL32(a, 5) + (b ^ c ^ d) + e + wblock[t] + 0xCA62C1D6;
         e = d;
         d = c;
         c = ROTL32(b, 30);
         b = a;
         a = temp;
     }
 
     hash[0] += a;
     hash[1] += b;
     hash[2] += c;
     hash[3] += d;
     hash[4] += e;
 }
 
 /*!
内部函数，处理数据的前面部分(>64字节的部分)，每次组成一个64字节的block就进行杂凑处理
ctx  算法的上下文，记录中间数据，结果等
msg  要进行计算的数据buffer
size 长度
 */
void sha1::sha1_update(sha1_ctx *ctx,const unsigned char *buf,size_t size)
 {
     //为了让sha1_update可以多次进入，长度可以累计
     ctx->length_ += size;
 
     //每个处理的块都是64字节
     while (size >= SHA1_BLOCK_SIZE)
     {
         sha1_process_block(ctx->hash_, reinterpret_cast<const uint32_t *>(buf));
         buf  += SHA1_BLOCK_SIZE;
         size -= SHA1_BLOCK_SIZE;
     }
 
     ctx->unprocessed_ = size;
 }
 
 
 /*!
内部函数，处理数据的最后部分，添加0x80,补0，增加长度信息
ctx    算法的上下文，记录中间数据，结果等
msg    要进行计算的数据buffer
result 返回的结果
 */
void sha1::sha1_final(sha1_ctx *ctx,const unsigned char *msg,size_t size, unsigned char *result)
 {
     uint32_t message[SHA1_BLOCK_SIZE / 4];
 
     //保存剩余的数据，我们要拼出最后1个（或者两个）要处理的块，前面的算法保证了，最后一个块肯定小于64个字节
     if (ctx->unprocessed_)
     {
         memcpy(message, msg + size - ctx->unprocessed_, static_cast<size_t>( ctx->unprocessed_));
     }
 
     //得到0x80要添加在的位置（在uint32_t 数组中），
     uint32_t index = ((uint32_t)ctx->length_ & 63) >> 2;
     uint32_t shift = ((uint32_t)ctx->length_ & 3) * 8;
 
     //添加0x80进去，并且把余下的空间补充0
     message[index]   &= ~(0xFFFFFFFF << shift);
     message[index++] ^= 0x80 << shift;
 
     //如果这个block还无法处理，其后面的长度无法容纳长度64bit，那么先处理这个block
     if (index > 14)
     {
         while (index < 16)
         {
             message[index++] = 0;
         }
 
         sha1_process_block(ctx->hash_, message);
         index = 0;
     }
 
     //补0
     while (index < 14)
     {
         message[index++] = 0;
     }
 
     //保存长度，注意是bit位的长度,这个问题让我看着郁闷了半天，
     uint64_t data_len = (ctx->length_) << 3;
 
     //注意SHA1算法要求的64bit的长度是大头BIG-ENDIAN，在小头的世界要进行转换
 #if BYTES_ORDER == LITTLE_ENDIAN
     data_len = SWAP_UINT64(data_len);
 #endif
 
     message[14] = (uint32_t) (data_len & 0x00000000FFFFFFFF);
     message[15] = (uint32_t) ((data_len & 0xFFFFFFFF00000000ULL) >> 32);
 
     sha1_process_block(ctx->hash_, message);
 
     //注意结果是大头党的，在小头的世界要进行转换
 #if BYTES_ORDER == LITTLE_ENDIAN
     swap_uint32_memcpy(result, &ctx->hash_, SHA1_HASH_SIZE);
 #else
     memcpy(result, &ctx->hash_, ZEN_SHA1_HASH_SIZE);
 #endif
 }

//计算一个内存数据的SHA1值
unsigned char *sha1::Encrypt(const unsigned char *msg,size_t size,unsigned char *result)
{
	assert(result != NULL);

	sha1_ctx ctx;
	sha1_init(&ctx);
	sha1_update(&ctx, msg, size);
	sha1_final(&ctx, msg, size, result);
	return result;
}
