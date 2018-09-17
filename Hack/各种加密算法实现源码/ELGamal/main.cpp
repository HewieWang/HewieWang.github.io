/******************************************************************************/
//	���̣�ELGamal
//	���ܣ�ELGamal�ӡ������ļ�
//  ���ߣ�jlcss|ExpNIS

/******************************************************************************/

#include <stdio.h>
#include <afxwin.h>
#include <assert.h>
#include <windows.h>
#include "elgamal.h"

//! �궨�壬��������ļ�
#define DECRY_FILE "ELGamal��������.txt"
//! �궨�壬��������ļ�
#define ENCRY_FILE "ELGamal��������.txt"

/******************************************************************************/
//	���ƣ�usage
//	���ܣ�������Ϣ
//  ������Ӧ�ó�������
//	���أ���ʾ��Ϣ

/******************************************************************************/
void Usage( const char *appname )
{
	//! ���ɹ�˽Կ��
	printf( "\n\tusage: elgamal -k ˽ԿX\n" );
	printf( "\tusage: elgamal -e �����ļ� ��Կ\n" );
	printf( "\tusage: elgamal -d �����ļ� ˽Կ\n" );
}

/******************************************************************************/
//	���ƣ�CheckParse
//	���ܣ�У��Ӧ�ó�����ڲ���
//  ������argc����main������argc������argvָ��main������argv����
//	���أ�
//  ��ע���򵥵���ڲ���У��

/******************************************************************************/
char CheckParse( int argc, char** argv )
{
	if( argc == 3 && argv[1][1] == 'k' ||
		argc == 4 && argv[1][1] == 'e' ||
		argc == 4 && argv[1][1] == 'd')
		return argv[1][1];
	else
	{
		Usage( argv[0] );
		return '\0';
	}
}

/******************************************************************************/
//	���ƣ�FileIn
//	���ܣ���ȡ�����ļ����ڴ�
//  ������strFile:�ļ����ƣ�inBuff:ָ���ļ����������
//	���أ��ļ���С

/******************************************************************************/
int FileIn( const char *strFile, unsigned char *&inBuff )
{
	int iFileLen=0;

	//! �������ļ�
	CFile file( strFile, CFile::modeRead );
	iFileLen = ( int )file.GetLength();
	if( iFileLen>1024 )
	{
		printf( "���ļ����ȴ���1024��ֻ����ǰ1024���ַ�!\n" );
		iFileLen = 1024;
	}

	inBuff = new unsigned char[iFileLen+1];
	if( !inBuff )
		return 0;

	file.Read( inBuff, iFileLen );
	file.Close();
	inBuff[iFileLen] = 0;

	return iFileLen;
}

/******************************************************************************/
//	���ƣ�FileOut
//	���ܣ����ܽ���������ǰĿ¼�����ļ���
//  ������strOutָ������ַ��������������Сlen��strFileΪ����ļ�
//	���أ���

/******************************************************************************/
void FileOut( int c1, int *c2, int iRead )
{
	int i;
	char buffer[12];
	CString strCw;

	//! ת�����������ʽΪ�ַ�����
	strCw.Empty();
	strCw += "c1: ";
	_itoa( c1, buffer, 10);

	CString c1wTem(buffer);
	strCw += c1wTem;
	strCw += '.';
	strCw += "\r\n";
	strCw += "c2: ";
	for( i = 0; i < iRead; i++ )
	{
		strCw += _itoa( c2[i], buffer, 10 );
		if( i!=iRead-1 )
			strCw += ',';
	}
	strCw += '\0';

	//! ������ļ�
	CFile outfile( DECRY_FILE , CFile::modeCreate | CFile::modeWrite );
	outfile.Write( strCw , strCw.GetLength() );
}

/******************************************************************************/
//	���ƣ�AnalyseCryptograph
//	���ܣ��������ĸ�ʽ
//  ������inBuff:ָ�������ڴ滺������iLength:���Ĵ�С(�ֽ�)��
//		  c1:����c1��c2:����c2
//	���أ����ķ���ͨ�������������ݶ���Ŀ�����򷵻�0
//
/******************************************************************************/
int AnalyseCryptograph( const unsigned char *inBuff, int iLength, int &c1, int *&c2 )
{
	int i = 0;
	CString strC1;
	strC1.Empty();

	//! ��ȡc1
	for( i=4; i<iLength; i++ )
	{
		//! �ӵ�4�ֽڿ�ʼ��ȡ��ֱ����.����
		if( inBuff[i] != '.' )
			strC1 += inBuff[i];
		else
			break;
	}
	i += 6;
	strC1 += '\0';
	c1 = ::atoi( strC1 );

	//! ��ȡc2
	CString strTemp;
	strTemp.Empty();

	for( i; i<iLength; i++ )
		strTemp += inBuff[i];
	int tempLen = strTemp.GetLength();
	if( tempLen == 0 )
		return false;

	//! c2���ݶ���Ŀ
	int c2Num = 0;
	for( i=0; i<tempLen; i++ )
	{
		if( strTemp.GetAt(i) == ',' )
			c2Num++;
	}
	c2Num++;

	//! c2����
	c2 = new int[c2Num];
	if( c2 == NULL )
		return false;

	char buffer[12];
	int j=0;
	int jj=0;
	int c2Tmp = 0;
	for( i=0; i<tempLen; i++ )
	{
		if( strTemp.GetAt(i) != ',' )
			buffer[jj] = strTemp.GetAt(i);
		else
		{
			if( j<c2Num )
			{
				c2Tmp = ::atoi( buffer );
				c2[j] = c2Tmp;
				j++;
				jj = -1;
			}
			else
				//! ������ʱ���ִ���
				return 0;
		}
		jj++;
		buffer[jj] = '\0';
	}
	if( j<c2Num )
	{
		c2Tmp = ::atoi(buffer);
		c2[j] = c2Tmp;
	}

	return c2Num;
}


//! ������(Use MFC in a Shared DLL)
int main( int argc, char **argv )
{
	//! �ڴ˴���д����......

	return 0;
}