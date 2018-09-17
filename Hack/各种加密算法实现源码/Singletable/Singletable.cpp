#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "singletable.h"

char chrTab[26] = {'A','B', 'C', 'D','E','F','G','H','I','J','K','L','M','N',
					'O','P','Q','R','S','T','U','V','W','X','Y','Z'};

/******************************************************************************/
//	名称：updateChrTab
//	功能：更新置换表
//  参数：phrase是以0结尾的密钥词组，长度<=26, a~z或A~Z
//	返回：更新成功返回true，否则返回false
//  备注：密钥词组不允许出现重复字符

/******************************************************************************/
bool UpdateChrTab( char *phrase, int plen )
{
	int i=0,j=0;
	char c,h;
	if( plen>26)
		return false;

	//! 申请内存空间，保存密钥词组或大写后的密钥词组
	char *strbak = new char[plen+1];
	if( strbak == NULL )
		return false;

	memcpy( strbak, phrase, plen );
	strbak[plen] = '\0';

	for(i=0; i<plen; i++)
	{
		c = phrase[i];
		//! 转换小写为大写字母
		if( c >= 97 && c <= 122 ) // a~z
			strbak[i] -= 32;
		else if( c >= 65 && c <=90 ); // A~Z
		else
		{
			//! 非字母字符
			printf( "输入的字符 %c 不合法，置换只能发生在英文字母之间", c );
			delete [] strbak;
			return false;
		}
	}

	//! 检查重复字符
	for( i=0; i<plen; i++ )
	{
		if( i+1 == plen )
			break;

		c = strbak[i];
		for( j=i+1; j<plen;j++ )
		{
			h = strbak[j];
			if( c == h )
			{
				printf( "字符 %c 重复出现\n", c );
				delete [] strbak;
				return false;
			}
		}
	}

	//! 更新转换表
	for(i=0; i<plen; i++)
	{
		c = strbak[i];
		for( int j=0; j<26 && c != chrTab[j]; j++ );
		if( j==i );
		else if( j>i && j<26 )
		{
			for( j; j>i; j-- )
				chrTab[j] = chrTab[j-1];
			chrTab[i] = c;
		}
	}

	delete [] strbak;
	return true;
}

/******************************************************************************/
//	名称：Encry
//	功能：单表置换加密
//  参数：strMw是以0结尾的文本明文，长度len<=1024；strCw是密文缓冲区，大小Clen
//	返回：更新成功返回密文长度，否则返回0
//  备注：只对明文中的字母字符进行加密

/******************************************************************************/
int Encry( char *strMw, int len,  char *strCw, int Clen )
{
	int i=0,j=0;
	int iCw = 0;

	if( strMw==NULL || strCw==NULL || len>1024 || len<=0 || Clen<len)
	{
		printf( "单表置换加密输入错误!\n" );
		return 0;
	}

	memset( strCw, 0,  Clen );
	for( i=0; i<len; i++ )
	{
		unsigned char aaa = strMw[i];

		if( strMw[i] >=65 && strMw[i] <=90 )
		{
			for( j=0; j<26; j++ )
			{
				if( strMw[i] == chrTab[j] )
				{
					//! 明文大写输入，密文小写输出
					strCw[iCw] = (char)('a'+j);
					iCw++;
				}
			}
		}
		else if( strMw[i] >= 97 && strMw[i] <=122 )
		{
			//! 明文小写输入，密文大写输出
			strCw[iCw] = chrTab[ strMw[i]-'a' ];
			iCw++;
		}
		else if( (strMw[i] > 127) ||
				 (strMw[i] < 32 && strMw[i] != 0x0a && strMw[i] != 0x0d ))
		{
			printf( "singtable不能够对某些明文字符进行加密操作!\n" );
			return 0;
		}
		else
		{
			strCw[iCw] = strMw[i];
			iCw++;
		}
	}
	strCw[iCw] = '\0';

	return len;
}

/******************************************************************************/
//	名称：Decry
//	功能：单表置换解密
//  参数：strCw是以0结尾的文本密文，长度len<=1024；strMw是明文缓冲区，大小Mlen
//	返回：更新成功返回明文长度，否则返回0
//  备注：只对密文中的字母字符进行解密

/******************************************************************************/
int Decry( char *strCw, int len,  char *strMw, int Mlen )
{
	int i=0,j=0;
	int iMw = 0;

	if( strCw==NULL || strMw==NULL || len>1024 || len<=0 || Mlen<len)
	{
		printf( "单表置换解密输入错误!\n" );
		return -1;
	}

	memset( strMw, 0,  Mlen );
	for( i=0; i<len; i++ )
	{
		if( strCw[i] >=65 && strCw[i] <=90 )
		{
			for( j=0; j<26; j++ )
			{
				if( strCw[i] == chrTab[j] )
				{
					//! 明文大写输入，密文小写输出
					strMw[iMw] = (char)('a'+j);
					iMw++;
				}
			}
		}
		else if( strMw[i] >= 97 && strMw[i] <=122 )
		{
			//! 明文小写输入，密文大写输出
			strCw[iMw] = chrTab[ strMw[i]-'a' ];
			iMw++;
		}
		else
		{
			strMw[iMw] = strCw[i];
			iMw++;
		}
	}
	strMw[iMw] = '\0';

	return len;

}