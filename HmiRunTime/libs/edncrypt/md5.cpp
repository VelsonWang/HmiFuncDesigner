#include "md5.h"
 
 #define ROTL32(dword, n) ((dword) << (n) ^ ((dword) >> (32 - (n))))
 #define ROTR32(dword, n) ((dword) >> (n) ^ ((dword) << (32 - (n))))
 #define ROTL64(qword, n) ((qword) << (n) ^ ((qword) >> (64 - (n))))
 #define ROTR64(qword, n) ((qword) >> (n) ^ ((qword) << (64 - (n))))
 
#define MD5_F(x, y, z) ((((y) ^ (z)) & (x)) ^ (z))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))
#define MD5_ROUND1(a, b, c, d, x, s, ac) { \
	(a) += MD5_F((b), (c), (d)) + (x) + (ac); \
	(a) = ROTL32((a), (s)); \
	(a) += (b); \
	 }
#define MD5_ROUND2(a, b, c, d, x, s, ac) { \
	(a) += MD5_G((b), (c), (d)) + (x) + (ac); \
	(a) = ROTL32((a), (s)); \
	(a) += (b); \
	 }
#define MD5_ROUND3(a, b, c, d, x, s, ac) { \
	(a) += MD5_H((b), (c), (d)) + (x) + (ac); \
	(a) = ROTL32((a), (s)); \
	(a) += (b); \
	 }
#define MD5_ROUND4(a, b, c, d, x, s, ac) { \
	(a) += MD5_I((b), (c), (d)) + (x) + (ac); \
	(a) = ROTL32((a), (s)); \
	(a) += (b); \
	 } 

md5::md5()
{

}

md5::~md5()
{

}
//初始化MD5的内容
void md5::md5_init(md5_ctx *ctx)
 {
     ctx->length_ = 0;
     ctx->unprocessed_ = 0;
 
     /* initialize state */
     ctx->hash_[0] = 0x67452301;
     ctx->hash_[1] = 0xefcdab89;
     ctx->hash_[2] = 0x98badcfe;
     ctx->hash_[3] = 0x10325476;
 }
 
 void * md5::swap_uint32_memcpy(void *to, const void *from, size_t length)
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
内部函数，将64个字节，16个uint32_t的数组进行摘要（杂凑）处理，处理的数据自己序是小头数据
state 存放处理的hash数据结果
block 要处理的block，64个字节，16个uint32_t的数组
 */
