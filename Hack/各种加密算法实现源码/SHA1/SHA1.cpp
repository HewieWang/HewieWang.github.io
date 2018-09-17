#include <string.h>
#include <stdlib.h>
#include "sha1.h"

#define rol(x,y) ( ((x) << (y)) | (((unsigned int)x) >> (32-y)) )

//! ����SHA1״̬���ݽṹ����
typedef struct{
	unsigned int	h[5];		// 5����ʼ���ӱ���;����20�ֽ�ժҪ
	unsigned char	block[64];	// ����
	int				blkused;
	unsigned int	lenhi,lenlo;// ������
}SHA_State;

/******************************************************************************/
//	���ƣ�SHA_Init
//	���ܣ���ʼ��SHA״̬����(��ʼ��SHA1��ʼ���ӱ���)
//  ������ָ��SHA״̬����
//	���أ���

/******************************************************************************/
void SHA_Init(SHA_State * s)
{
    s->h[0] = 0x67452301;
	s->h[1] = 0xefcdab89;
	s->h[2] = 0x98badcfe;
	s->h[3] = 0x10325476;
	s->h[4] = 0xc3d2e1f0;

    s->blkused = 0;
    s->lenhi = s->lenlo = 0;
}

/******************************************************************************/
//	���ƣ�SHATransform
//	���ܣ�80�������ķ��������
//  ������digest:��ʼ���ӱ�����block:�����ķ���
//	���أ���

/******************************************************************************/
void SHATransform( unsigned int * digest, unsigned int * block )
{
    unsigned int w[80];
    unsigned int a, b, c, d, e;
    int t;

	//! ���ķ�������(0��15)
	//! ���䷽�� W[t] = block[t]
    for ( t = 0; t < 16; t++ )
		w[t] = block[t];
	
	//! ���ķ�������(16-80)
	//! ���䷽����W[t] = (W[t-3]^W[t-8]^W[t-14]^W[t-16]) <<< 1
	//! <<<1 <=>  ((x) << (y)) | (((x) >> (32-y)))
    for ( t = 16; t < 80; t++ )
	{
		unsigned int tmp = w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16];
		w[t] = rol(tmp, 1);
    }

    a = digest[0];
    b = digest[1];
    c = digest[2];
    d = digest[3];
    e = digest[4];

	//! 80�������1��(1-20��)
    for ( t = 0; t < 20; t++ ) 
	{
		unsigned int tmp = rol(a, 5) + ((b & c) | (d & ~b)) + e + w[t] + 0x5a827999;
		e = d;
		d = c;
		c = rol(b, 30);
		b = a;
		a = tmp;
    }

	//! 80�������2��(21-40��)
    for (t = 20; t < 40; t++) 
	{
		unsigned int tmp = rol(a, 5) + (b ^ c ^ d) + e + w[t] + 0x6ed9eba1;
		e = d;
		d = c;
		c = rol(b, 30);
		b = a;
		a = tmp;
    }

	//! 80�������3��(41-60��)
    for (t = 40; t < 60; t++)
	{
		unsigned int tmp =rol(a,5) + ((b & c) | (b & d) | (c & d)) + e + w[t] +0x8f1bbcdc;
		e = d;
		d = c;
		c = rol(b, 30);
		b = a;
		a = tmp;
    }

	//! 80�������4��(61-80��)
    for (t = 60; t < 80; t++) 
	{
		unsigned int tmp = rol(a, 5) + (b ^ c ^ d) + e + w[t] + 0xca62c1d6;
		e = d;
		d = c;
		c = rol(b, 30);
		b = a;
		a = tmp;
    }

	//! �����������
    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
    digest[4] += e;
}

/******************************************************************************/
//	���ƣ�SHA_Byte
//	���ܣ�������䣬���ķ��飬16�������ķ���
//  ������ָ��SHA״̬����
//	���أ���

/******************************************************************************/
void SHA_Bytes( SHA_State *s, void *p, int len )
{
	int i;
	unsigned char *q  = (unsigned char *)p;
	//! ��512λ�����ķ���ֳ�16�������ķ��飬������16�������ķ���
	unsigned int wordblock[16];
	unsigned int lenw = len;

	//! ���³�����
	s->lenlo += lenw;
	s->lenhi += (s->lenlo < lenw);

	if (s->blkused && s->blkused + len < 64) 
	{
		memcpy(s->block + s->blkused, q, len);
		s->blkused += len;
    } 
	else
	{
		while (s->blkused + len >= 64)
		{
			memcpy(s->block + s->blkused, q, 64 - s->blkused);
			q += 64 - s->blkused;
			len -= 64 - s->blkused;

			for (i = 0; i < 16; i++) 
			{
				//! �����ķ���
				wordblock[i] = (( (unsigned int) s->block[i * 4 + 0] ) << 24) |
								(( (unsigned int) s->block[i * 4 + 1] ) << 16) |
								(( (unsigned int) s->block[i * 4 + 2] ) << 8) |
								(( (unsigned int) s->block[i * 4 + 3] ) << 0);
			}
			SHATransform( s->h, wordblock );
			s->blkused = 0;
		}
		memcpy(s->block, q, len);
		s->blkused = len;
    }
}


/******************************************************************************/
//	���ƣ�SHA_Final
//	���ܣ��ļ�ժҪʮ���������
//  ������s:ָ��SHA״̬����; output:ָ�����������
//	���أ���

/******************************************************************************/
void SHA_Final( SHA_State * s, unsigned char *output )
{
    int i, pad;
    unsigned char c[64];
    unsigned int lenhi, lenlo;

    if( s->blkused >= 56 )
		pad = 56 + 64 - s->blkused;
    else
		pad = 56 - s->blkused;

	lenhi = (s->lenhi << 3) | (s->lenlo >> (32 - 3));
    lenlo = (s->lenlo << 3);

    memset( c, 0, pad );
    c[0] = 0x80;
    SHA_Bytes( s, &c, pad );

    c[0] = (lenhi >> 24) & 0xFF;
    c[1] = (lenhi >> 16) & 0xFF;
    c[2] = (lenhi >> 8) & 0xFF;
    c[3] = (lenhi >> 0) & 0xFF;
    c[4] = (lenlo >> 24) & 0xFF;
    c[5] = (lenlo >> 16) & 0xFF;
    c[6] = (lenlo >> 8) & 0xFF;
    c[7] = (lenlo >> 0) & 0xFF;

    SHA_Bytes(s, &c, 8);

    for( i = 0; i < 5; i++ ) 
	{
		output[i * 4] = (s->h[i] >> 24) & 0xFF;
		output[i * 4 + 1] = (s->h[i] >> 16) & 0xFF;
		output[i * 4 + 2] = (s->h[i] >> 8) & 0xFF;
		output[i * 4 + 3] = (s->h[i]) & 0xFF;
    }
}

/******************************************************************************/
//	���ƣ�SHA_digest
//	���ܣ������ļ�ժҪ
//  ������strContent:ָ���ļ����ݻ�����; iLength:�ļ����ݳ���; output:ժҪ���������
//	���أ���

/******************************************************************************/
void sha1_digest( unsigned char *strConent, unsigned int iLength, unsigned char *output )
{
	//! ����SHA1״̬��������
	SHA_State s;

	SHA_Init( &s );
	SHA_Bytes( &s, strConent, iLength );
	SHA_Final(&s, output);
}