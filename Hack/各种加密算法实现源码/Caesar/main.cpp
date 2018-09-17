/******************************************************************************/
//	���̣�Caesar
//	���ܣ�Caesar�ӡ������ļ�
//  ���ߣ�jlcss|ExpNIS

/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Caesar.h"

#define CHAR_SIZE		26
#define DECRYPT_FILE	"Caesar����.txt"
#define ENCRYPT_FILE	"Caesar����.txt"

/******************************************************************************/
//	���ƣ�usage
//	���ܣ�������Ϣ
//  ������Ӧ�ó�������
//	���أ���ʾ��Ϣ

/******************************************************************************/
void Usage( const char *appname )
{
	printf( "\n\tusage: caesar -e �����ļ� ��Կk\n" );
	printf( "\tusage: caesar -d �����ļ� ��Կk\n" );
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
	if( argc != 4 || 
		(argv[1][1]!='e' && argv[1][1]!='d') )
	{
		Usage( argv[0] );
		return false;
	}

	return true;
}

/******************************************************************************/
//	���ƣ�ContrastTable
//	���ܣ�����ԿiKey�������Ķ��ձ�
//  ������iKey: ��Կk; outTable: ���Ķ��ձ�
//	���أ���
//  ��ע�����Ķ��ձ��Ϊ��д��ĸ

/******************************************************************************/
void ContrastTable( int iKey, char *outTable )
{
	int i=0, k= 0;

	if( !outTable )
		return;

	memset( outTable, 0, CHAR_SIZE );
	k = iKey % CHAR_SIZE;
	for( i=0; i<CHAR_SIZE; i++ )
	{
		outTable[i] = 'A' + i;
	}

	for( i=0; i<CHAR_SIZE; i++ )
	{
		if( outTable[i]+k > 'Z' )
			outTable[i] = outTable[i]+k -'Z'+'A'-1;
		else
			outTable[i] = (outTable[i]+k);
	}
	outTable[i] = '\0';
}

/******************************************************************************/
//	���ƣ�ShowTable
//	���ܣ����Ķ��ձ����̨�����ʾ
//  ������outTable: ���Ķ��ձ�
//	���أ���

/******************************************************************************/
void ShowTable( const char *outTable )
{
	int i=0;

	printf( "���ĺ����Ķ��ձ�\n" );
	for( i=0; i<CHAR_SIZE; i++ )
	{
		printf( "%c ", 'a'+i );
	}
	printf( "\n" );

	for( i=0; i<CHAR_SIZE; i++ )
	{
		printf( "%c ", outTable[i] );
	}
	printf( "\n" );
}

//! ����������
int main( int argc, char **argv )
{
	//! �ڴ˴���д����......

	return 0;
}