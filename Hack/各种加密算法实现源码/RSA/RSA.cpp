#include "rsa.h"

//!������˽Կd����
struct pKeyset
{
	unsigned int set[ MAX_NUM ];
	unsigned int size;
}pset;

//! ���湫��˽Կ��
struct pPairkey
{
	unsigned int d;
	unsigned int e;
	unsigned int n;
}pairkey;

/******************************************************************************/
//	���ƣ�isPrime
//	���ܣ��ж��������Ƿ���
//  ������m: ��a; n: ��b
//	���أ�m��n���ʷ���true; ���򷵻�false

/******************************************************************************/
bool isPrime( unsigned int m, unsigned int n )
{
	unsigned int i=0;
	bool Flag = true;

	if( m<2 || n<2 )
		return false;

	unsigned int tem = ( m > n ) ? n : m;
	for( i=2; i<=tem && Flag; i++ )
	{
		bool mFlag = true;
		bool nFlag = true;
		if( m % i == 0 )
			mFlag = false;
		if( n % i == 0 )
			nFlag = false;
		if( !mFlag && !nFlag )
			Flag = false;
	}
	if( Flag )
		return true;
	else
		return false;
}

/******************************************************************************/
//	���ƣ�MakePrivatedKeyd
//	���ܣ�������Q��Q����˽Կd
//  ������uiP: ����P; uiQ: ����Q
//	���أ�˽Կd

/******************************************************************************/
unsigned int MakePrivatedKeyd( unsigned int uiP, unsigned int uiQ )
{
	unsigned int i=0;

	//! �õ�������z���ʵ���( ˽Կd�ļ��� )
	unsigned int z = ( uiP -1 ) * ( uiQ -1 );
	pset.size = 0;
	for( i=0; i<z; i++ )
	{
		if( isPrime( i, z ) )
		{
			pset.set[ pset.size++ ] = i;
		}
	}

	return pset.size;
}

/******************************************************************************/
//	���ƣ�MakePairKey
//	���ܣ�����RSA����˽Կ��
//  ������uiP: ����P; uiQ: ����Q; uiD: ˽Կd
//	���أ��������

/******************************************************************************/
unsigned int MakePairkey( unsigned int uiP, unsigned int uiQ, unsigned int uiD )
{
	bool bFlag = true;
	unsigned int i = 0, e;
	unsigned int z = ( uiP-1 ) * ( uiQ-1 );
	unsigned int d = pset.set[uiD];

	if( !isPrime( z, d ) )
		return ERROR_NOEACHPRIME;

	for( i=2; i<z; i++ )
	{
		if( (i*d)%z == 1 )
		{
			e = i;
			bFlag = false;
		}
	}
	if( bFlag )
		return ERROR_NOPUBLICKEY;

	if( (d*e)%z != 1 )
		ERROR_GENERROR;

	pairkey.d = d;
	pairkey.e = e;
	pairkey.n = uiP * uiQ;

	return OK;
}

/******************************************************************************/
//	���ƣ�GetPairKey
//	���ܣ������ṩ�ӿڣ���ù���˽Կ��
//  ������uiP: ����P; uiQ: ����Q; uiD: ˽Կd
//	���أ�

/******************************************************************************/
unsigned int GetPairKey( unsigned int &d, unsigned int &e )
{
	d = pairkey.d;
	e = pairkey.e;
	
	return pairkey.n;
}

/******************************************************************************/
//	���ƣ�GetPrivateKeyd
//	���ܣ������ṩ�ӿڣ����û�ѡ��ID����˽Կd
//  ������iWhich: �û�ѡ��˽Կd��ID
//	���أ�˽Կdֵ

/******************************************************************************/
unsigned int GetPrivateKeyd( unsigned int iWhich )
{
	if( pset.size >= iWhich )
		return pset.set[ iWhich ];
	else
		return 0;
}

/******************************************************************************/
//	���ƣ�rsa_encrypt
//	���ܣ�RSA��������
//  ������n: ��Կn; e: ��Կe; mw: ��������; iLength: ���ĳ���; cw: �������
//	���أ���

/******************************************************************************/
void rsa_encrypt( int n, int e, char *mw, int mLength, int *&cw )
{	
	int i=0, j=0;
	__int64 temInt = 0;

	for( i=0; i<mLength; i++ )
	{
		temInt = mw[i];
		if( e!=0 )
		{
			for( j=1; j<e; j++ )
			{
				temInt = ( temInt * mw[i] ) % n;
			}
		}
		else
		{
			temInt = 1;
		}

		cw[i] = (int)temInt;
	}
}

/******************************************************************************/
//	���ƣ�rsa_decrypt
//	���ܣ�RSA��������
//  ������n: ˽Կn; d: ˽Կd; cw: ����; cLength: ���ĳ���; mw: �������
//	���أ���

/******************************************************************************/
void rsa_decrypt( int n, int d, int *&cw, int cLength, char *mw )
{
	int i=0, j=-1;
	__int64 temInt = 0;

	for( i=0; i<cLength/4; ++i )
	{
		mw[i] = 0;
		temInt = cw[i];

		if( d != 0 )
		{
			for( j=1; j<d; j++ )
			{
				temInt = (__int64)( temInt * cw[i] ) % n;
			}
		}
		else
		{
			temInt = 1;
		}

		mw[i] = (char)temInt;
	}
}