void md5::md5_process_block(uint32_t state[4], const uint32_t block[MD5_BLOCK_SIZE / 4])
 {
     register unsigned a, b, c, d;
     a = state[0];
     b = state[1];
     c = state[2];
     d = state[3];
 
     const uint32_t *x = NULL;
 
     //MD5里面计算的数据都是小头数据.大头党的数据要处理
 #if BYTES_ORDER == LITTLE_ENDIAN
     x = block;
 #else
     uint32_t swap_block[MD5_BLOCK_SIZE / 4];
     swap_uint32_memcpy(swap_block, block, 64);
     x = swap_block;
 #endif
 
     MD5_ROUND1(a, b, c, d, x[ 0],  7, 0xd76aa478);
     MD5_ROUND1(d, a, b, c, x[ 1], 12, 0xe8c7b756);
     MD5_ROUND1(c, d, a, b, x[ 2], 17, 0x242070db);
     MD5_ROUND1(b, c, d, a, x[ 3], 22, 0xc1bdceee);
     MD5_ROUND1(a, b, c, d, x[ 4],  7, 0xf57c0faf);
     MD5_ROUND1(d, a, b, c, x[ 5], 12, 0x4787c62a);
     MD5_ROUND1(c, d, a, b, x[ 6], 17, 0xa8304613);
     MD5_ROUND1(b, c, d, a, x[ 7], 22, 0xfd469501);
     MD5_ROUND1(a, b, c, d, x[ 8],  7, 0x698098d8);
     MD5_ROUND1(d, a, b, c, x[ 9], 12, 0x8b44f7af);
     MD5_ROUND1(c, d, a, b, x[10], 17, 0xffff5bb1);
     MD5_ROUND1(b, c, d, a, x[11], 22, 0x895cd7be);
     MD5_ROUND1(a, b, c, d, x[12],  7, 0x6b901122);
     MD5_ROUND1(d, a, b, c, x[13], 12, 0xfd987193);
     MD5_ROUND1(c, d, a, b, x[14], 17, 0xa679438e);
     MD5_ROUND1(b, c, d, a, x[15], 22, 0x49b40821);
 
     MD5_ROUND2(a, b, c, d, x[ 1],  5, 0xf61e2562);
     MD5_ROUND2(d, a, b, c, x[ 6],  9, 0xc040b340);
     MD5_ROUND2(c, d, a, b, x[11], 14, 0x265e5a51);
     MD5_ROUND2(b, c, d, a, x[ 0], 20, 0xe9b6c7aa);
     MD5_ROUND2(a, b, c, d, x[ 5],  5, 0xd62f105d);
     MD5_ROUND2(d, a, b, c, x[10],  9,  0x2441453);
     MD5_ROUND2(c, d, a, b, x[15], 14, 0xd8a1e681);
     MD5_ROUND2(b, c, d, a, x[ 4], 20, 0xe7d3fbc8);
     MD5_ROUND2(a, b, c, d, x[ 9],  5, 0x21e1cde6);
     MD5_ROUND2(d, a, b, c, x[14],  9, 0xc33707d6);
     MD5_ROUND2(c, d, a, b, x[ 3], 14, 0xf4d50d87);
     MD5_ROUND2(b, c, d, a, x[ 8], 20, 0x455a14ed);
     MD5_ROUND2(a, b, c, d, x[13],  5, 0xa9e3e905);
     MD5_ROUND2(d, a, b, c, x[ 2],  9, 0xfcefa3f8);
     MD5_ROUND2(c, d, a, b, x[ 7], 14, 0x676f02d9);
     MD5_ROUND2(b, c, d, a, x[12], 20, 0x8d2a4c8a);
 
     MD5_ROUND3(a, b, c, d, x[ 5],  4, 0xfffa3942);
     MD5_ROUND3(d, a, b, c, x[ 8], 11, 0x8771f681);
     MD5_ROUND3(c, d, a, b, x[11], 16, 0x6d9d6122);
     MD5_ROUND3(b, c, d, a, x[14], 23, 0xfde5380c);
     MD5_ROUND3(a, b, c, d, x[ 1],  4, 0xa4beea44);
     MD5_ROUND3(d, a, b, c, x[ 4], 11, 0x4bdecfa9);
     MD5_ROUND3(c, d, a, b, x[ 7], 16, 0xf6bb4b60);
     MD5_ROUND3(b, c, d, a, x[10], 23, 0xbebfbc70);
     MD5_ROUND3(a, b, c, d, x[13],  4, 0x289b7ec6);
     MD5_ROUND3(d, a, b, c, x[ 0], 11, 0xeaa127fa);
     MD5_ROUND3(c, d, a, b, x[ 3], 16, 0xd4ef3085);
     MD5_ROUND3(b, c, d, a, x[ 6], 23,  0x4881d05);
     MD5_ROUND3(a, b, c, d, x[ 9],  4, 0xd9d4d039);
     MD5_ROUND3(d, a, b, c, x[12], 11, 0xe6db99e5);
     MD5_ROUND3(c, d, a, b, x[15], 16, 0x1fa27cf8);
     MD5_ROUND3(b, c, d, a, x[ 2], 23, 0xc4ac5665);
 
     MD5_ROUND4(a, b, c, d, x[ 0],  6, 0xf4292244);
     MD5_ROUND4(d, a, b, c, x[ 7], 10, 0x432aff97);
     MD5_ROUND4(c, d, a, b, x[14], 15, 0xab9423a7);
     MD5_ROUND4(b, c, d, a, x[ 5], 21, 0xfc93a039);
     MD5_ROUND4(a, b, c, d, x[12],  6, 0x655b59c3);
     MD5_ROUND4(d, a, b, c, x[ 3], 10, 0x8f0ccc92);
     MD5_ROUND4(c, d, a, b, x[10], 15, 0xffeff47d);
     MD5_ROUND4(b, c, d, a, x[ 1], 21, 0x85845dd1);
     MD5_ROUND4(a, b, c, d, x[ 8],  6, 0x6fa87e4f);
     MD5_ROUND4(d, a, b, c, x[15], 10, 0xfe2ce6e0);
     MD5_ROUND4(c, d, a, b, x[ 6], 15, 0xa3014314);
     MD5_ROUND4(b, c, d, a, x[13], 21, 0x4e0811a1);
     MD5_ROUND4(a, b, c, d, x[ 4],  6, 0xf7537e82);
     MD5_ROUND4(d, a, b, c, x[11], 10, 0xbd3af235);
     MD5_ROUND4(c, d, a, b, x[ 2], 15, 0x2ad7d2bb);
     MD5_ROUND4(b, c, d, a, x[ 9], 21, 0xeb86d391);
 
     state[0] += a;
     state[1] += b;
     state[2] += c;
     state[3] += d;
 }
 
 
 /*!
内部函数，处理数据的前面部分(>64字节的部分)，每次组成一个64字节的block就进行杂凑处理
ctx  算法的context，用于记录一些处理的上下文和结果
buf  处理的数据，
size 处理的数据长度
 */
