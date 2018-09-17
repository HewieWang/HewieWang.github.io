#include <string.h>
#include <stdlib.h>
#include "idea.h"

//! �궨�壬IDEA�����ִ�
#define ROUNDS	8
//! �궨�壬������ܹ��̵�����Կ��Ŀ
#define KEYLEN	(6*ROUNDS+4)
//! �궨�壬��ȡ��16λ��
#define low16(x) ((x) & 0xffff)

//! ���ڱ���52����������Կ
unsigned short gEnkey[52];
//! ���ڱ���52�������ӽ�Կ
unsigned short gDekey[52];
typedef unsigned short IDEAkey[KEYLEN];

struct IDEA_MSG_ITEM
{
	unsigned short	X[4];
	unsigned short	Z[6];
	unsigned short	W[4];	
};
struct IDEA_MSG
{
	IDEA_MSG_ITEM	items[8];
	unsigned short	Y[4];
	unsigned short	LZ[4];

};
IDEA_MSG	gIDEA_MSG;

unsigned int inv( unsigned int x )
{
	unsigned int t0,t1;
	unsigned int q,y;
	if( x <= 1 )
		return x;

	t1 = (unsigned int)(0x10001l / x);
	y = (unsigned int)(0x10001l % x);
	if( y == 1 )
		return low16( 1 - t1 );

	t0 = 1 ;
	do
	{
		q = x / y;
		x = x % y;
		t0 += q * t1;
		if (x == 1)
			return t0;

		q = y / x;
		y = y % x;
		t1 += q * t0;
	} while( y != 1 );

	return low16( 1-t1 );
}

unsigned short mul( unsigned int a, unsigned int b )
{
	unsigned int p;

	if(a)
	{
		if(b)
		{
			p=(unsigned int)a*b;
			b=(unsigned int)(low16(p));
			a=(unsigned int)(p>>16);
			return b-a+(b<a);
		}
		else
		{
			return 1-a;
		}
	}
	else
		return 1-b;
}
#define IDEA_MUL(x,y) (x=mul(low16(x),y))

/******************************************************************************/
//	���ƣ�MakeEnkey
//	���ܣ����û�������Կ����52������Կ��ÿ������Կ16λ
//  ������userKey: �û�������Կ; exKey: 52����������Կ
//	���أ���

/******************************************************************************/
void MakeEnkey( unsigned short *userKey, unsigned short *enKey )
{
	int i,j;

	//! �û�������Կ�ֳ�8�飬���ɵ�1�鹲8������Կ
	for( j=0; j<8; j++ )
		enKey[j] = *userKey++;

	//! ���¶���ѭ��ִ��52��
	//! ������Կѭ������25λ���ֳ�8�飬�õ�һ������Կ
	//! ��ǰ�ִ�ѭ�����ƺ����Կ��������һ�ִε�������Կ
	for( i=0; j<KEYLEN; j++ )
	{
		i++;
		enKey[i+7] = ((enKey[i&7] << 9) | (enKey[i+1 & 7] >> 7));
		enKey += i&8;
		i &= 7;
	}
}

/******************************************************************************/
//	���ƣ�MakeDekey
//	���ܣ���52����������Կ����52����������Կ��ÿ������Կ16λ
//  ������userKey: �û�������Կ; exKey: 52����������Կ
//	���أ���

/******************************************************************************/
void MakeDekey( IDEAkey enKey, IDEAkey deKey )
{
	int j;
	unsigned int t1,t2,t3;
	unsigned short usTemp[KEYLEN];
	unsigned short *p = usTemp + KEYLEN;
	t1 = inv( *enKey++ );
	t2 =- *enKey++;
	t3 =- *enKey++;
	*--p = inv( *enKey++ );
	*--p = t3;
	*--p = t2;
	*--p = t1;
	for( j=1; j<ROUNDS; j++ )
	{
		t1 = *enKey++;
		*--p = *enKey++;
		*--p = t1;
		t1 = inv( *enKey++ );
		t2 = -*enKey++;
		t3 = -*enKey++;
		*--p = inv( *enKey++ );
		*--p = t2;
		*--p = t3;
		*--p = t1;
	}
	t1 = *enKey++;
	*--p = *enKey++;
	*--p = t1;
	t1 = inv( *enKey++ );
	t2 = -*enKey++;
	t3 = -*enKey++;
	*--p = inv( *enKey++ );
	*--p = t3;
	*--p = t2;
	*--p = t1;

	for( j=0,p=usTemp; j<KEYLEN; j++ )
	{
		*deKey++ = *p;
		*p++ = 0;
	}
}

