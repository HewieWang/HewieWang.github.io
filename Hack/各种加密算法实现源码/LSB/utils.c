/*********************************************************************
  Ӧ�ó���

  LSB�㷨�еĲ��ֱ�Ҫ����
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lsb.h"
#include "utils.h"
#include "scramble.h"
#include "StdAfx.h"

/* globals */
int hiding = TRUE, verbose = 1;              /* ������ѡ�� command line options */
FILE    *StegFile;                           /* ����Ƕ��ˮӡ���ļ������¼��ͼ���āvdata file to work on*/
FILE    *SecretFile;            /* ��������������Ϣ���ʱ)�ļ������¼��ˮӡ�āv(temporary) file with secret data*/
WORD32  StegLength = 0;             /* ����)���ص�������Ϣ�ĳ��� length of hidden/to hide data*/
WORD32  StegLeft = 0;        /* ��δ�����������������Ϣ�ĳ���part of StegLength not yet processed*/
WORD32  MediaLength = 0;        /* �����ڴ洢��������Ϣ���� data points available for storage*/
size_t  DataSize;                            /* ÿ�����ݵ�ı��ؔ`bytes per data point*/
int     MaxBitsAllowed;                 /* ���õı��أ����ݞ�bits/data point available*/
int     LastLength = FALSE;    /* ��־λ�� ��������е���󳤶�λ flag: last length byte in progress*/
void    (*ModData) (void *, size_t);     /* ����: �޸Ļ��� funct ptr: modify buffer*/
unsigned int (*GetBits) (int);
                      /* ����:�����ݵ�����ȡ������ funct ptr: extracts bit(s) from data points*/
UBYTE   (*NextByte) (void);  /* ����: ��ȡ��һ��Ҫ���صı�������funct ptr: fetches next byte to hide*/
void    (*OutputByte) (UBYTE);  /* ����: delivers restored byte */

/* ��̬����*/
        /*  ��ʼ�����ط��䷽��Ϊ1���أ����ݵ� initialize bit distribution algorithm to 1 bit/data point*/
static long IndicatorSum = 0, BitsNeeded = 1, BytesAvail = 1;


/*********************************************************************
  ����ע�͸���˵������һ���Ǻ��������ڶ����Ǻ�������������Ĳ������������Ǻ����ķ���ֵ?
  *********************************************************************
  ErrorExit: ��������������ʾ������Ϣ���˃�
  						called on critical errors; prints error message and aborts
  parameter: �����˳����룬ָ����ʾ��Ϣ���ݵ�ָ��Programm Exit Code, Pointer to Message
  returns:   ����
*********************************************************************/

void ErrorExit (int ExitCode, char *ErrorMessage)
{
  if (verbose > 1)
    fputs ("\n", stderr);

  fputs (ErrorMessage, stderr);
  exit (ExitCode);
}


/*********************************************************************
  UnexpEOF:  ������EOFʱ�����ʣ��������һ��������Ϣ���˃�
  						is called when read past EOF, outputs an error message and aborts
  parameter: ��
  returns:   ����
*********************************************************************/

void UnexpEOF (void)
{
  ErrorExit (UNEXPEOF,
   "Error: Unexpected end of file encountered - aborting.\n");
}


/*********************************************************************
  NoMemory:  �����ڴ�ʧ��ʱ������
  						is called when malloc fails or SetBlock ends up with a zero byte block
  parameter:	 ��
  returns:   ����
*********************************************************************/

void NoMemory (void)
{
  ErrorExit (OUTOFMEMORY, "Error: Not enough memory - aborting.\n");
}


/*********************************************************************
  GetStegByte: ��ͼ���ļ����w��������
  							reads 8 bit from media file
  parameter: 		 ��
  returns:     ��һ�����~
*********************************************************************/

UBYTE GetStegByte (void)
{
  int b;

  if ((b = getc (StegFile)) == EOF)
    UnexpEOF ();

  return (UBYTE) b;
}


/*********************************************************************
  GetStegWord: ��ͼ���ļ��ж�һ?6��������
  							reads one 16 bit word from media file
  parameter:			��
  returns:     ���������~
*********************************************************************/

UWORD16 GetStegWord (void)
{
  UWORD16 value;

  if (fread (&value, 2, 1, StegFile) != 1)
    UnexpEOF ();

  return value;
}


/*********************************************************************
  GetStegLong: ��ͼ���ļ��ж�һ?2��������
  							reads one 32 bit word from media file
  parameter:			��
  returns:     ���ĸ����~
*********************************************************************/

