/*********************************************************************
  Bitmap程序
  支持 OS/2 1.x, Windows 3.x and OS/2 2.x 单张BMP图片;通过256色图片，这种算法的基本思想来源于"Spyder"项目
  为了便于理解，在本程序中对应于8位位图（256色BMP图片）的处理代码被注销掉，如果想要具体了解这一部分处理过
  程，可以去掉注销，稍作修改即可运行。
  last modified by lirui
*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lsb.h"
#include "utils.h"
#include "scramble.h"
#include "StdAfx.h"

#define MINDIST 3.0
#define MIDDIST 20.0
#define MAXDIST 1000.0

#define UseCount(x) (ColorData[x].HowOften[0] + ColorData[x].HowOften[1])
#define WeightedVote(x) ((ColorData[x].HowOften[1] - ColorData[x].HowOften[0]) \
                         / sqrt (UseCount (x)))
#define Criterion(x) (ColorData[x].Distance * sqrt (UseCount (x)))

typedef union
{
  UWORD16 RGB[3];
  struct
  {
    UWORD16 L;
    WORD16  a;
    WORD16  b;
  } Lab;
} PalEntry;

typedef struct
{
  UBYTE   Partner;
  UBYTE   Assigned;   /* 也是一个多用途标志位 */
} PalRelated;

typedef struct
{
  WORD32  Total;         /* 这种颜色的像素 */
  WORD32  HowOften[2];     /* 0: 0-bits, 1: 1-bits */
  float   Distance;
  float   LWeightedSum;
  float   aWeightedSum;
  float   bWeightedSum;
  float   Deposit;
} PalStatist;

 /* 附加标志位，fillup用来标志使用调色板条目情况， */
int fillup = 1, smooth = TRUE;    

static long LineOffset, position, BMPInfoSize;
static UWORD32 height, width, (**NextLineM) (void);
static UWORD16 ColorCount = 0;
static PalRelated *ColorAttr;
static PalEntry *Color;
static PalStatist *ColorData;


/*********************************************************************
  RGB2Lab:    按照Microsoft/ Hewlett的三原色规范（sRGB specification），
  						将对应的三原色的调色板条目转换为CIE L*a*b* 色彩空间
  						converts palette entries in monitor RGB coordinates to
              the CIE L*a*b* colorspace using the Microsoft/ Hewlett
              Packard sRGB specification
  parameter:  指向调色板条目的指针
  returns:    nothing
*********************************************************************/
/*
void RGB2Lab (PalEntry *col)
{
  static float convD65[3][3] =
    { {0.4124, 0.3576, 0.1805},
      {0.2126, 0.7152, 0.0722},
      {0.0193, 0.1192, 0.9505} };
  float   cRGB[3], XYZ[3];
  int     i,j;

  // linearize with approximate gamma of 2.2 
  for (j = 3; j--; )
    cRGB[j] = col->RGB[j] > 10
              ? pow ((col->RGB[j] + 14.025) / 269.025, 2.4)
              : col->RGB[j] / 3294.6;

  // 将正确的RGB色彩空间转换为对应的CIE XYZ 
  for (i = 3; i--; )
  {
    XYZ[i] = 0.0;
    for (j = 3; j--; )
      XYZ[i] += convD65[i][j] * cRGB[j];

    // and further to f(X/Xn) resp. Y and Z 
    XYZ[i] = XYZ[i] > 0.008856 ? pow (XYZ[i], 1.0 / 3.0)
                               : XYZ[i] * 7.787 + 16.0 / 116.0;
  }

  // 再转换为相应的CIE  L*a*b*色彩空间，并转换为适当的编码: 0 <= L <= 100 in 0x0000 - 0xFFFF
  //                  -128.000 <= a,b <= 127.996 in 0x8000 - 0x7FFF  
  col->Lab.L = XYZ[1] * 76020.6 - 10485.1;
                          // (116 * f(Y) - 16) / 100 * 65535 + 0.5 
  col->Lab.a = (XYZ[0] - XYZ[1]) * 128000.0 + 0.5;
                                // 500 * (f(X) - f(Y)) * 256 + 0.5 
  col->Lab.b = (XYZ[1] - XYZ[2]) * 51200.0 + 0.5;
                                // 200 * (f(Y) - f(Z)) * 256 + 0.5 
}
*/