/******************************************************************************/
//	���ƣ�SetKey
//	���ܣ����ú��������û�������Կ����52������Կ
//  ������key: �û�������Կ; kLen: ��Կ����
//	���أ��ɹ�����true�����򷵻�false

/******************************************************************************/
bool SetKey( unsigned short *key, unsigned short kLen )
{
	if( kLen != 8 )
		return false;

	MakeEnkey( key, gEnkey );
	MakeDekey( gEnkey, gDekey );

	return true;
}

/******************************************************************************/
//	���ƣ�idea_encrypt
//	���ܣ�IDEA��/���ܹ���8�ֵ��������任����
//  ������in: 64λ��������; out: 64λ�������; enKey: ��������Կ
//	���أ���

/******************************************************************************/
void cipher_idea( unsigned short in[4], unsigned short out[4], const IDEAkey enKey )
{
	unsigned short x1,x2,x3,x4,t1,t2;
	int r = 0;
	x1 = *in++;
	x2 = *in++;
	x3 = *in++;
	x4 = *in;

	//! 8�ֵ�������
	for( r=0; r<ROUNDS; ++r)
	{
		gIDEA_MSG.items[r].X[0] = x1;
		gIDEA_MSG.items[r].X[1] = x2;
		gIDEA_MSG.items[r].X[2] = x3;
		gIDEA_MSG.items[r].X[3] = x4;
		gIDEA_MSG.items[r].Z[0] = enKey[0];
		gIDEA_MSG.items[r].Z[1] = enKey[1];
		gIDEA_MSG.items[r].Z[2] = enKey[2];
		gIDEA_MSG.items[r].Z[3] = enKey[3];
		gIDEA_MSG.items[r].Z[4] = enKey[4];
		gIDEA_MSG.items[r].Z[5] = enKey[5];
		IDEA_MUL( x1, *enKey++ );
		x2 += *enKey++;
		x3 += *enKey++;
		IDEA_MUL( x4, *enKey++ );
		t2 = x1 ^ x3;
		IDEA_MUL( t2, *enKey++ );
		t1 = t2 + (x2 ^ x4);
		IDEA_MUL( t1, *enKey++ );
		t2 = t1 + t2;
		x1 ^= t1;
		x4 ^= t2;
		t2 ^= x2;
		x2 = x3 ^ t1;
		x3 = t2;
		gIDEA_MSG.items[r].W[0] = x1;
		gIDEA_MSG.items[r].W[1] = x2;
		gIDEA_MSG.items[r].W[2] = x3;
		gIDEA_MSG.items[r].W[3] = x4;
	}
	gIDEA_MSG.LZ[0] = enKey[0];
	gIDEA_MSG.LZ[1] = enKey[1];
	gIDEA_MSG.LZ[2] = enKey[2];
	gIDEA_MSG.LZ[3] = enKey[3];

	//! ���任
	IDEA_MUL( x1, *enKey++ );
	gIDEA_MSG.Y[0] = x1;
	*out++ = x1;
	gIDEA_MSG.Y[1] = (x3 + *enKey++);
	*out++ = gIDEA_MSG.Y[1];
	gIDEA_MSG.Y[2] = (x2 + *enKey++);
	*out++ = gIDEA_MSG.Y[2];
	IDEA_MUL( x4, *enKey );
	gIDEA_MSG.Y[3] = x4;
	*out = x4;
}

/******************************************************************************/
//	���ƣ�idea_encrypt
//	���ܣ����ú���ʵ�֣�IDEA����
//  ������in: ָ���������뻺����; out: ָ���������������
//	���أ���

/******************************************************************************/
void idea_encrypt( unsigned short *in, unsigned short *out )
{
	cipher_idea( in, out, gEnkey );
}

/******************************************************************************/
//	���ƣ�idea_decrypt
//	���ܣ����ú���ʵ�֣�IDEA����
//  ������in: ָ���������뻺����; out: ָ���������������
//	���أ���

/******************************************************************************/
void idea_decrypt( unsigned short *in, unsigned short *out )
{
	cipher_idea( in, out, gDekey );
}