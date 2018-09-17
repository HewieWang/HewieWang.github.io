#include <string.h>
#include <stdlib.h>
#include "idea.h"

//! 宏定义，IDEA迭代轮次
#define ROUNDS	8
//! 宏定义，参与加密过程的子密钥数目
#define KEYLEN	(6*ROUNDS+4)
//! 宏定义，获取低16位字
#define low16(x) ((x) & 0xffff)

//! 用于保存52个加密子密钥
unsigned short gEnkey[52];
//! 用于保存52个解密子解钥
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
//	名称：MakeEnkey
//	功能：由用户输入密钥生成52个子密钥，每个子密钥16位
//  参数：userKey: 用户输入密钥; exKey: 52个加密子密钥
//	返回：无

/******************************************************************************/
void MakeEnkey( unsigned short *userKey, unsigned short *enKey )
{
	int i,j;

	//! 用户输入密钥分成8组，生成第1组共8个子密钥
	for( j=0; j<8; j++ )
		enKey[j] = *userKey++;

	//! 以下动作循环执行52次
	//! 输入密钥循环左移25位，分成8组，得到一个子密钥
	//! 当前轮次循环左移后的密钥，即是下一轮次的输入密钥
	for( i=0; j<KEYLEN; j++ )
	{
		i++;
		enKey[i+7] = ((enKey[i&7] << 9) | (enKey[i+1 & 7] >> 7));
		enKey += i&8;
		i &= 7;
	}
}

/******************************************************************************/
//	名称：MakeDekey
//	功能：由52个加密子密钥生成52个解密子密钥，每个子密钥16位
//  参数：userKey: 用户输入密钥; exKey: 52个加密子密钥
//	返回：无

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
//	名称：SetKey
//	功能：调用函数，由用户输入密钥生成52个字密钥
//  参数：key: 用户输入密钥; kLen: 密钥长度
//	返回：成功返回true，否则返回false

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
//	名称：idea_encrypt
//	功能：IDEA加/解密过程8轮迭代及最后变换运算
//  参数：in: 64位输入明文; out: 64位输出密文; enKey: 加密子密钥
//	返回：无

/******************************************************************************/
void cipher_idea( unsigned short in[4], unsigned short out[4], const IDEAkey enKey )
{
	unsigned short x1,x2,x3,x4,t1,t2;
	int r = 0;
	x1 = *in++;
	x2 = *in++;
	x3 = *in++;
	x4 = *in;

	//! 8轮迭代计算
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

	//! 最后变换
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
//	名称：idea_encrypt
//	功能：调用函数实现，IDEA加密
//  参数：in: 指向明文输入缓冲区; out: 指向密文输出缓冲区
//	返回：无

/******************************************************************************/
void idea_encrypt( unsigned short *in, unsigned short *out )
{
	cipher_idea( in, out, gEnkey );
}

/******************************************************************************/
//	名称：idea_decrypt
//	功能：调用函数实现，IDEA解密
//  参数：in: 指向密文输入缓冲区; out: 指向明文输出缓冲区
//	返回：无

/******************************************************************************/
void idea_decrypt( unsigned short *in, unsigned short *out )
{
	cipher_idea( in, out, gDekey );
}