void md5::md5_update(md5_ctx *ctx, const unsigned char *buf, size_t size)
 {
     //为什么不是=，因为在某些环境下，可以多次调用zen_md5_update，但这种情况，必须保证前面的调用，每次都没有unprocessed_
     ctx->length_ += size;
 
     //每个处理的块都是64字节
     while (size >= MD5_BLOCK_SIZE)
     {
         md5_process_block(ctx->hash_, reinterpret_cast<const uint32_t *>(buf));
         buf  += MD5_BLOCK_SIZE;
         size -= MD5_BLOCK_SIZE;
     }
 
     ctx->unprocessed_ = size;
 }
 
 
 /*!
内部函数，处理数据的末尾部分，我们要拼出最后1个（或者两个）要处理的BLOCK，加上0x80，加上长度进行处理
ctx    算法的context，用于记录一些处理的上下文和结果
buf    处理的数据
size   处理buffer的长度
result 返回的结果，
 */
 void md5::md5_final(md5_ctx *ctx, const unsigned char *buf, size_t size, unsigned char *result)
 {
     uint32_t message[MD5_BLOCK_SIZE / 4];
 
     //保存剩余的数据，我们要拼出最后1个（或者两个）要处理的块，前面的算法保证了，最后一个块肯定小于64个字节
     if (ctx->unprocessed_)
     {
         memcpy(message, buf + size - ctx->unprocessed_, static_cast<size_t>( ctx->unprocessed_));
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
 
         md5_process_block(ctx->hash_, message);
         index = 0;
     }
 
     //补0
     while (index < 14)
     {
         message[index++] = 0;
     }
 
     //保存长度，注意是bit位的长度,这个问题让我看着郁闷了半天，
     uint64_t data_len = (ctx->length_) << 3;
 
     //注意MD5算法要求的64bit的长度是小头LITTLE-ENDIAN编码，注意下面的比较是!=
 #if BYTES_ORDER != LITTLE_ENDIAN
     data_len = SWAP_UINT64(data_len);
 #endif
 
     message[14] = (uint32_t) (data_len & 0x00000000FFFFFFFF);
     message[15] = (uint32_t) ((data_len & 0xFFFFFFFF00000000ULL) >> 32);
 
     md5_process_block(ctx->hash_, message);
 
     //注意结果是小头党的，在大头的世界要进行转换
 #if BYTES_ORDER == LITTLE_ENDIAN
     memcpy(result, &ctx->hash_, MD5_HASH_SIZE);
 #else
     swap_uint32_memcpy(result, &ctx->hash_, MD5_HASH_SIZE);
 #endif
 
 }
 
 //计算一个内存数据的MD5值
 unsigned char *md5::Encrypt(const unsigned char *buf, size_t size, unsigned char result[16])
 {
     assert(result != NULL);
     md5_ctx ctx;
     md5_init(&ctx);
     md5_update(&ctx, buf, size);
     md5_final(&ctx, buf, size, result);
     return result;
 }
 
