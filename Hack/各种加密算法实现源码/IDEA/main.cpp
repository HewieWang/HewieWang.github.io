/******************************************************************************/
//	工程：IDEA
//	功能：IDEA加、解密文件
//  作者：jlcss|ExpNIS

/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <afxwin.h>
#include "idea.h"

#define DECRYPT_FILE "IDEA加密密文.txt"
#define ENCRYPT_FILE "IDEA解密明文.txt"
//! 约束文件最大2M
#define MAX_FILE	1024*1024*2

/******************************************************************************/
//	名称：usage
//	功能：帮助信息
//  参数：应用程序名称
//	返回：提示信息

/******************************************************************************/
void Usage( const char *appname )
{
	printf( "\n\tusage: idea -e 明文文件 128位密钥\n" );
	printf( "\tusage: idea -d 密文文件 128位密钥\n" );
}

/******************************************************************************/
//	名称：FileIn
//	功能：读取磁盘文件到内存
//  参数：strFile:文件名称；inBuff:指向文件内容缓冲区
//	返回：实际读取内容大小(字节)

/******************************************************************************/
int FileIn( const char *strFile, unsigned char *&inBuff )
{
	int iFileLen=0, iBuffLen=0;

	//! 打开密文文件
	CFile file( strFile, CFile::modeRead );
	iFileLen = ( int )file.GetLength();
	if( iFileLen>MAX_FILE )
	{
		printf( "文件长度不能大于 %dM,!\n", MAX_FILE/(1024*1024) );
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
//	名称：FileOut
//	功能：加/解密结果输出到当前目录磁盘文件中
//  参数：strOut指向输出字符缓冲区，输出大小len，strFile为输出文件
//	返回：无

/******************************************************************************/
void FileOut( const void *strOut, int len, const char *strFile )
{
	//! 输出到文件
	CFile outfile( strFile , CFile::modeCreate | CFile::modeWrite );
	outfile.Write( strOut , len );
	outfile.Close();
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
	if( (argc != 4) || 
		(argv[1][1]!='e' && argv[1][1]!='d') ||
		(strlen( argv[3] ) >(128/8) ) )
	{
		Usage( argv[0] );
		return false;
	}

	return true;
}

//! 程序主函数(Use MFC in a Shared DLL)
int main( int argc, char **argv )
{
	//! 在此处填写代码......

	return true;
}