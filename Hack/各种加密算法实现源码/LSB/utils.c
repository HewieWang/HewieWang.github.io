/*********************************************************************
  应用程序

  LSB算法中的部分必要函数
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lsb.h"
#include "utils.h"
#include "scramble.h"
#include "StdAfx.h"

/* globals */
int hiding = TRUE, verbose = 1;              /* 命令行选项 command line options */
FILE    *StegFile;                           /* 用来嵌入水印的文件，以下简称图像文乿data file to work on*/
FILE    *SecretFile;            /* 含有秘密数据信息瘿临时)文件，以下简称水印文乿(temporary) file with secret data*/
WORD32  StegLength = 0;             /* （欲)隐藏的数据信息的长度 length of hidden/to hide data*/
WORD32  StegLeft = 0;        /* 尚未处理完的隐藏数据信息的长帿part of StegLength not yet processed*/
WORD32  MediaLength = 0;        /* 可用于存储的数据信息长度 data points available for storage*/
size_t  DataSize;                            /* 每个数据点的比特擿bytes per data point*/
int     MaxBitsAllowed;                 /* 可用的比特／数据瀿bits/data point available*/
int     LastLength = FALSE;    /* 标志位： 处理过程中的最后长度位 flag: last length byte in progress*/
void    (*ModData) (void *, size_t);     /* 函数: 修改缓冲 funct ptr: modify buffer*/
unsigned int (*GetBits) (int);
                      /* 函数:从数据点上提取比特流 funct ptr: extracts bit(s) from data points*/
UBYTE   (*NextByte) (void);  /* 函数: 获取下一个要隐藏的比特数拿funct ptr: fetches next byte to hide*/
void    (*OutputByte) (UBYTE);  /* 函数: delivers restored byte */

/* 静态常量*/
        /*  初始化比特分配方法为1比特／数据点 initialize bit distribution algorithm to 1 bit/data point*/
static long IndicatorSum = 0, BitsNeeded = 1, BytesAvail = 1;


/*********************************************************************
  函数注释各行说明：第一行是函数名，第二行是函数运行所必需的参数，第三行是函数的返回值?
  *********************************************************************
  ErrorExit: 调用致命错误，显示错误信息并退兿
  						called on critical errors; prints error message and aborts
  parameter: 程序退出代码，指向显示信息数据的指针Programm Exit Code, Pointer to Message
  returns:   永无
*********************************************************************/

void ErrorExit (int ExitCode, char *ErrorMessage)
{
  if (verbose > 1)
    fputs ("\n", stderr);

  fputs (ErrorMessage, stderr);
  exit (ExitCode);
}


/*********************************************************************
  UnexpEOF:  当读出EOF时被访问，用来输出一个错误信息并退兿
  						is called when read past EOF, outputs an error message and aborts
  parameter: 敿
  returns:   永无
*********************************************************************/

void UnexpEOF (void)
{
  ErrorExit (UNEXPEOF,
   "Error: Unexpected end of file encountered - aborting.\n");
}


/*********************************************************************
  NoMemory:  申请内存失败时被访问
  						is called when malloc fails or SetBlock ends up with a zero byte block
  parameter:	 敿
  returns:   永无
*********************************************************************/

void NoMemory (void)
{
  ErrorExit (OUTOFMEMORY, "Error: Not enough memory - aborting.\n");
}


/*********************************************************************
  GetStegByte: 从图像文件中譿比特数据
  							reads 8 bit from media file
  parameter: 		 敿
  returns:     下一个字舿
*********************************************************************/

UBYTE GetStegByte (void)
{
  int b;

  if ((b = getc (StegFile)) == EOF)
    UnexpEOF ();

  return (UBYTE) b;
}


/*********************************************************************
  GetStegWord: 从图像文件中读一?6比特数据
  							reads one 16 bit word from media file
  parameter:			敿
  returns:     下两个字舿
*********************************************************************/

UWORD16 GetStegWord (void)
{
  UWORD16 value;

  if (fread (&value, 2, 1, StegFile) != 1)
    UnexpEOF ();

  return value;
}


