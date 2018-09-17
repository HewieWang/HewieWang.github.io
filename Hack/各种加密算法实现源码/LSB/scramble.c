/*********************************************************************
  Scrambling routines

*********************************************************************/


#include "lsb.h"
#include "scramble.h"
#include "StdAfx.h"

/*********************************************************************
  Scramble: 置乱函数，字节的线性变换 = 加上经过编码(simple 'cipher 
  					block feedback')的数值之后的8-比特向量
  parameter: 字节 = 输入的位向量
  returns:   字节 = 输出的位向量
*********************************************************************/

UBYTE Scramble (UBYTE BitVector)
{
  static UBYTE m[8] = {178, 155, 106, 39, 153, 116, 199, 89};
  static UBYTE r = 0;
  int i;

  BitVector ^= r;             /* 'CBF' 模式*/
  r = 105;
  for (i = 0; i < 8; i++)
    if (BitVector & (1u << i))
      r ^= m[i];

  return r;
}


/*********************************************************************
  DeScramble: 置乱恢复函数，字节的线性变换 = 8-比特向量连续加已经编码过的数值(CFB)
  parameter: 字节 = 输入的位向量
  returns:   字节 = 输出的位向量
*********************************************************************/

UBYTE DeScramble (UBYTE BitVector)
{
  static UBYTE m[8] = {97, 18, 163, 186, 47, 216, 116, 228};
  static UBYTE last = 0;
  UBYTE r;
  int i;

  r = 119;
  for (i = 0; i < 8; i++)
    if (BitVector & (1u << i))
      r ^= m[i];

  r ^= last;
  last = BitVector;
  return r;
}


/*********************************************************************
  ResetScrambler: 重置Scramble和DeScramble程序的内部参数为零
  parameter: none
  returns:   nothing
*********************************************************************/

void ResetScrambler (void)
{
  DeScramble (0);
  Scramble (Scramble (0) ^ DeScramble (0));
}