/*********************************************************************
  Lab2RGB:    将调色板条目从CIE L*a*b*转换回三原色色彩空间 
  parameter:  指向调色板条目的指针
  returns:    nothing
*********************************************************************/
/*
void Lab2RGB (PalEntry *col)
{
  static float convD65[3][3] =
    { { 3.2410, -1.5374, -0.4986},
      {-0.9692,  1.8760,  0.0416},
      { 0.0556, -0.2040,  1.0570} };
  float   cRGB[3], XYZ[3];
  int     i,j;

  // calculate f(?/?n) for XYZ from L*a*b* 
  XYZ[1] = (col->Lab.L + 10485.6) / 76020.6;
  XYZ[0] = col->Lab.a / 128000.0 + XYZ[1];
  XYZ[2] = XYZ[1] - col->Lab.b / 51200.0;

  // invert f(?/?n) function 
  for (i = 3; i--; )
    XYZ[i] = XYZ[i] > 0.206893 ? XYZ[i] * XYZ[i] * XYZ[i]
                               : (XYZ[i] - 16.0 / 116.0) / 7.787;

  // convert CIE XYZ to RGB 
  for (j = 3; j--; )
  {
    cRGB[j] = 0.0;
    for (i = 3; i--; )
      cRGB[j] += convD65[j][i] * XYZ[i];

    // precorrect for an approximate gamma of 2.2 
    col->RGB[j] = cRGB[j] > 0.00304
                ? pow (cRGB[j], 1.0 / 2.4) * 269.025 - 13.525
                : cRGB[j] * 3294.6 + 0.5;
  }
}
*/

/*********************************************************************
  mDeltaE:    calculates the 'distance' (delta E) of two palette
              entries in CIE L*a*b* coordinates with much more weight
              on luminance (* 6.2), multiplied by a factor derived from
              the difference in bit frequencies (differences around
              the squareroot of the sums result in a factor of 1.1,
              maximum factor 2.0)
  parameter:  two palette entries
  returns:    result
*********************************************************************/
/*
float mDeltaE (int x, int y)
{
  float   t, d, f;

  t = ((float) Color[y].Lab.L - Color[x].Lab.L) / (655.35 / 6.2);
  d = t * t;
  t = ((float) Color[y].Lab.a - Color[x].Lab.a) / 256.0;
  d += t * t;
  t = ((float) Color[y].Lab.b - Color[x].Lab.b) / 256.0;
  d += t * t;

  d = sqrt (d);
  f = (float) UseCount (x) * UseCount (y);
  if (f > 0)
  {
    t = f > 1.0 ? log (49.0) / log (f) : 1.0;
    f = (float) (ColorData[x].HowOften[0] - ColorData[x].HowOften[1]) *
                (ColorData[y].HowOften[0] - ColorData[y].HowOften[1]) /
                f;
    f = (f > 0 ? pow (f, t) : - pow (-f, t)) / 3.0;
    if (1.0 + f > (1.0 - f) * MAXDIST)
      d = MAXDIST;
    else
      d *= (1.0 + f) / (1.0 - f);
  }
  return d;
}
*/

/*********************************************************************
  Mod2Pixel:  在内存中将欲隐藏数据比特插入到一个8比特的数据块中
  parameter:  指向UBYTE数组的指针,数组大小
  returns:    nothing
*********************************************************************/
/*
void Mod2Pixel (UBYTE *block, size_t count)
{
  while (count--)
  {
    if (BitsThisTime () &&
        NextBits (1) != ColorAttr[*block].Assigned)
      *block = ColorAttr[*block].Partner;
    block++;
  }
}
*/

/*********************************************************************
  ModPixel:   在内存中将欲隐藏数据比特插入到一个8比特的数据块中，
  						在最后一个字节转移到数据程序
  						injects the length bits into a block of 8 bit data
              in memory, switches to data routine after last byte
  parameter:  指向UBYTE数组的指针,数组大小
  returns:    nothing
*********************************************************************/
/*
void ModPixel (UBYTE *block, size_t count)
{
  static int BitsLeft = 0;

  while (count--)
  {
    if (NextBits (1) != ColorAttr[*block].Assigned)
      *block = ColorAttr[*block].Partner;
    block++;

    if (BitsLeft)                    // 在处理时最后7位?
    {
      if (--BitsLeft == 0)
      {
        ModData = (void (*) (void *, size_t)) Mod2Pixel;
        Mod2Pixel (block, count); // 用新程序修理其它数据块 
        return;
      }
    }
    else if (LastLength)                  // 最后一个字节的第一位 
      BitsLeft = 7;
  }
}
*/

