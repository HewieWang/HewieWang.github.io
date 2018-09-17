/******************************************************************************/
//	工程：Caesar
//	功能：Caesar加、解密文件
//  作者：jlcss|ExpNIS

/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Caesar.h"

#define CHAR_SIZE		26
#define DECRYPT_FILE	"Caesar密文.txt"
#define ENCRYPT_FILE	"Caesar明文.txt"

/******************************************************************************/
//	名称：usage
//	功能：帮助信息
//  参数：应用程序名称
//	返回：提示信息

/******************************************************************************/
void Usage( const char *appname )
{
	printf( "\n\tusage: caesar -e 明文文件 密钥k\n" );
	printf( "\tusage: caesar -d 密文文件 密钥k\n" );
}

/******************************************************************************/
//	名称：FileOut
//	功能：加/解密结果输出到当前目录磁盘文件中
//  参数：strOut指向输出字符缓冲区，输出大小len，strFile为输出文件
//	返回：更新成功返回实际输出长度，否则返回0

/******************************************************************************/
int FileOut( const char *strOut, int len, const char *strFile )
{
	FILE *fp = NULL;
	int iWrite=0;

	if( strOut == NULL || len ==0 )
		return false;

	//! 打开文件句柄
	if( (fp = fopen( strFile,"w" )) == NULL )	// 文本模式写
		return false;

	iWrite = fwrite( strOut, 1, len, fp );
	fclose(fp);

	if( iWrite>0 )
		return true;
	else
		return false;
}

/******************************************************************************/
//	名称：CheckParse
//	功能：校验应用程序入口参数
//  参数：argc等于main主函数argc参数，argv指向main主函数argv参数
//	返回：若参数合法返回true，否则返回false
//  备注：简单的入口参数校验

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
//	名称：ContrastTable
//	功能：由密钥iKey生成密文对照表
//  参数：iKey: 密钥k; outTable: 密文对照表
//	返回：无
//  备注：密文对照表均为大写字母

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
//	名称：ShowTable
//	功能：密文对照表控制台输出显示
//  参数：outTable: 密文对照表
//	返回：无

/******************************************************************************/
void ShowTable( const char *outTable )
{
	int i=0;

	printf( "明文和密文对照表\n" );
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

//! 程序主函数
int main( int argc, char **argv )
{
	//! 在此处填写代码......

	return 0;
}