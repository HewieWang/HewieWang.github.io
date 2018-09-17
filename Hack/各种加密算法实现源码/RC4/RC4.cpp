#include <memory.h>
#include <assert.h>
#include <afxwin.h>
#include "rc4.h"

//! �궨�壬SBoxԪ��256
#define SBOX_SIZE 256

//! ȫ�ֱ������洢��Կ
unsigned char*	g_Key = NULL;
//! ȫ�ֱ�������Կ����
unsigned int	g_KeyLen = 0;
//! ȫ�ֱ�����S-Box
int *g_SBox = NULL;

/******************************************************************************/
//	���ƣ�SetKey
//	���ܣ����ü�����Կ
//  ������key: ָ�������Կָ�룻nKeyLen: ��Կ����
//	���أ���
//  ��ע��ʹ��Setupǰ�������SetKey���ü�/������Կ

/******************************************************************************/
void SetKey( unsigned char* key, int nKeyLen )
{
	if( key == NULL || nKeyLen == 0 )
		return;

	g_KeyLen = nKeyLen;
	g_Key = new unsigned char[ nKeyLen ];
	assert( NULL != g_Key );
	if( NULL == g_Key )
	{
		return;
	}

	for( int i = 0; i < nKeyLen; ++i )
	{
		g_Key[i] = key[i];
	}
}

/******************************************************************************/
//	���ƣ�Setup
//	���ܣ���Կ��װ�׶Σ������û�S-Box
//  ��������
//	���أ���
//  ��ע��ʹ��Cryptǰ�������Setup������ʼ�����ܻ���

/******************************************************************************/
void Setup()
{
	unsigned int i,j,k;
	int tmp;

	//! ��ԿΪ��
	if( !g_Key || g_KeyLen == 0 ) return;

	g_SBox = new int[ SBOX_SIZE ];
	assert( NULL != g_SBox );

	ZeroMemory( g_SBox, SBOX_SIZE );

	//! ��0~255���S-Box
	for( i=0; i<SBOX_SIZE; i++ )
	{
		g_SBox[i] = i;
	}

	//! �����û�S-Box
	//! ͨ����s-Box�е�ÿһ��Ԫ����S-Box�е���һ��Ԫ�ؽ���һ�εķ�ʽ������S-Box�����ֵ�����˳��
	//! ����Կѭ�����256���飬����K-Box
	j = k = 0;
	for( i=0; i<SBOX_SIZE; ++i )
	{
		tmp = g_SBox[i];
		j = ( j + tmp + g_Key[k] ) % SBOX_SIZE;
		g_SBox[i] = g_SBox[j];
		g_SBox[j] = tmp;
		if( ++k >= g_KeyLen ) k=0;
	}
}

/******************************************************************************/
//	���ƣ�Encrypt
//	���ܣ�1. ��Կ��װ�׶Σ�Ϊÿһ�������ܵ��ֽ�����һ��α�����;
//		  2. ��/����
//  ������pData: ָ�����/�������ݣ�nLength: ���ݳ���
//	���أ���
//  ��ע��ʹ��Cryptǰ�������RandomKey������ʼ�����ܻ���

/******************************************************************************/
void Encrypt( unsigned char *pData, int nLength )
{
	int i,j,k,R,tmp;
	i=j=k=0;

	for( i=0; i<nLength; ++i )
	{
		j = ( j + 1 ) % SBOX_SIZE;
		k = ( k + g_SBox[j] ) % SBOX_SIZE;

		//! ��ת��S-Box�еĵ�j��Ԫ�غ͵�k��Ԫ�ؽ���
		tmp = g_SBox[j];
		g_SBox[j] = g_SBox[k];
		g_SBox[k] = tmp;

		//! ���������R
		R = g_SBox[( g_SBox[j] + g_SBox[k] ) % SBOX_SIZE];

		//! ����������������������
		pData[i] ^= R;
	}

	//! ��/���ܲ�����ɣ��ͷ��ڴ�ռ�
	if( g_SBox != NULL )
	{
		delete [] g_SBox;
		g_SBox = NULL;
	}
	if( g_Key != NULL )
	{
		delete [] g_Key;
		g_Key = NULL;
	}
}