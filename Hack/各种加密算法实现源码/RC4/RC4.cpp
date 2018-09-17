#include <memory.h>
#include <assert.h>
#include <afxwin.h>
#include "rc4.h"

//! 宏定义，SBox元素256
#define SBOX_SIZE 256

//! 全局变量，存储密钥
unsigned char*	g_Key = NULL;
//! 全局变量，密钥长度
unsigned int	g_KeyLen = 0;
//! 全局变量，S-Box
int *g_SBox = NULL;

/******************************************************************************/
//	名称：SetKey
//	功能：设置加密密钥
//  参数：key: 指向加密密钥指针；nKeyLen: 密钥长度
//	返回：无
//  备注：使用Setup前必须调用SetKey设置加/解密密钥

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
//	名称：Setup
//	功能：密钥安装阶段：生成置换S-Box
//  参数：无
//	返回：无
//  备注：使用Crypt前必须调用Setup函数初始化加密环境

/******************************************************************************/
void Setup()
{
	unsigned int i,j,k;
	int tmp;

	//! 密钥为空
	if( !g_Key || g_KeyLen == 0 ) return;

	g_SBox = new int[ SBOX_SIZE ];
	assert( NULL != g_SBox );

	ZeroMemory( g_SBox, SBOX_SIZE );

	//! 用0~255填充S-Box
	for( i=0; i<SBOX_SIZE; i++ )
	{
		g_SBox[i] = i;
	}

	//! 生成置换S-Box
	//! 通过将s-Box中的每一个元素与S-Box中的另一个元素交换一次的方式，打乱S-Box中数字的排列顺序
	//! 由密钥循环填充256数组，生成K-Box
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
//	名称：Encrypt
//	功能：1. 密钥安装阶段：为每一个待加密的字节生成一个伪随机数;
//		  2. 加/解密
//  参数：pData: 指向待加/解密数据；nLength: 数据长度
//	返回：无
//  备注：使用Crypt前必须调用RandomKey函数初始化加密环境

/******************************************************************************/
void Encrypt( unsigned char *pData, int nLength )
{
	int i,j,k,R,tmp;
	i=j=k=0;

	for( i=0; i<nLength; ++i )
	{
		j = ( j + 1 ) % SBOX_SIZE;
		k = ( k + g_SBox[j] ) % SBOX_SIZE;

		//! 将转换S-Box中的第j个元素和第k个元素交换
		tmp = g_SBox[j];
		g_SBox[j] = g_SBox[k];
		g_SBox[k] = tmp;

		//! 生成随机数R
		R = g_SBox[( g_SBox[j] + g_SBox[k] ) % SBOX_SIZE];

		//! 数据与随机数进行异或运算
		pData[i] ^= R;
	}

	//! 加/解密操作完成，释放内存空间
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