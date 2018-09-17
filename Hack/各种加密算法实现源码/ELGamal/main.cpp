/******************************************************************************/
//	工程：ELGamal
//	功能：ELGamal加、解密文件
//  作者：jlcss|ExpNIS

/******************************************************************************/

#include <stdio.h>
#include <afxwin.h>
#include <assert.h>
#include <windows.h>
#include "elgamal.h"

//! 宏定义，密文输出文件
#define DECRY_FILE "ELGamal加密密文.txt"
//! 宏定义，明文输出文件
#define ENCRY_FILE "ELGamal解密明文.txt"

/******************************************************************************/
//	名称：usage
//	功能：帮助信息
//  参数：应用程序名称
//	返回：提示信息

/******************************************************************************/
void Usage( const char *appname )
{
	//! 生成公私钥对
	printf( "\n\tusage: elgamal -k 私钥X\n" );
	printf( "\tusage: elgamal -e 明文文件 公钥\n" );
	printf( "\tusage: elgamal -d 密文文件 私钥\n" );
}

/******************************************************************************/
//	名称：CheckParse
//	功能：校验应用程序入口参数
//  参数：argc等于main主函数argc参数，argv指向main主函数argv参数
//	返回：
//  备注：简单的入口参数校验

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
//	名称：FileIn
//	功能：读取磁盘文件到内存
//  参数：strFile:文件名称；inBuff:指向文件输出缓冲区
//	返回：文件大小

/******************************************************************************/
int FileIn( const char *strFile, unsigned char *&inBuff )
{
	int iFileLen=0;

	//! 打开密文文件
	CFile file( strFile, CFile::modeRead );
	iFileLen = ( int )file.GetLength();
	if( iFileLen>1024 )
	{
		printf( "该文件长度大于1024，只导入前1024个字符!\n" );
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
//	名称：FileOut
//	功能：加密结果输出到当前目录磁盘文件中
//  参数：strOut指向输出字符缓冲区，输出大小len，strFile为输出文件
//	返回：无

/******************************************************************************/
void FileOut( int c1, int *c2, int iRead )
{
	int i;
	char buffer[12];
	CString strCw;

	//! 转换密文输出格式为字符数组
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

	//! 输出到文件
	CFile outfile( DECRY_FILE , CFile::modeCreate | CFile::modeWrite );
	outfile.Write( strCw , strCw.GetLength() );
}

/******************************************************************************/
//	名称：AnalyseCryptograph
//	功能：分析密文格式
//  参数：inBuff:指向密文内存缓冲区；iLength:密文大小(字节)；
//		  c1:密文c1；c2:密文c2
//	返回：密文分析通过返回密文数据段数目，否则返回0
//
/******************************************************************************/
int AnalyseCryptograph( const unsigned char *inBuff, int iLength, int &c1, int *&c2 )
{
	int i = 0;
	CString strC1;
	strC1.Empty();

	//! 提取c1
	for( i=4; i<iLength; i++ )
	{
		//! 从第4字节开始提取，直到“.”符
		if( inBuff[i] != '.' )
			strC1 += inBuff[i];
		else
			break;
	}
	i += 6;
	strC1 += '\0';
	c1 = ::atoi( strC1 );

	//! 提取c2
	CString strTemp;
	strTemp.Empty();

	for( i; i<iLength; i++ )
		strTemp += inBuff[i];
	int tempLen = strTemp.GetLength();
	if( tempLen == 0 )
		return false;

	//! c2数据段数目
	int c2Num = 0;
	for( i=0; i<tempLen; i++ )
	{
		if( strTemp.GetAt(i) == ',' )
			c2Num++;
	}
	c2Num++;

	//! c2数据
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
				//! 读密文时出现错误
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


//! 主程序(Use MFC in a Shared DLL)
int main( int argc, char **argv )
{
	//! 在此处填写代码......

	return 0;
}