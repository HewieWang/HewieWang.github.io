#include <stdio.h>
#include <string.h>
#include "md5.h"

//! 定义MD5状态数据结构类型
typedef struct{
	unsigned long int	state[4];		// 初始链接变量;保存16字节摘要
	unsigned long int	count[2];		// 明文位数
	unsigned char		PADDING[64];	// 填充位，最大64*8位
	unsigned char		buffer[64];		// 输入缓冲
}MD5_State;

//! MD5转换常量
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

//! F, G, H and I 基本MD5函数
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

//! 将x循环左移n位
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

//! 4轮运算中FF(第1轮), GG(第2轮), HH(第3轮), and II(第4轮)转换
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}

/******************************************************************************/
//	名称：MD5_memcpy
//	功能：输入输出字节拷贝
//  参数：output:	指向unsigned char类型输出缓冲区
//		  input:	指向unsigned char类型输入缓冲区
//		  len:		输入数据长度(字节)
//	返回：无

/******************************************************************************/
void MD5_memcpy( unsigned char *output, unsigned char *input, unsigned int len )
{
	unsigned int i;

	for( i = 0; i < len; i++ )
		output[i] = input[i];
}

/******************************************************************************/
//	名称：MD5_memset
//	功能：对MD5运算缓冲区中填充数据
//  参数：output:	指向unsigned char类型缓冲区
//		  value:	数据
//		  len:		填充大小
//	返回：无

/******************************************************************************/
void MD5_memset( unsigned char *output, int value, unsigned int len )
{
	unsigned int i;

	for( i = 0; i < len; i++ )
		((char *)output)[i] = (char)value;
}

/******************************************************************************/
//	名称：Encode
//	功能：数据类型轮换(unsigned long char -> unsigned char)
//  参数：output:	指向unsigned char类型输出缓冲区
//		  input:	指向unsigned long int类型输入缓冲区
//		  len:		输入数据长度(字节)
//	返回：无

/******************************************************************************/
void Encode( unsigned char *output, unsigned long int *input, unsigned int len )
{
	unsigned int i, j;

	for(i = 0, j = 0; j < len; i++, j += 4) 
	{
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

/******************************************************************************/
//	名称：Decode
//	功能：数据类型轮换(unsigned char -> unsigned long int)
//  参数：output:	指向unsigned long int类型输出缓冲区
//		  input:	指向unsigned char类型输入缓冲区
//		  len:		输入数据长度(字节)
//	返回：无

/******************************************************************************/
void Decode( unsigned long int *output, unsigned char *input, unsigned int len )
{
	unsigned int i, j;

	for( i=0, j=0; j<len; i++, j+=4 )
		output[i] = ((unsigned long int)input[j]) | (((unsigned long int)input[j+1]) << 8) |
		(((unsigned long int)input[j+2]) << 16) | (((unsigned long int)input[j+3]) << 24);
}

/******************************************************************************/
//	名称：MD5Init
//	功能：初始链接变量赋值；初始化填充位
//  参数：指向MD5状态数据变量
//	返回：无
//  备注：填充位第1位为1,其余位为0

/******************************************************************************/
void MD5_Init( MD5_State *s )
{
	s->count[0] = s->count[1] = 0;
	//! 初始链接变量
	s->state[0] = 0x67452301;
	s->state[1] = 0xefcdab89;
	s->state[2] = 0x98badcfe;
	s->state[3] = 0x10325476;

	//! 初始填充位(目标形式: 0x80000000......，共计512位)
	MD5_memset( s->PADDING, 0, sizeof(s->PADDING) );
	*(s->PADDING)=0x80;
	//  s->PADDING = {
	//	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	};
}

/******************************************************************************/
//	名称：MD5Transform
//	功能：MD5 4轮运算
//  参数：state: 链接变量；block: 子明文分组
//	返回：无
//  备注：4轮共计64步运算

/******************************************************************************/
void MD5_Transform( unsigned long int state[4], unsigned char block[64] )
{
	unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode( x, block, 64 );

	//! 第1轮

	FF (a, b, c, d, x[0], S11, 0xd76aa478); // 1
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); // 2
	FF (c, d, a, b, x[ 2], S13, 0x242070db); // 3
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); // 4
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); // 5
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); // 6
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); // 7
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); // 8
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); // 9
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); // 10
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); // 11
	FF (b, c, d, a, x[11], S14, 0x895cd7be); // 12
	FF (a, b, c, d, x[12], S11, 0x6b901122); // 13
	FF (d, a, b, c, x[13], S12, 0xfd987193); // 14
	FF (c, d, a, b, x[14], S13, 0xa679438e); // 15
	FF (b, c, d, a, x[15], S14, 0x49b40821); // 16

	//! 第2轮
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); // 17
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); // 18
	GG (c, d, a, b, x[11], S23, 0x265e5a51); // 19
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); // 20
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); // 21
	GG (d, a, b, c, x[10], S22,  0x2441453); // 22
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); // 23
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); // 24
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); // 25
	GG (d, a, b, c, x[14], S22, 0xc33707d6); // 26
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); // 27
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); // 28
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); // 29
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); // 30
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); // 31
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); // 32

	//! 第3轮
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); // 33
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); // 34
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); // 35
	HH (b, c, d, a, x[14], S34, 0xfde5380c); // 36
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); // 37
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); // 38
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); // 39
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); // 40
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); // 41
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); // 42
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); // 43
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); // 44
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); // 45
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); // 46
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); // 47
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); // 48

	//! 第4轮
	II (a, b, c, d, x[ 0], S41, 0xf4292244); // 49
	II (d, a, b, c, x[ 7], S42, 0x432aff97); // 50
	II (c, d, a, b, x[14], S43, 0xab9423a7); // 51
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); // 52
	II (a, b, c, d, x[12], S41, 0x655b59c3); // 53
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); // 54
	II (c, d, a, b, x[10], S43, 0xffeff47d); // 55
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); // 56
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); // 57
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); // 58
	II (c, d, a, b, x[ 6], S43, 0xa3014314); // 59
	II (b, c, d, a, x[13], S44, 0x4e0811a1); // 60
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); // 61
	II (d, a, b, c, x[11], S42, 0xbd3af235); // 62
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); // 63
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); // 64

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	MD5_memset( (unsigned char*)x, 0, sizeof (x) );
}

