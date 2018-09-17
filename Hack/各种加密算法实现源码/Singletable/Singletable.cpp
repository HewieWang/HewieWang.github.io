#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "singletable.h"

char chrTab[26] = {'A','B', 'C', 'D','E','F','G','H','I','J','K','L','M','N',
					'O','P','Q','R','S','T','U','V','W','X','Y','Z'};

/******************************************************************************/
//	���ƣ�updateChrTab
//	���ܣ������û���
//  ������phrase����0��β����Կ���飬����<=26, a~z��A~Z
//	���أ����³ɹ�����true�����򷵻�false
//  ��ע����Կ���鲻��������ظ��ַ�

/******************************************************************************/
bool UpdateChrTab( char *phrase, int plen )
{
	int i=0,j=0;
	char c,h;
	if( plen>26)
		return false;

	//! �����ڴ�ռ䣬������Կ������д�����Կ����
	char *strbak = new char[plen+1];
	if( strbak == NULL )
		return false;

	memcpy( strbak, phrase, plen );
	strbak[plen] = '\0';

	for(i=0; i<plen; i++)
	{
		c = phrase[i];
		//! ת��СдΪ��д��ĸ
		if( c >= 97 && c <= 122 ) // a~z
			strbak[i] -= 32;
		else if( c >= 65 && c <=90 ); // A~Z
		else
		{
			//! ����ĸ�ַ�
			printf( "������ַ� %c ���Ϸ����û�ֻ�ܷ�����Ӣ����ĸ֮��", c );
			delete [] strbak;
			return false;
		}
	}

	//! ����ظ��ַ�
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
				printf( "�ַ� %c �ظ�����\n", c );
				delete [] strbak;
				return false;
			}
		}
	}

	//! ����ת����
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
//	���ƣ�Encry
//	���ܣ������û�����
//  ������strMw����0��β���ı����ģ�����len<=1024��strCw�����Ļ���������СClen
//	���أ����³ɹ��������ĳ��ȣ����򷵻�0
//  ��ע��ֻ�������е���ĸ�ַ����м���

/******************************************************************************/
int Encry( char *strMw, int len,  char *strCw, int Clen )
{
	int i=0,j=0;
	int iCw = 0;

	if( strMw==NULL || strCw==NULL || len>1024 || len<=0 || Clen<len)
	{
		printf( "�����û������������!\n" );
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
					//! ���Ĵ�д���룬����Сд���
					strCw[iCw] = (char)('a'+j);
					iCw++;
				}
			}
		}
		else if( strMw[i] >= 97 && strMw[i] <=122 )
		{
			//! ����Сд���룬���Ĵ�д���
			strCw[iCw] = chrTab[ strMw[i]-'a' ];
			iCw++;
		}
		else if( (strMw[i] > 127) ||
				 (strMw[i] < 32 && strMw[i] != 0x0a && strMw[i] != 0x0d ))
		{
			printf( "singtable���ܹ���ĳЩ�����ַ����м��ܲ���!\n" );
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
//	���ƣ�Decry
//	���ܣ������û�����
//  ������strCw����0��β���ı����ģ�����len<=1024��strMw�����Ļ���������СMlen
//	���أ����³ɹ��������ĳ��ȣ����򷵻�0
//  ��ע��ֻ�������е���ĸ�ַ����н���

/******************************************************************************/
int Decry( char *strCw, int len,  char *strMw, int Mlen )
{
	int i=0,j=0;
	int iMw = 0;

	if( strCw==NULL || strMw==NULL || len>1024 || len<=0 || Mlen<len)
	{
		printf( "�����û������������!\n" );
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
					//! ���Ĵ�д���룬����Сд���
					strMw[iMw] = (char)('a'+j);
					iMw++;
				}
			}
		}
		else if( strMw[i] >= 97 && strMw[i] <=122 )
		{
			//! ����Сд���룬���Ĵ�д���
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