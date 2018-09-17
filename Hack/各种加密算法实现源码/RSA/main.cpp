/******************************************************************************/
//	���̣�RSA
//	���ܣ�RSA�ӡ������ļ�
//  ���ߣ�jlcss|ExpNIS

/******************************************************************************/

#include <stdio.h>
#include <afxwin.h>
#include <math.h>
#include "rsa.h"

#define DECRYPT_FILE "RSA��������.txt"
#define ENCRYPT_FILE "RSA��������.txt"
//! Լ���ļ����2M
#define MAX_FILE	1024*1024*2

/******************************************************************************/
//	���ƣ�usage
//	���ܣ�������Ϣ
//  ������Ӧ�ó�������
//	���أ���ʾ��Ϣ

/******************************************************************************/
void Usage( const char *appname )
{
	printf( "\n\tusage:rsa -k ����P ����Q\n" );
	printf( "\tusage: rsa -e �����ļ� ��Կe ��Կn\n" );
	printf( "\tusage: rsa -d �����ļ� ˽Կd ˽Կn\n" );
}

/******************************************************************************/
//	���ƣ�IsNumber
//	���ܣ��ж������ַ�����
//  ������strNumber:�ַ�����
//	���أ������������鷵��true�����򷵻�false;

/******************************************************************************/
bool IsNumber( const char *strNumber )
{
	unsigned int i;

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
//	���ƣ�IsPrimeNumber
//	���ܣ��ж�����
//  ������num: ��������
//	���أ���������true�����򷵻�false;

/******************************************************************************/
bool IsPrimeNumber( unsigned int num )
{
	unsigned int i;
	if( num <= 1 )
		return false;

	unsigned int sqr = (unsigned int)sqrt((double)num);
	for( i = 2; i <= sqr; i++ )
	{
		if( num % i == 0 )
			return false;
	}

	return true;
}

/******************************************************************************/
//	���ƣ�FileIn
//	���ܣ���ȡ�����ļ����ڴ�
//  ������strFile:�ļ����ƣ�inBuff:ָ���ļ����ݻ�����
//	���أ�ʵ�ʶ�ȡ���ݴ�С(�ֽ�)

/******************************************************************************/
int FileIn( const char *strFile, unsigned char *&inBuff )
{
	int iFileLen=0, iBuffLen=0;

	//! �������ļ�
	CFile file( strFile, CFile::modeRead );
	iFileLen = ( int )file.GetLength();
	if( iFileLen>MAX_FILE )
	{
		printf( "�ļ����Ȳ��ܴ��� %dM,!\n", MAX_FILE/(1024*1024) );
		goto out;
	}

	if( iFileLen % 8 != 0 )
		iBuffLen = ( iFileLen/8+1 ) * 8;
	else
		iBuffLen = iFileLen;

	inBuff = new unsigned char[iBuffLen];
	if( !inBuff )
		goto out;

	ZeroMemory( inBuff, iBuffLen );

	file.Read( inBuff, iFileLen );
	file.Close();

out:
	return iBuffLen;
}

/******************************************************************************/
//	���ƣ�FileOut
//	���ܣ���/���ܽ���������ǰĿ¼�����ļ���
//  ������strOutָ������ַ��������������Сlen��strFileΪ����ļ�
//	���أ���

/******************************************************************************/
void FileOut( const void *strOut, int len, const char *strFile )
{
	//! ������ļ�
	CFile outfile( strFile , CFile::modeCreate | CFile::modeWrite );
	outfile.Write( strOut , len );
	outfile.Close();
}

/******************************************************************************/
//	���ƣ�CheckParse
//	���ܣ�У��Ӧ�ó�����ڲ���
//  ������argc����main������argc������argvָ��main������argv����
//	���أ��������Ϸ�����true�����򷵻�false
//  ��ע���򵥵���ڲ���У��

/******************************************************************************/
bool CheckParse( int argc, char** argv )
{
	bool bRes = false;

	if( argc != 4 && argc != 5 )
		goto out;

	if( argc == 4 && argv[1][1] == 'k' )
	{
		//! ���ɹ���˽Կ��
		if( !IsNumber( argv[2] ) || 
			!IsNumber( argv[3] ) ||
			atoi( argv[2] ) > MAX_PRIME ||
			atoi( argv[3] ) > MAX_PRIME )
			goto out;
	}
	else if( (argc == 5) && (argv[1][1] == 'e' || argv[1][1] == 'd') )
	{
		//! ���ܡ����ܲ���
		if( !IsNumber( argv[3] ) ||
			!IsNumber( argv[4] ) ||
			atoi( argv[3] ) > MAX_NUM ||
			atoi( argv[4] ) > MAX_NUM )
			goto out;
	}
	bRes = true;

out:
	return bRes;
}

/******************************************************************************/
//	���ƣ�kOption1
//	���ܣ�����kѡ�������������P��Q����˽Կd����
//  ������uiP: ������ڲ���P; uiQ: ������ڲ���Q
//	���أ�ִ����ȷ��������˽Կ��Ŀ�����򷵻�0

/******************************************************************************/
unsigned int kOption1( unsigned int uiP, unsigned int uiQ )
{
	unsigned int uiRes = 0;

	if( !IsPrimeNumber( uiP ) )
	{
		printf( "P�������P����Ϊ(0, %d]����", MAX_PRIME );
		return uiRes;
	}
	if( !IsPrimeNumber( uiQ ) )
	{
		printf( "Q�������Q����Ϊ(0, %d]����", MAX_PRIME );
		return uiRes;
	}
	if( uiP == uiQ )
	{
		printf( "����P������Q��ͬ�������׸��ݹ�Կn��ƽ���ó�����P��Q�����ּ��ܲ���ȫ�����������!\n" );
		return uiRes;
	}
	printf( "��������˽Կd����......\n" );
	uiRes = MakePrivatedKeyd( uiP, uiQ );

	return uiRes;
}

//! ����������(Use MFC in a Shared DLL)
int main( int argc, char **argv )
{
	//! �ڴ˴���д����......

	return true;
}