/*********************************************************************
  GetStegLong: 从图像文件中读一?2比特数据
  							reads one 32 bit word from media file
  parameter:			敿
  returns:     下四个字舿
*********************************************************************/

UWORD32 GetStegLong (void)
{
  UWORD32 value;

  if (fread (&value, 4, 1, StegFile) != 1)
    UnexpEOF ();

  return value;
}


/*********************************************************************
  PutStegWord: 儿6比特数据到图像文件中
  							 writes one 16 bit word to media file
  parameter: 数据信息，指向文件描述符的指鐿
  						data word, Pointer to FILE descriptor
  returns:   写入数据的数酿
  						number of written bytes
*********************************************************************/

size_t PutStegWord (UWORD16 value)
{
  return fwrite (&value, 2, 1, StegFile);
}


/*********************************************************************
  NextIs:   将字符串与文件中的下一字节进行比较，通常读出整个长度
  					compare string to next bytes in file, allways read the whole length
  parameter: 指向需要比较字符串位置的指鐿
  						pointer to a string to be matched
  returns:   比较结果标志仿如果字符串相同为皿flag, TRUE if equal
           			  在字符比较完成后设置的文件指鐿	file pointer set behind the characters compared
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
  TurnWheel: 输出进展标志位中一个小分隔窿?
  					shows a little wheel as progress indicator
  parameter: none
  returns:   nothing
*********************************************************************/

void TurnWheel (void)
{
  static int    i = 3;
  static UBYTE  wheel[4] = {'-', '\\', '|', '/'};

  fputc (wheel[i = (i + 1) % 4], stderr);		//将wheel中对应的字符写入stderr?
  putc ('\b', stderr);
  fflush (stderr);													//输出stderr内容
}


/*********************************************************************
  BitsThisTime: 计算当前数据点的比特量，然后通过一种拉线算法（line draw?
  							将所有这些比特数据平均分配到图片中所有的可用数据中去。？?
						 calculates the number of bits in current data point;
             uses a 'line draw' algorithm to distribute all bits
             evenly among all available bytes
  parameter: none
  returns:   比特便	
  					bit number
*********************************************************************/

int BitsThisTime (void)
{
  ldiv_t btt;
  
  //用IndicatorSum + BitsNeeded除以BytesAvail，除得整商赋给btt.quot，余数赋给btt.rem
  btt = ldiv (IndicatorSum + BitsNeeded, BytesAvail);	

  IndicatorSum = btt.rem;

  return btt.quot;	//将除法整商返噿
}


/*********************************************************************
  InitBitDist: 初始化数据分配算法，以用来找到数据长度和可用的数据点?
				 initializes bit distribution algorithm to found data
             length and available data points
  parameter: 处理过程中的字节长度
  					number of length bytes processed
  returns:   nothing
*********************************************************************/

void InitBitDist (int LenByt)
{
  BitsNeeded = StegLength << 3;           // 右移3位数，即8 比特/1 仿
  BytesAvail = MediaLength - (LenByt << 3);	//？？
  IndicatorSum = BytesAvail >> 1;
}


/*********************************************************************
  NextBits:  从静态偏移量中读出所需的比特数；如果有必要，偏移量会由左到右填充下一字节？？
						 delivers the required number of bits from a static shift
             register; when necessary, shift register is filled from
             left to right with next scrambled byte
  parameter: 比特擿	
  					number of bits
  returns:   需要的整型比特
				 requested bits as integer
*********************************************************************/

unsigned int NextBits (int HowMany)
{
  static unsigned int ShiftReg = 0;	//静态偏移量
  static int TopBit = 0;
  unsigned int r;

  if (HowMany > TopBit)      /* 比特数不足，无法满足需褿*/
  {
    ShiftReg |= (unsigned int) Scramble ((*NextByte) ()) << TopBit;
    TopBit += 8;
  }

/*请在这里填入一段代码，它的作用是每次从ShiftReg里提取最低的
HowMany位数据，并将它赋值给r。赋值完成后将ShiftReg右移
HowMany位，TopBit减去HowMany。
*/

  return r;
}


