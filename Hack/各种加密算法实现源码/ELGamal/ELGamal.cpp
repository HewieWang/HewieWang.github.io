#include <afxwin.h>
#include <assert.h>
#include "elgamal.h"
#include <iostream>

using namespace std;

//! 宏定义，固定素数P
#define PRIME_P		9871
//! 宏定义，固定本原元素
#define PRIME_G		7527
//! 宏定义，加密随机值
#define RANDOM		1795

/******************************************************************************/
//	名称：IsNumber
//	功能：判断数字字符数组
//  参数：strNumber:字符数组
//	返回：数字字组数组返回true，否则返回false;

/******************************************************************************/
bool IsNumber( const char *strNumber )
{
	int i;

	if( !strNumber )
		return false;

	for ( i = 0 ; i < strlen(strNumber) ; i++ )
	{
		if ( strNumber[i] < '0' || strNumber[i] > '9' )
			return false;
	}

	return true;
}

/******************************************************************************/
//	名称：PublicKey
//	功能：由私钥生成公钥
//  参数：xKey:私钥X(整数)
//	返回：公钥yKey(整数)
//  备注：固定素数P与本原元素

/******************************************************************************/
unsigned int PublicKey( unsigned int xKey )
{
	unsigned int yKey;

	//! 私钥取值应在区间[0, PRIME_P)
	if( xKey >= PRIME_P )
		return 0;

	yKey = PRIME_G;
	if( xKey != 0 )
	{
		for( unsigned int i=1; i<xKey; i++ )
			yKey = ( yKey * PRIME_G) % PRIME_P;

	}
	else
		yKey = 1;

	return yKey;
}

/******************************************************************************/
//	名称：ELGamal_encrypt
//	功能：加密
//  参数：yKey:加密公钥；mw:指向明文缓冲区；clw:密文输出c1；c2w:密文输出c2(保存密文)
//	返回：加密成功返回true，否则返回false
//  备注：固定素数P、本原元素和加密随机值

/******************************************************************************/
bool ELGamal_encrypt( int yKey, char * mw, int &c1w, int *&c2w )
{
	int i;
	std::string mTem(mw);
	
	int mLong = (int)mTem.length();
	char *m = new char[mLong];

	for( i = 0; i < mLong; i++)
		m[i] = mTem.at(i);

	if( !( RANDOM>0 && RANDOM<PRIME_P ) )
		return false;

	int c1 = PRIME_G;
	int k = yKey;
	for( i = 1; i <RANDOM ; i++)
	{
		c1 = ( c1 * PRIME_G ) % PRIME_P;
		k = ( k * yKey ) % PRIME_P;
	}

	for( i = 0; i < mLong; i++)
		c2w[i] = ( m[i] * k ) % PRIME_P;

	c1w = c1;

	//! 别忘记释放内存空间
	delete [] m;
	return true;
}

/******************************************************************************/
//	名称：ELGamal_decrypt
//	功能：解密
//  参数：xKey:解密私钥；cw:指向密文缓冲区；clw:密文输出；c2w:密文输出
//		  mw:指向明文缓冲区；cLong：密文c2中数据段(以“,”间隔)
//	返回：解密成功返回true，否则返回false
//  备注：固定素数P和本原元素

/******************************************************************************/
bool ELGamal_decrypt( int xKey, int c1w, int *c2w, char *mw, int cLong )
{
	int kb = c1w;

	if ( xKey != 0 )
	{
		for( int i = 1; i < xKey; i++ )
			kb = ( kb * c1w ) % PRIME_P;
	}
	else
	{
		kb = 1;
	}
	
	int kReciprocal = 1;
	int rslt = 0;
	for ( kReciprocal; rslt != 1; kReciprocal++ )
	{
		rslt = ( kb * kReciprocal ) % PRIME_P;//求k的倒数kReciprocal
	}
	kReciprocal--;

	for( int i = 0; i < cLong; i++ )
	{
		mw[i] = ( kReciprocal * c2w[i] ) % PRIME_P;
	}

	return true;
}