/*********************************************************************
  UpdateStat: 用像素内容储存偏移量数据，更新颜色值和频率
  						registers bit to be stored with pixel contents, updates
              color count and frequency
  parameter:  像素值
  returns:    nothing
*********************************************************************/
/*
void UpdateStat (UBYTE ColorIndex)
{
  static int BitsLeft = 8;

  if (ColorIndex >= ColorCount)
    ColorCount = ColorIndex + 1;

  ColorData[ColorIndex].Total++;

  if (BitsLeft || BitsThisTime ())
    ColorData[ColorIndex].HowOften[NextBits (1)]++;

  if (BitsLeft && LastLength)
    BitsLeft--;
}
*/

/*********************************************************************
  GetBitFromPixel: 获取指定给调色板条目的图像文件下一个像素的比特值，或者是0
  						get the bit assigned to the palette entry of the
              next pixel of the disk file, or 0
  parameter:  标志位（如果比特值可以被返回）
  						Flag if bit should be returned
  returns:    请求的整型比特值
  						requested bit as integer
*********************************************************************/
/*
unsigned int GetBitFromPixel (int Flag)
{
  UBYTE b;

  b = GetStegByte ();

  return Flag ? ColorAttr[b].Assigned : 0;
}
*/

/*********************************************************************
  FindNearest: 在所有颜色中找到编码一个比特值最接近的颜色
  							Find the closest Color for all colors that encode a bit
  parameter:  none
  returns:    nothing
*********************************************************************/
/*
void FindNearest (void)
{
  int     i, j;
  float   ActDist;

  for (i = 0; i < ColorCount; i++)
    ColorData[i].Distance = MAXDIST + 1.0;

  for (i = 0; i < ColorCount - 1; i++)
  {
    // 只有在图片中使用的图片才可以成为配对颜色 only colors used in the picture may be partners 
    if (ColorData[i].Total)
    {
      for (j = i + 1; j < ColorCount; j++)
      {
        if (ColorData[j].Total)
        {
          ActDist = mDeltaE (i, j);
          // 只有用于水印操作的颜色需要配对 only colors used for steganography need partners 
          if (UseCount (i) && ActDist < ColorData[i].Distance)
          {
            ColorAttr[i].Partner = j;
            ColorData[i].Distance = ActDist;
          }
          if (UseCount (j) && ActDist < ColorData[j].Distance)
          {
            ColorAttr[j].Partner = i;
            ColorData[j].Distance = ActDist;
          }
        }
      }
    }
  }
}
*/

/*********************************************************************
  FillGaps:    分配未使用的调色板条目作为那些距离配对差异最大的比特的配对值
  						Distribute unused palette entries as partners for those
              with distant partners on a longest distance first base
  parameter:  none
  returns:    nothing
*********************************************************************/
/*
void FillGaps (void)
{
  int     i, j, top = 0;
  UBYTE   Least[256], m;
  float   Crit;

  // 找到没有配对的颜色 
  for (i = ColorCount; i--; )  // 有最高元素的第一个颜色first colors to highest elements 
    if (ColorData[i].Distance > (fillup > 1 ? MINDIST : MIDDIST))
      Least[top++] = i;

  // (插入) 按照距离和数据对它们进行分类  
  //(Insertion)sort them on distance and count 
  for (i = 1; i < top; i++)
  {
    m = Least [j = i];
    Crit = Criterion (m);
    while (j && Criterion (Least[j - 1]) > Crit)
    {
      Least[j] = Least[j - 1];
      j--;
    }
    Least[j] = m;
  }

  // 分配未使用的调色板给差异最大的分配者
  //distribute unused palette entries to colors with farthest partner
  for (i = 0; top && i < 256; i++)
  {
    if (! ColorData[i].Total)
    {
      ColorAttr[Least[--top]].Partner = i;
      Color[i] = Color[Least[top]];
      if (++i > ColorCount)
        ColorCount = i;
    }
  }
}
*/