/*********************************************************************
  ModDBytes: 将秘密信息插入到内存中一?比特的数据块?
				 injects the secret data bits into a block of 8 bit data
             in memory
  parameter: 指向UBYTE类型数组的指鐿 数组的大宿
  					pointer to UBYTE array, size of array
  returns:   敿
*********************************************************************/

void ModDBytes (UBYTE *block, size_t count)
{
  int n;

  while (count--)
  {
    if (n = BitsThisTime ())
	/*逢1的点才存在，否则不存在，继续查找，直到为1
	(~0u << n)的意思是根据要插入的位数n得到一个8位二进制数，
	如n=1则为11111110，将它和*block进行与操作，则其它位不变，
	欲插入的n位被置0，再与NextBits(n)做或操作，即可将欲嵌入的f
	数据写入block中
 */
      *block = *block & (~0u << n) | NextBits (n);
    block++;
  }
}


/*********************************************************************
  ModDWords: 将秘密信息插入到内存中一?6比特的数据块?
				 injects the secret data bits into a block of 16 bit data
             in memory
  parameter: 指向int类型数组的指鐿 数组的大宿
  returns:   敿
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
  ModLBytes: 将秘密信息插入到内存中一?比特的数据块中，在最后一字节插入完后转换到数据处理程帿
				 injects the length bits into a block of 8 bit data
             in memory, switches to data routine after last byte
  parameter: 指向UBYTE类型数组的指鐿 数组的大宿
  returns:   敿
*********************************************************************/

void ModLBytes (UBYTE *block, size_t count)
{
  static int BitsLeft = 0;

  while (count--)
  {
    *block = *block & ~1u | NextBits (1);
    block++;

    if (BitsLeft)                    /* 在处理过程中的最后七比特数据 */
    {
      if (--BitsLeft == 0)
      {
        ModData = (void (*) (void *, size_t)) ModDBytes;
        ModDBytes (block, count); /* 新程序中剩余的数据块 rest of block with new routin*/
        return;
      }
    }
    else if (LastLength)                  /* 最后一个字节中的第一个比燿*/
      BitsLeft = 7;
  }
}


/*********************************************************************
  ModLWords: 将秘密信息插入到内存中一?6比特的数据块中，在最后一字节插入完后转换到数据处理程帿
				 injects the length bits into a block of 16 bit data
             in memory, switches to data routine after last byte
  parameter: 指向int类型数组的指鐿 数组的大宿
  returns:   敿
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
  GetBitsFromByte: 从目标文件中获取下一个字节的LSB比特便
									get LSB bit(s) from next byte of the disk file
  parameter: 需要使用的最低有效位数量
  						number of Least Significant Bits to extract
  returns:   可以嵌入的integer类型的比特量
  						requested bits as integer
*********************************************************************/

/*请学生独立完成这一函数体代码，这一函数的作用就是从目标文件
中获取所需字节的LSB比特值，函数声明已经给出：
unsigned int GetBitsFromByte (int Bits)
其中Bits就是代表LSB算法中所要取的最低几位。取目标文件的一字
节数据可调用GetStegByte函数，学生要做的主要是把最低Bits位
从该字节取出来返回。
在函数里可以用到如下操作符：&与操作符；～取反；0u表示无符号的
0，同理1u表示无符号的1；<<表示左移，>>表示右移
*/
unsigned int GetBitsFromByte (int Bits)
{
	
}


/*********************************************************************
  GetBitsFromWord: 从目标文件中获取下一个词（word）的LSB比特便
  								get LSB bit(s) from next word of the disk file
  parameter: 需要使用的最低有效位数量
  returns:   可以嵌入的integer类型的比特量
*********************************************************************/

unsigned int GetBitsFromWord (int Bits)
{
  return (unsigned int) GetStegWord () & ~(~0u << Bits);
}