UWORD32 GetStegLong (void)
{
  UWORD32 value;

  if (fread (&value, 4, 1, StegFile) != 1)
    UnexpEOF ();

  return value;
}


/*********************************************************************
  PutStegWord: ��6�������ݵ�ͼ���ļ���
  							 writes one 16 bit word to media file
  parameter: ������Ϣ��ָ���ļ���������ָ�O
  						data word, Pointer to FILE descriptor
  returns:   д�����ݵ�����
  						number of written bytes
*********************************************************************/

size_t PutStegWord (UWORD16 value)
{
  return fwrite (&value, 2, 1, StegFile);
}


/*********************************************************************
  NextIs:   ���ַ������ļ��е���һ�ֽڽ��бȽϣ�ͨ��������������
  					compare string to next bytes in file, allways read the whole length
  parameter: ָ����Ҫ�Ƚ��ַ���λ�õ�ָ�O
  						pointer to a string to be matched
  returns:   �ȽϽ����־������ַ�����ͬΪ��flag, TRUE if equal
           			  ���ַ��Ƚ���ɺ����õ��ļ�ָ�O	file pointer set behind the characters compared
*********************************************************************/

int NextIs (char *pattern)
{
  int result = TRUE;

  while (*pattern)
    if (GetStegByte () != *pattern++)
      result = FALSE;

  return result;
}


/*********************************************************************
  TurnWheel: �����չ��־λ��һ��С�ָ���?
  					shows a little wheel as progress indicator
  parameter: none
  returns:   nothing
*********************************************************************/

void TurnWheel (void)
{
  static int    i = 3;
  static UBYTE  wheel[4] = {'-', '\\', '|', '/'};

  fputc (wheel[i = (i + 1) % 4], stderr);		//��wheel�ж�Ӧ���ַ�д��stderr?
  putc ('\b', stderr);
  fflush (stderr);													//���stderr����
}


/*********************************************************************
  BitsThisTime: ���㵱ǰ���ݵ�ı�������Ȼ��ͨ��һ�������㷨��line draw?
  							��������Щ��������ƽ�����䵽ͼƬ�����еĿ���������ȥ����?
						 calculates the number of bits in current data point;
             uses a 'line draw' algorithm to distribute all bits
             evenly among all available bytes
  parameter: none
  returns:   ���ر�	
  					bit number
*********************************************************************/

int BitsThisTime (void)
{
  ldiv_t btt;
  
  //��IndicatorSum + BitsNeeded����BytesAvail���������̸���btt.quot����������btt.rem
  btt = ldiv (IndicatorSum + BitsNeeded, BytesAvail);	

  IndicatorSum = btt.rem;

  return btt.quot;	//���������̷���
}


/*********************************************************************
  InitBitDist: ��ʼ�����ݷ����㷨���������ҵ����ݳ��ȺͿ��õ����ݵ�?
				 initializes bit distribution algorithm to found data
             length and available data points
  parameter: ��������е��ֽڳ���
  					number of length bytes processed
  returns:   nothing
*********************************************************************/

void InitBitDist (int LenByt)
{
  BitsNeeded = StegLength << 3;           // ����3λ������8 ����/1 ��
  BytesAvail = MediaLength - (LenByt << 3);	//����
  IndicatorSum = BytesAvail >> 1;
}


/*********************************************************************
  NextBits:  �Ӿ�̬ƫ�����ж�������ı�����������б�Ҫ��ƫ�����������������һ�ֽڣ���
						 delivers the required number of bits from a static shift
             register; when necessary, shift register is filled from
             left to right with next scrambled byte
  parameter: ���ؔ`	
  					number of bits
  returns:   ��Ҫ�����ͱ���
				 requested bits as integer
*********************************************************************/

unsigned int NextBits (int HowMany)
{
  static unsigned int ShiftReg = 0;	//��̬ƫ����
  static int TopBit = 0;
  unsigned int r;

  if (HowMany > TopBit)      /* ���������㣬�޷��������G*/
  {
    ShiftReg |= (unsigned int) Scramble ((*NextByte) ()) << TopBit;
    TopBit += 8;
  }

/*������������һ�δ��룬����������ÿ�δ�ShiftReg����ȡ��͵�
HowManyλ���ݣ���������ֵ��r����ֵ��ɺ�ShiftReg����
HowManyλ��TopBit��ȥHowMany��
*/

  return r;
}