/*********************************************************************
  AssignBits: 找出对于所有颜色来说最好的比特值来进行配对；让所有颜色
  						找到它们最适合比特值；每遇到一个1，就增加对于这个颜色的
  						投票数，0则减少；一个颜色的选票从它的配对比特值中减去；
  						选票做标准偏差的校正。
  						Find out best bit values for all colors with partners;
              let all colors vote for their preferred bit; every 1 to
              be stored increases, every 0 decreases the vote for this
              color; the vote of one color subtracts from the vote of
              its partner; votes are corrected for standard deviation
  parameter:  none
  returns:    nothing
*********************************************************************/
/*
void AssignBits (void)
{
  int     i, p, q;
  float   vote;

  for (i = 0; i < ColorCount; i++)
  {
    // 如果选票已经被收集起来，或者颜色没有配对值，就继续
    //if vote already collected or color without partner: advance 
    if (ColorAttr[i].Assigned > 1 || ! UseCount (i))
      continue;

    // 投票收集每一步的选票，改变每一步的标号
    //vote and collect votes on the way, change sign every step 
    q = i;
    vote = 0.0;
    do
    {
      vote = WeightedVote (q) - vote;
      ColorAttr[q].Assigned = 2;           // 标志: 已经投票 
      q = ColorAttr[p = q].Partner;          // p: keep previous 
    }
    while (ColorAttr[q].Assigned < 2 && UseCount (q));

    if (UseCount (q) && ColorAttr[q].Partner != p)
    {
      while (ColorAttr[q].Assigned != 3)
      {
        vote = - vote;
        q = ColorAttr[q].Partner;
      }
    }

    //记录选票和标志条目以作为投票箱
    //register votes and mark entry as ballot box 
    ColorData[q].Deposit -= vote;
    ColorAttr[q].Assigned = 3;
  }

  // 分配比特值反对成为颜色的配对
  //assign bits to colors opposite to partner's 

  for (i = 0; i < ColorCount; i++)
  {
    // 还没被分配的的颜色  
    if (ColorAttr[i].Assigned > 1)
    {
      p = 0;
      q = i;
      while (ColorAttr[q].Assigned == 2) // 被选中，但还没分配的 
      {
        p ^= 1u;
        q = ColorAttr[q].Partner;
      }

      if (ColorAttr[q].Assigned == 3)   // 投票箱没有估值的 ballot box not evaluated 
      {
        ColorAttr[q].Assigned = (ColorData[q].Deposit > 0.0);
        ColorData[q].Deposit = 0;
      }

      ColorAttr[i].Assigned = ColorAttr[q].Assigned ^ p;
    }
  }
}
*/

/*********************************************************************
  AdjustColors: 以分配改变最少为依据确定颜色的最佳匹配，在RED[0]存储分配比特值
  						Determine colors best suited for bit distribution with
              least overall change, store assigned bit in RED[0]
  parameter:  none
  returns:    nothing
*********************************************************************/
/*
void AdjustColors (void)
{
  int     i, p;
  float   Weight;

  if (smooth)
  {
    for (i = 0; i < ColorCount; i++)
    {
      if (UseCount (i))
      {
        p = ColorAttr[i].Partner;
        Weight = ColorData[i].HowOften[! ColorAttr[i].Assigned];
        ColorData[p].Deposit += Weight;
        ColorData[p].LWeightedSum += Color[i].Lab.L * Weight;
        ColorData[p].aWeightedSum += Color[i].Lab.a * Weight;
        ColorData[p].bWeightedSum += Color[i].Lab.b * Weight;
      }
    }
  }

  for (i = 0; i < 256; i++)
  {
    if (smooth && ColorData[i].Deposit > 0)
    {
      Weight = ColorData[i].Total - ColorData[i].HowOften[! ColorAttr[i].Assigned];
      ColorData[i].Deposit += Weight;
      ColorData[i].LWeightedSum += Color[i].Lab.L * Weight;
      ColorData[i].aWeightedSum += Color[i].Lab.a * Weight;
      ColorData[i].bWeightedSum += Color[i].Lab.b * Weight;
      Color[i].Lab.L = ColorData[i].LWeightedSum / ColorData[i].Deposit + 0.5;
      Color[i].Lab.a = ColorData[i].aWeightedSum / ColorData[i].Deposit + 0.5;
      Color[i].Lab.b = ColorData[i].bWeightedSum / ColorData[i].Deposit + 0.5;
    }

    Lab2RGB (&Color[i]);

    if (UseCount (i) || ColorData[i].Deposit > 0)
    {
      Color[i].RGB[0] &= ~1u;   // 代码在红色的最低有效位上分配比特 
      Color[i].RGB[0] |= ColorAttr[i].Assigned;
    }
  }
}
*/