/*********************************************************************
  ExtractBits:  从之前修改的数据中提取出隐藏的比特值，然后重新分配
  							它们到当前偏移量中，当这些偏移量已存满时对长度或羿
  							水印文件计算就无法进行了
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
  GetSecretByte:	从中间文件中读取一个字舿
  							reads one byte from the temporary intermediate file
  parameter: none
  returns:   读取的字舿
  						read byte
*********************************************************************/

UBYTE GetSecretByte (void)
{
  if (! StegLeft--)
    ErrorExit (OUTOFSYNC, "Error: Reading past end of data to hide.\n");

  return getc (SecretFile);
}


/*********************************************************************
  DataOut:  向水印文件写入重建字舿
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


    /*EncodeLength/LengthOut处理文件长度一直到满足269525055,因为
    8-fold的比特总数是必要的而且适用于长整型便避免导致为零*/
    /* EncodeLength/LengthOut handle Lengths up to 269525055  */
    /* enough, as the 8-fold amount of bits is necessary and  */
    /* must fit into a 'long' value; avoids leading zeros     */


/*********************************************************************
  EncodeLength: 把欲隐藏数据的长帿Secretlength)编码为一?字节长的缓冲区数便?
  					 encodes the length of secret data (Secretlength) into
             a buffer of up to 4 bytes
  parameter: 	指向缓冲区的指针
  						pointer to buffer
  returns:  使用了的字节擿
  					number of bytes used
*********************************************************************/

int EncodeLength (UBYTE *LenghtBytes)
{
  UWORD32 Len;

  Len = StegLength;

  if (Len < 64)			//隐藏数据长度小于64
  {
    LenghtBytes[0] = (UBYTE) Len;
    return 1;
  }

  Len -= 64;
  LenghtBytes[0] = Len & 0xFF;
  Len >>= 8;

  if (Len < 160)		//隐藏数据长度小于224
  {
    LenghtBytes[1] = (UBYTE) Len + 64;
    return 2;
  }

  Len -= 160;
  LenghtBytes[1] = Len & 0xFF;
  Len >>= 8;

  if (Len < 16)			//隐藏数据长度小于240
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
  GetLengthByte: 返回在静态缓冲区里保存的经过经过编码的一个字节数拿
  首先,对长度进行编瞿最县变换为从中间文件中读出输僿
  					 returns one byte of the encoded length kept in a
             static buffer; on 1st invocation, encode length; on last
             invocation, switch input to read from intermediate file
  parameter: none
  returns:   一个长度字舿
  					 one length byte
*********************************************************************/

UBYTE GetLengthByte (void)
{
  static UBYTE LengthBytes [4];
  static int BytesLeft = 0;

  /* 首次调用时加载字节数繿 Load byte array on first call */
  if (BytesLeft == 0)
  {
    BytesLeft = EncodeLength (LengthBytes);
    InitBitDist (BytesLeft);      /* 准备比特分配 */
  }

  /* 当这上最后的长度字节when this is the last length byte */
  if (--BytesLeft == 0)
  {
    LastLength = TRUE;                       /* 标志信息:最后字舿*/
    fseek (SecretFile, 0L, SEEK_SET);
    NextByte = GetSecretByte;    /* 转到临时文件的开始处 */
  }

  return LengthBytes [BytesLeft];
}


/*********************************************************************
  LengthOut: 通过翻译1冿字节的连接数据解码隐藏信息的长度;在最后一个字
  					 节后转换到数据输兿
  					 decodes the length of hidden data by interpreting 1 to 4
             successive bytes; switches to data output after last one
  parameter: 编码长度的一个字舿
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
  SetBlock:  设置在图像文件中的隐藏比特信息最大为千分之十(10 kB)
  					 set the secret bits in a block of the disk file in
             portions of 10 kB max;
  parameter: 在数据点上连续的数据块长帿指向第一个字节的文件指针
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
  GetBlock:  从午分之十的图像文件中获取比特信忿
  					 get bits from a block of the disk file in 10 kB portions
  parameter: 连续数据块的数量,指向第一个字节的读指鐿
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