/*********************************************************************
  ModDBytes: ��������Ϣ���뵽�ڴ���һ?���ص����ݿ�?
				 injects the secret data bits into a block of 8 bit data
             in memory
  parameter: ָ��UBYTE���������ָ�O ����Ĵ���
  					pointer to UBYTE array, size of array
  returns:   ��
*********************************************************************/

void ModDBytes (UBYTE *block, size_t count)
{
  int n;

  while (count--)
  {
    if (n = BitsThisTime ())
	/*��1�ĵ�Ŵ��ڣ����򲻴��ڣ��������ң�ֱ��Ϊ1
	(~0u << n)����˼�Ǹ���Ҫ�����λ��n�õ�һ��8λ����������
	��n=1��Ϊ11111110��������*block�����������������λ���䣬
	�������nλ����0������NextBits(n)������������ɽ���Ƕ���f
	����д��block��
 */
      *block = *block & (~0u << n) | NextBits (n);
    block++;
  }
}


/*********************************************************************
  ModDWords: ��������Ϣ���뵽�ڴ���һ?6���ص����ݿ�?
				 injects the secret data bits into a block of 16 bit data
             in memory
  parameter: ָ��int���������ָ�O ����Ĵ���
  returns:   ��
*********************************************************************/

void ModDWords (UWORD16 *block, size_t count)
{
  int n;

  while (count--)
  {
    if (n = BitsThisTime ())
      *block = *block & (~0u << n) | NextBits (n);
    block++;
  }
}


/*********************************************************************
  ModLBytes: ��������Ϣ���뵽�ڴ���һ?���ص����ݿ��У������һ�ֽڲ������ת�������ݴ���̎�
				 injects the length bits into a block of 8 bit data
             in memory, switches to data routine after last byte
  parameter: ָ��UBYTE���������ָ�O ����Ĵ���
  returns:   ��
*********************************************************************/

void ModLBytes (UBYTE *block, size_t count)
{
  static int BitsLeft = 0;

  while (count--)
  {
    *block = *block & ~1u | NextBits (1);
    block++;

    if (BitsLeft)                    /* �ڴ�������е�����߱������� */
    {
      if (--BitsLeft == 0)
      {
        ModData = (void (*) (void *, size_t)) ModDBytes;
        ModDBytes (block, count); /* �³�����ʣ������ݿ� rest of block with new routin*/
        return;
      }
    }
    else if (LastLength)                  /* ���һ���ֽ��еĵ�һ���Ƞd*/
      BitsLeft = 7;
  }
}


/*********************************************************************
  ModLWords: ��������Ϣ���뵽�ڴ���һ?6���ص����ݿ��У������һ�ֽڲ������ת�������ݴ���̎�
				 injects the length bits into a block of 16 bit data
             in memory, switches to data routine after last byte
  parameter: ָ��int���������ָ�O ����Ĵ���
  returns:   ��
*********************************************************************/

void ModLWords (UWORD16 *block, size_t count)
{
  static int BitsLeft = 0;

  while (count--)
  {
    *block = *block & ~1u | NextBits (1);
    block++;

    if (BitsLeft)                    /* last seven bits in progress */
    {
      if (--BitsLeft == 0)
      {
        ModData = (void (*) (void *, size_t)) ModDWords;
        ModDWords (block, count); /* rest of block with new routine */
        return;
      }
    }
    else if (LastLength)                  /* first bit of last byte */
      BitsLeft = 7;
  }
}


/*********************************************************************
  GetBitsFromByte: ��Ŀ���ļ��л�ȡ��һ���ֽڵ�LSB���ر�
									get LSB bit(s) from next byte of the disk file
  parameter: ��Ҫʹ�õ������Чλ����
  						number of Least Significant Bits to extract
  returns:   ����Ƕ���integer���͵ı�����
  						requested bits as integer
*********************************************************************/

/*��ѧ�����������һ��������룬��һ���������þ��Ǵ�Ŀ���ļ�
�л�ȡ�����ֽڵ�LSB����ֵ�����������Ѿ�������
unsigned int GetBitsFromByte (int Bits)
����Bits���Ǵ���LSB�㷨����Ҫȡ����ͼ�λ��ȡĿ���ļ���һ��
�����ݿɵ���GetStegByte������ѧ��Ҫ������Ҫ�ǰ����Bitsλ
�Ӹ��ֽ�ȡ�������ء�
�ں���������õ����²�������&�����������ȡ����0u��ʾ�޷��ŵ�
0��ͬ��1u��ʾ�޷��ŵ�1��<<��ʾ���ƣ�>>��ʾ����
*/
unsigned int GetBitsFromByte (int Bits)
{
	
}