/*********************************************************************
  IsGreyScale:	确定调色板是否真的是灰色范围(greyscale)，比如对于所有的i ，
  							Palette[i].Red = .Green = .Blue = i
  						determines if palette is true greyscale, i.e.
              Palette[i].Red = .Green = .Blue = i for all i
  parameter:  无; 使用BMPInfoSize来确定调色板偏移量
  returns:    结果限于UWORD16类型
*********************************************************************/
/*
int IsGreyScale (void)
{
  int i, j;

  // go to palette 
  if (fseek (StegFile, 14 + BMPInfoSize, SEEK_SET))
    UnexpEOF ();

  for (i = 0; i < 256; i++)
  {
    for (j = 3; j--; )
      if (GetStegByte () != i)
        return FALSE;

    if (BMPInfoSize != 12)  // if RGBQUAD: dummy read 
      GetStegByte ();
  }
  return TRUE;
}
*/




/*********************************************************************
  FirstLine:  在256色BMP图中在第一个nextblock调用时被访问；所有的初始
  						化、调色板和像互不分析和最佳化都在这里进行；链接指向
							所有较远的调用到BMPNextBlock
  						is called on the 1st nextblock call with 256 colore BMPs;
              all initialization, palette and pixel analysis and
              optimization is done here; chains then to and directs
              all further calls to BMPNextBlock;
  parameter:  none; uses external variables:
              标志位： hiding, fillup, smooth
              BMPInfoSize, position, width, height, LineOffset
              MediaLength, StegLength
  returns:    BMPNextBlock返回的代码
              改变的外部变量
              ColorAttr[256]：存有比特值和其配对颜色的数组
*********************************************************************/
/*
UWORD32 FirstLine (void)
{
  int   i, j;
  long  ls, lc;

  if ((ColorAttr = calloc (256, sizeof (PalRelated))) == NULL)
    NoMemory ();

  // 转到调色板 
  if (fseek (StegFile, 14 + BMPInfoSize, SEEK_SET))
    UnexpEOF ();

  if (hiding)
  {
    if (verbose > 1)
    {
      fputs ("\n分析调色板 ... ", stderr);
      fflush (stderr);
    }

    if ((Color = calloc (256, sizeof (PalEntry))) == NULL)
      NoMemory ();

    for (i = 0; i < 256; i++)
    {
      // 读入所有的调色板条目并转换为Lab 
      for (j = 3; j--; )
        Color[i].RGB[j] = GetStegByte ();

      RGB2Lab (&Color[i]);

      if (BMPInfoSize != 12)  //if RGBQUAD: dummy read 
        GetStegByte ();
    }

    if (verbose > 1)
    {
      fputs ("Done.\n分析图片 ... ", stderr);
      fflush (stderr);
    }

    if ((ColorData = calloc (256, sizeof (PalStatist))) == NULL)
      NoMemory ();

    //检查所有的像素，看看哪个比特值可以被存储
    //go through all pixels and see which bits are to be stored 
    ls = position;
    for (i = height; i--; )
    {
      if (verbose > 1)
        TurnWheel ();
      if (fseek (StegFile, ls, SEEK_SET))
        UnexpEOF ();
      for (lc = width; lc--; )
        UpdateStat (GetStegByte ());
      ls += LineOffset;
    }
    // 重设嵌入运算的开始参数 
    NextByte = GetLengthByte;
    LastLength = FALSE;
    StegLeft = StegLength;
    ResetScrambler ();

    if (verbose > 1)
    {
      fputs ("完成.\n查找最佳编码配置 ... ", stderr);
      fflush (stderr);
    }

    // 这里是处理主要任务 
    FindNearest ();
    if (fillup)
      FillGaps ();
    AssignBits ();

    if (verbose > 1)
    {
      fputs ("完成.\n 优化调色板 ... ", stderr);
      fflush (stderr);
    }

    AdjustColors ();

    if (fillup && BMPInfoSize != 12)
    {
      // 在BMP图片文件信息头中设置ClrUsed/ClrImportant-Fields
      fseek (StegFile, 46, SEEK_SET);
      PutStegWord (ColorCount);
      PutStegWord (0);
      PutStegWord (ColorCount);
      PutStegWord (0);
    }

    // 写改变了的调色板
    fseek (StegFile, 14 + BMPInfoSize, SEEK_SET);
    for (i = 0; i < 256; i++)
    {
      // 写所有的调色板条目
      for (j = 3; j--; )
        fputc (Color[i].RGB[j], StegFile);

      if (BMPInfoSize != 12)  // 如果RGBQUAD: 写0 
        fputc (0, StegFile);
    }

    free (ColorData);
    free (Color);

    if (verbose > 1)
    {
      fputs ("完成.\n为数据编码 ... ", stderr);
      fflush (stderr);
    }
  }
  else
  {
    if (verbose > 1)
    {
      fputs ("\n获取编码配置 ... ", stderr);
      fflush (stderr);
    }

    // 确定从Red LSB指定的位 
    for (i = 0; i < 256; i++)
    {
      GetStegByte ();
      GetStegByte ();
      ColorAttr[i].Assigned = GetStegByte () & 1u;
      if (BMPInfoSize != 12)  // 如果RGBQUAD: dummy read 
        GetStegByte ();
    }

    if (verbose > 1)
    {
      fputs ("完成.\n为数据解码 ... ", stderr);
      fflush (stderr);
    }
  }

  *NextLineM = BMPNextBlock;
  return BMPNextBlock ();
}
*/

