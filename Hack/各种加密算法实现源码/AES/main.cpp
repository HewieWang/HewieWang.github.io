/******************************************************************************/
//	���̣�AES
//	���ܣ�AES�ӡ������ļ�
//  ���ߣ�jlcss|ExpNIS

/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <afxwin.h>

#include "aes.h"

#define DECRYPT_FILE "AES��������.txt"
#define ENCRYPT_FILE "AES��������.txt"
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
	printf( "\n\tusage: aes -e �����ļ� 128λ��Կ\n" );
	printf( "\tusage: aes -d �����ļ� 128λ��Կ\n" );
}

/******************************************************************************/
//	���ƣ�FileOut
//	���ܣ���/���ܽ���������ǰĿ¼�����ļ���
//  ������strOutָ������ַ��������������Сlen��strFileΪ����ļ�
//	���أ����³ɹ�����ʵ��������ȣ����򷵻�0

/******************************************************************************/
int FileOut( const void *strOut, int len, const char *strFile )
{
	FILE *fp = NULL;
	int iWrite=0;

	if( strOut == NULL || len ==0 )
		goto out;

	//! ���ļ����
	if( (fp = fopen( strFile,"wb" )) == NULL ) // ������д
		goto out;

	iWrite = fwrite( strOut, 1, len, fp );
	fclose(fp);

out:
	return iWrite;
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
	if( (argc != 4) || 
		(argv[1][1]!='e' && argv[1][1]!='d') ||
		(strlen( argv[3] ) >(128/8) ) )
	{
		Usage( argv[0] );
		return false;
	}

	return true;
}

//! ����������(Use MFC in a Shared DLL)
int main( int argc, char **argv )
{
	//! �ڴ˴���д����......

	return 0;
}