/*********************************************************************
  GetBitsFromWord: ��Ŀ���ļ��л�ȡ��һ���ʣ�word����LSB���ر�
  								get LSB bit(s) from next word of the disk file
  parameter: ��Ҫʹ�õ������Чλ����
  returns:   ����Ƕ���integer���͵ı�����
*********************************************************************/

unsigned int GetBitsFromWord (int Bits)
{
  return (unsigned int) GetStegWord () & ~(~0u << Bits);
}


/*********************************************************************
  ExtractBits:  ��֮ǰ�޸ĵ���������ȡ�����صı���ֵ��Ȼ�����·���
  							���ǵ���ǰƫ�����У�����Щƫ�����Ѵ���ʱ�Գ��Ȼ���
  							ˮӡ�ļ�������޷�������
  					 extracts the hidden bits out of previously modified
             data and rearranges them to bytes in a local shift
             register that is emptied to length calculation or
             SecretFile when full
  parameter: none
  returns:   nothing
*********************************************************************/

void ExtractBits (void)
{
  static unsigned int ShiftReg = 0;
  static int TopBit = 0;
  int HowMany;

  HowMany = BitsThisTime ();

  ShiftReg  |= (*GetBits) (HowMany) << TopBit;
  TopBit += HowMany;

  if (TopBit >= 8)      /* another byte ready */
  {
    (*OutputByte) (DeScramble (ShiftReg & 0xFFu));
    ShiftReg >>= 8;
    TopBit -= 8;
  }
}


/*********************************************************************
  GetSecretByte:	���м��ļ��ж�ȡһ�����~
  							reads one byte from the temporary intermediate file
  parameter: none
  returns:   ��ȡ�����~
  						read byte
*********************************************************************/

UBYTE GetSecretByte (void)
{
  if (! StegLeft--)
    ErrorExit (OUTOFSYNC, "Error: Reading past end of data to hide.\n");

  return getc (SecretFile);
}


/*********************************************************************
  DataOut:  ��ˮӡ�ļ�д���ؽ����~
  					writes reconstructed byte to SecretFile
  parameter: byte
  returns:   nothing
*********************************************************************/

void DataOut (UBYTE which)
{
  if (StegLeft--)
    putc (which, SecretFile);
  else
    ErrorExit (OUTOFSYNC, "Error: Reading past end of media file.\n");
}


    /*EncodeLength/LengthOut�����ļ�����һֱ������269525055,��Ϊ
    8-fold�ı��������Ǳ�Ҫ�Ķ��������ڳ����ͱ���⵼��Ϊ��*/
    /* EncodeLength/LengthOut handle Lengths up to 269525055  */
    /* enough, as the 8-fold amount of bits is necessary and  */
    /* must fit into a 'long' value; avoids leading zeros     */


/*********************************************************************
  EncodeLength: �����������ݵĳ���Secretlength)����Ϊһ?�ֽڳ��Ļ���������?
  					 encodes the length of secret data (Secretlength) into
             a buffer of up to 4 bytes
  parameter: 	ָ�򻺳�����ָ��
  						pointer to buffer
  returns:  ʹ���˵��ֽڔ`
  					number of bytes used
*********************************************************************/

int EncodeLength (UBYTE *LenghtBytes)
{
  UWORD32 Len;

  Len = StegLength;

  if (Len < 64)			//�������ݳ���С��64
  {
    LenghtBytes[0] = (UBYTE) Len;
    return 1;
  }

  Len -= 64;
  LenghtBytes[0] = Len & 0xFF;
  Len >>= 8;

  if (Len < 160)		//�������ݳ���С��224
  {
    LenghtBytes[1] = (UBYTE) Len + 64;
    return 2;
  }

  Len -= 160;
  LenghtBytes[1] = Len & 0xFF;
  Len >>= 8;

  if (Len < 16)			//�������ݳ���С��240
  {
    LenghtBytes[2] = (UBYTE) Len + 224;
    return 3;
  }

  Len -= 16;
  LenghtBytes[2] = Len & 0xFF;
  Len >>= 8;
  LenghtBytes[3] = (UBYTE) Len + 240;
  return 4;
}