/*********************************************************************
  BMPNextBlock: 指向图像文件下一个扫描行(scanline)开始处的读指针位置
  parameter:  无；使用外部变量
  						位置：扫描行相对于文件开始处的偏移量(offset)
  						宽度：扫描行的宽度（字节）
  						高度：扫描行的数量
  						行偏移：到下一个扫描行的偏移量
  returns:    扫描行长度（字节），如果是EOF就返回0
  						改变了的外部变量
  						位置：指向下一个扫描行
  						高度：剩余扫描行数量
*********************************************************************/

UWORD32 BMPNextBlock (void)
{
  if (height--)
  {
    if (fseek (StegFile, position, SEEK_SET))
      UnexpEOF ();
    position += LineOffset;
    return width;
  }
  else
    return 0;
}

/*********************************************************************
  CheckIfBMP: 检查和输出图像文件的特征；如果位图格式不满足水印算法，就中断；
              # of planes != 1, # of bits not 8 or 24, 位图被压缩；为
              BMPNextBlock设置外部变量
              位置：扫描行相对于文件开始处的偏移量
  						宽度：扫描行的宽度（字节）
  						高度：扫描行的数量
  						行偏移：到下一个扫描行的偏移量
							在嵌入时对于8bit调色板操作如下：
              1. with newer format and fewer than maximum # of colors
                 used or important these are reset to maximum for ste-
                 ganography almost certainly changes this number
              2. the palette is optimized it switches are set
  parameter:  指向'NextBlock'程序的指针的位置
  						文件中指向第二个字节的读指针
  returns:    如果被认可为位图格式即为TRUE
*********************************************************************/

