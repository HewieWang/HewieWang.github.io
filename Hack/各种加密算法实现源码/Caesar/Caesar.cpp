#include "Caesar.h"
//#include <string>

/******************************************************************************/
//	名称：Caesar_encrypt
//	功能：Caesar加密
//  参数：k:密钥; m:明文; c:加密后的密文
//	返回：无
//  备注：Caesar加密变换: c = (m + k) mod 26

/******************************************************************************/
void Caesar_encrypt( int k, char *m, unsigned int mLen, char *c )
{
	unsigned int i=0;
	for( i=0; i<mLen; i++ )
	{
		if( m[i] >= 'a' && m[i] <= 'z'-k )
		{
			c[i] = (m[i]-'a') + ('A'+k);
		}
		else if( m[i] >= 'a' && m[i] > 'z'-k )
		{
			c[i] = (m[i]-'z'-1) + ('A'+k);
		}
		else if( m[i] >= 'A' + k && m[i] <= 'Z' )
		{
			c[i] = (m[i]-'A'-k) + 'a';
		}
		else if( m[i] >= 'A' && m[i] < 'A' + k)
		{
			c[i] = (m[i]-'A') + ('z'+1-k);
		}
		else
		{
			c[i] = m[i];
		}
	}
}

/******************************************************************************/
//	Caesar_decrypt
//	功能：Caesar解密
//  参数：k:密钥; c:密文; m:解密后的明文
//	返回：更新成功返回true，否则返回false
//  备注：Caesar解密变换: m = (c – k) mod 26

/******************************************************************************/
void Caesar_decrypt( int k, char *c, unsigned int cLen, char *m )
{
	unsigned int i=0;
	for( i=0; i<cLen; i++ )
	{
		if( c[i] >= 'A'+k && c[i] <= 'Z' )
		{
			m[i] = (c[i]-'A'-k) + 'a';
		}
		else if( c[i] >= 'A' && c[i] < 'A'+k )
		{
			m[i] = (c[i]-'A') + ('z'+1-k);
		}
		else if( c[i] >= 'a' && c[i] <= 'z'-k )
		{
			m[i] = (c[i]-'a')+('A'+k);
		}
		else if( c[i]>'z'-k && c[i]<='z' )
		{
			m[i] = (c[i]+k-'z'-1) + 'A';
		}
		else
			m[i] = c[i];
	}
}
