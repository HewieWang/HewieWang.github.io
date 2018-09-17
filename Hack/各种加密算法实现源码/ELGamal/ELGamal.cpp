#include <afxwin.h>
#include <assert.h>
#include "elgamal.h"
#include <iostream>

using namespace std;

//! �궨�壬�̶�����P
#define PRIME_P		9871
//! �궨�壬�̶���ԭԪ��
#define PRIME_G		7527
//! �궨�壬�������ֵ
#define RANDOM		1795

/******************************************************************************/
//	���ƣ�IsNumber
//	���ܣ��ж������ַ�����
//  ������strNumber:�ַ�����
//	���أ������������鷵��true�����򷵻�false;

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
//	���ƣ�PublicKey
//	���ܣ���˽Կ���ɹ�Կ
//  ������xKey:˽ԿX(����)
//	���أ���ԿyKey(����)
//  ��ע���̶�����P�뱾ԭԪ��

/******************************************************************************/
unsigned int PublicKey( unsigned int xKey )
{
	unsigned int yKey;

	//! ˽ԿȡֵӦ������[0, PRIME_P)
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
//	���ƣ�ELGamal_encrypt
//	���ܣ�����
//  ������yKey:���ܹ�Կ��mw:ָ�����Ļ�������clw:�������c1��c2w:�������c2(��������)
//	���أ����ܳɹ�����true�����򷵻�false
//  ��ע���̶�����P����ԭԪ�غͼ������ֵ

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

	//! �������ͷ��ڴ�ռ�
	delete [] m;
	return true;
}

/******************************************************************************/
//	���ƣ�ELGamal_decrypt
//	���ܣ�����
//  ������xKey:����˽Կ��cw:ָ�����Ļ�������clw:���������c2w:�������
//		  mw:ָ�����Ļ�������cLong������c2�����ݶ�(�ԡ�,�����)
//	���أ����ܳɹ�����true�����򷵻�false
//  ��ע���̶�����P�ͱ�ԭԪ��

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
		rslt = ( kb * kReciprocal ) % PRIME_P;//��k�ĵ���kReciprocal
	}
	kReciprocal--;

	for( int i = 0; i < cLong; i++ )
	{
		mw[i] = ( kReciprocal * c2w[i] ) % PRIME_P;
	}

	return true;
}