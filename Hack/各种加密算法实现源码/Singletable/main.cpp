/******************************************************************************/
//	���̣�Singletable
//	���ܣ�Singletable�ӡ������ļ�
//  ���ߣ�jlcss|ExpNIS

/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#include "singletable.h"

#define DECRY_FILE "��������.txt"
#define ENCRY_FILE "��������.txt"

/******************************************************************************/
//	���ƣ�usage
//	���ܣ�������Ϣ
//  ������Ӧ�ó�������
//	���أ���ʾ��Ϣ

/******************************************************************************/
void Usage( const char *appname )
{
	printf( "\n\tusage: singletable -e �����ļ� ��Կ����\n" );
	printf( "\tusage: singletable -d �����ļ� ��Կ����\n" );
}

/******************************************************************************/
//	���ƣ�FileOut
//	���ܣ���/���ܽ���������ǰĿ¼�����ļ���
//  ������strOutָ������ַ��������������Сlen��strFileΪ����ļ�
//	���أ����³ɹ�����ʵ��������ȣ����򷵻�0

/******************************************************************************/
int FileOut( const char *strOut, int len, const char *strFile )
{
	FILE *fp = NULL;
	int iWrite=0;

	if( strOut == NULL || len ==0 )
		return false;

	//! ���ļ����
	if( (fp = fopen( strFile,"w" )) == NULL )	// �ı�ģʽд
		return false;

	iWrite = fwrite( strOut, 1, len, fp );
	fclose(fp);

	if( iWrite>0 )
		return true;
	else
		return false;
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
	if( argc != 4 )
	{
		Usage( argv[0] );
		return false;
	}

	if( argv[1][1]!='e' && argv[1][1]!='d')
	{
		Usage( argv[0] );
		return false;
	}

	return true;
}

//! ����������
int main( int argc, char **argv )
{
	//! �ڴ˴���д����......

	return 0;
}