/******************************************************************************/
//	名称：MD5_Update
//	功能：明文填充，明文分组，16个子明文分组
//  参数：指向SHA状态变量
//	返回：无

/******************************************************************************/
void MD5_Update( MD5_State *s, unsigned char *input, unsigned int inputLen )
{
	unsigned int i, index, partLen;

	//! 明文填充

	//! 字节数 mod 64
	index = (unsigned int)((s->count[0] >> 3) & 0x3F);

	//! 更新位数
	if((s->count[0] += ((unsigned long int)inputLen << 3))
		< ((unsigned long int)inputLen << 3))
		s->count[1]++;
	s->count[1] += ((unsigned long int)inputLen >> 29);

	partLen = 64 - index;

	//! MD5 4轮运算
	if (inputLen >= partLen)
	{
		MD5_memcpy( (unsigned char*)&s->buffer[index],  (unsigned char*)input, partLen );
		MD5_Transform( s->state, s->buffer );

		for( i = partLen; i + 63 < inputLen; i += 64 )
			MD5_Transform( s->state, &input[i] );

		index = 0;
	}
	else
		i = 0;

	MD5_memcpy ((unsigned char*)&s->buffer[index], (unsigned char*)&input[i], inputLen-i);
}

/******************************************************************************/
//	名称：MD5_Final
//	功能：MD5最后变换
//  参数：strContent:指向文件内容缓冲区; iLength:文件内容长度; output:摘要输出缓冲区
//	返回：无

/******************************************************************************/
void MD5_Final( MD5_State *s, unsigned char digest[16] )
{
	unsigned char bits[8];
	unsigned int index, padLen;

	Encode (bits, s->count, 8);

	//! 长度小于448位(mod 512),对明文进行填充
	index = (unsigned int)((s->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5_Update( s, s->PADDING, padLen );

	MD5_Update( s, bits, 8);
	Encode( digest, s->state, 16 );

	MD5_memset ((unsigned char*)s, 0, sizeof (*s));
	MD5_Init( s );
}


/******************************************************************************/
//	名称：SHA_digest
//	功能：生成文件摘要
//  参数：strContent:指向文件内容缓冲区; iLength:文件内容长度; output:摘要输出缓冲区
//	返回：无

/******************************************************************************/
void md5_digest( void const *strContent, unsigned int iLength, unsigned char output[16] )
{
	unsigned char *q = (unsigned char*)strContent;
	MD5_State s;

	MD5_Init( &s );
	MD5_Update( &s, q, iLength );
	MD5_Final( &s, output );
}