int CheckIfBMP (UWORD32 (**NextBlockP) (void))
{
  int BitCount, planes;

  switch (GetStegByte ())
  {
    case 'M':
      break;
    case 'A':
      if (verbose > 1)
        fputs ("Hint: File is probably an OS/2 bitmap ARRAY. "
               "Convert to normal bitmap!\n", stderr);
    default:
      return FALSE;
  }

  if (fseek (StegFile, 10, SEEK_SET))
    return FALSE;
  position = GetStegLong ();    /* 相对第一个扫描行的偏移量 */

  BMPInfoSize = GetStegLong ();
  switch (BMPInfoSize)//通过头文件大小可以知道位图格式，如果是40，是Windows下的位图文件
  {
    case 12:
    case 40:
    case 64:
      break;
    default:
      return FALSE;
  }

  width = BMPInfoSize == 12 ? GetStegWord () : GetStegLong ();
  height = BMPInfoSize == 12 ? GetStegWord () : GetStegLong ();
  planes = GetStegWord ();
  BitCount = GetStegWord ();

  if (verbose)
  {
    fputs ("位图格式 <", stderr);
    switch (BMPInfoSize)
    {
      case 12:
        fputs ("OS/2 1", stderr);
        break;
      case 40:
        fputs ("Windows 3", stderr);
        break;
      case 64:
        fputs ("OS/2 2", stderr);
        break;
    }
    fputs (".x> detected, ", stderr);
    switch (BitCount)
    { case 1:
        fputs ("monochrome (2", stderr);
        break;
      case 4:
        fputs ("VGA (16", stderr);
        break;
      case 8:
        fputs ("SVGA or greyscale (256", stderr);
        break;
      case 16:
        fputs ("High Color (65536", stderr);
        break;
      case 24:
        fputs ("24位真彩色 (16.7 Mio", stderr);
        break;
      default:
        fprintf (stderr, "未知的颜色位数 (? = 2 ^ %i", BitCount);
    }
    fputs (" colors).\n", stderr);

    if (verbose > 1)
      fprintf (stderr, "位图大小是 %li x %li 像素.\n", width, height);
  }

  if (BMPInfoSize > 12)
  {
    if (GetStegLong ())
      ErrorExit (BMPCOMPRESS,
                 "错误(BMP): 位图是一个(RLE-)压缩 - 不被支持.\n");

    if (verbose > 1)
    {
	/*请在这里填入一段代码，首先取出图像头文件中下四个字节数据，这四个字节里
	的数据可以标识整个图像文件的实际字节大小，也可以为0。要求如果不为0时，就
	输出如下形式文字：位图的实际大小是：XX字节。
	其中取出四字节数据使用GetStegLong函数，它 在steg.c文件里被定义，如果不理解，可以参见相关代码；在终端输出信息使用fprintf函数。
	它们的具体用法可以参考上、下文的相关代码。
		*/  
		
		long pam1=GetStegLong();
		long pam2=GetStegLong();		
		
		if(pam1!=0 && pam2!=0)
		{
			fprintf (stderr, "位图分辨率：水平方向每英尺 %li点 ",
				(pam1 * 127 + 2500) / 5000);
			fprintf (stderr, "，垂直方向每英尺 %li点。\n",
				(pam2 * 127 + 2500) / 5000);
		}
    }
  }

  if (height == 0 || width == 0)
    ErrorExit (BMPNOPIXELS, "错误(BMP): 没有图片 ?!?\n");

  if (planes != 1)
    ErrorExit (BMPNOT1PLANE,
               "错误(BMP): 像素被分成超过一个平面.\n");

  switch (BitCount)
  {
    case 24:
      width *= 3;
    case 8:
      break;
    default:
      ErrorExit (BMPCOLORCNT,
        "错误(BMP): 颜色尝试无法满足水印要求.\n");
  }

//  if (BitCount == 24 || IsGreyScale ())
  if (BitCount == 24)
  {
    MaxBitsAllowed = 2;
    if (hiding)
      ModData = (void (*) (void *, size_t)) ModLBytes;
    else
      GetBits = GetBitsFromByte;
    *NextBlockP = BMPNextBlock;
  }
  else
  {
    ErrorExit (BMPCOLORCNT,
		  "Error (BMP): Color depth not suited for steganography.\n");
    /*
    MaxBitsAllowed = 1;
    if (hiding)
      ModData = (void (*) (void *, size_t)) ModPixel;
    else
      GetBits = GetBitFromPixel;
    *NextBlockP = FirstLine;
    NextLineM = NextBlockP;         // 保持指针位置 
		*/  
  }
  /*下面是三个常量的赋值过程。这三个常量分别是：
  用户选择的在每字节嵌入的水印数据位数（DataSize），一般常设为1；
  图像可用的字节数据大小（MediaLength），它等于字节宽度（width）
  乘以像素高度；行偏移量，它被设为字节宽度。
  */
  DataSize = 1;
  MediaLength = width * height;
  LineOffset = width + 3 & ~3ul;   // 调整扫描行为WORD32边界 

  return TRUE;
}
