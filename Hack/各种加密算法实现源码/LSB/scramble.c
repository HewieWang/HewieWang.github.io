/*********************************************************************
  Scrambling routines

*********************************************************************/


#include "lsb.h"
#include "scramble.h"
#include "StdAfx.h"

/*********************************************************************
  Scramble: ���Һ������ֽڵ����Ա任 = ���Ͼ�������(simple 'cipher 
  					block feedback')����ֵ֮���8-��������
  parameter: �ֽ� = �����λ����
  returns:   �ֽ� = �����λ����
*********************************************************************/

UBYTE Scramble (UBYTE BitVector)
{
  static UBYTE m[8] = {178, 155, 106, 39, 153, 116, 199, 89};
  static UBYTE r = 0;
  int i;

  BitVector ^= r;             /* 'CBF' ģʽ*/
  r = 105;
  for (i = 0; i < 8; i++)
    if (BitVector & (1u << i))
      r ^= m[i];

  return r;
}


/*********************************************************************
  DeScramble: ���һָ��������ֽڵ����Ա任 = 8-���������������Ѿ����������ֵ(CFB)
  parameter: �ֽ� = �����λ����
  returns:   �ֽ� = �����λ����
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
  ResetScrambler: ����Scramble��DeScramble������ڲ�����Ϊ��
  parameter: none
  returns:   nothing
*********************************************************************/

void ResetScrambler (void)
{
  DeScramble (0);
  Scramble (Scramble (0) ^ DeScramble (0));
}