/*********************************************************************
  GetLengthByte: �����ھ�̬�������ﱣ��ľ������������һ���ֽ�����
  ����,�Գ��Ƚ��б������ر任Ϊ���м��ļ��ж�����w
  					 returns one byte of the encoded length kept in a
             static buffer; on 1st invocation, encode length; on last
             invocation, switch input to read from intermediate file
  parameter: none
  returns:   һ���������~
  					 one length byte
*********************************************************************/

UBYTE GetLengthByte (void)
{
  static UBYTE LengthBytes [4];
  static int BytesLeft = 0;

  /* �״ε���ʱ�����ֽ����a Load byte array on first call */
  if (BytesLeft == 0)
  {
    BytesLeft = EncodeLength (LengthBytes);
    InitBitDist (BytesLeft);      /* ׼�����ط��� */
  }

  /* ���������ĳ����ֽ�when this is the last length byte */
  if (--BytesLeft == 0)
  {
    LastLength = TRUE;                       /* ��־��Ϣ:������~*/
    fseek (SecretFile, 0L, SEEK_SET);
    NextByte = GetSecretByte;    /* ת����ʱ�ļ��Ŀ�ʼ�� */
  }

  return LengthBytes [BytesLeft];
}


/*********************************************************************
  LengthOut: ͨ������1���ֽڵ��������ݽ���������Ϣ�ĳ���;�����һ����
  					 �ں�ת�����������
  					 decodes the length of hidden data by interpreting 1 to 4
             successive bytes; switches to data output after last one
  parameter: ���볤�ȵ�һ�����~
  					 one byte of the encoded length
  returns:   nothing
*********************************************************************/

void LengthOut (UBYTE AddByte)
{
  static int ByteNo = 0;

  StegLength += AddByte;
  switch (++ByteNo)
  {
    case 1:
      if (StegLength >= 64)
      {
        StegLength <<= 8;
        StegLength -= 16320;
        return;
      }
      break;
    case 2:
      if (StegLength >= 41024)
      {
        StegLength <<= 8;
        StegLength -= 10461120;
        return;
      }
      break;
    case 3:
      if (StegLength >= 1089600)
      {
        StegLength <<= 8;
        StegLength -= 277848000;
        return;
      }
      break;
  }

  if (StegLength > (MediaLength - ByteNo) * MaxBitsAllowed / 8)
    ErrorExit (INVALIDLENGTH, "Error: File seems not to contain hidden data.\n");

  StegLeft = StegLength;
  InitBitDist (ByteNo);
  OutputByte = DataOut;   /* send following data to stdout */
}


/*********************************************************************
  SetBlock:  ������ͼ���ļ��е����ر�����Ϣ���Ϊǧ��֮ʮ(10 kB)
  					 set the secret bits in a block of the disk file in
             portions of 10 kB max;
  parameter: �����ݵ������������ݿ鳤��ָ���һ���ֽڵ��ļ�ָ��
  					 length of the continous block in data points, file
             pointer set to first byte
  returns:   nothing
*********************************************************************/

void SetBlock (UWORD32 count)
{
  void   *block;
  long   position;
  size_t subcount;

  subcount = 10240 / DataSize;
  while (count)
  {
    if (subcount > count)
      subcount = count;
    count -= subcount;

    if (block = malloc (subcount * DataSize))
    {
      if (verbose > 1)
        TurnWheel ();

      position = ftell (StegFile);
      fseek (StegFile, position, SEEK_SET);
      if (fread (block, DataSize, subcount, StegFile) != subcount)
        UnexpEOF ();

      (*ModData) (block, subcount);

      fseek (StegFile, position, SEEK_SET);
      fwrite (block, DataSize, subcount, StegFile);
      free (block);
    }
    else
      NoMemory ();
  }
}


/*********************************************************************
  GetBlock:  �����֮ʮ��ͼ���ļ��л�ȡ�����ŷ�
  					 get bits from a block of the disk file in 10 kB portions
  parameter: �������ݿ������,ָ���һ���ֽڵĶ�ָ�O
  					 count of the continous block, read pointer set to first
             byte
  returns:   nothing
*********************************************************************/

void GetBlock (UWORD32 count)
{
  size_t subcount;

  while (count)
  {
    subcount = 10240 / DataSize;
    if (subcount > count)
      subcount = count;
    count -= subcount;

    if (verbose > 1)
      TurnWheel ();

    while (subcount--)
      ExtractBits ();
  }
}
