/*********************************************************************
  Bitmap����
  ֧�� OS/2 1.x, Windows 3.x and OS/2 2.x ����BMPͼƬ;ͨ��256ɫͼƬ�������㷨�Ļ���˼����Դ��"Spyder"��Ŀ
  Ϊ�˱�����⣬�ڱ������ж�Ӧ��8λλͼ��256ɫBMPͼƬ���Ĵ�����뱻ע�����������Ҫ�����˽���һ���ִ����
  �̣�����ȥ��ע���������޸ļ������С�
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
  UBYTE   Assigned;   /* Ҳ��һ������;��־λ */
} PalRelated;

typedef struct
{
  WORD32  Total;         /* ������ɫ������ */
  WORD32  HowOften[2];     /* 0: 0-bits, 1: 1-bits */
  float   Distance;
  float   LWeightedSum;
  float   aWeightedSum;
  float   bWeightedSum;
  float   Deposit;
} PalStatist;

 /* ���ӱ�־λ��fillup������־ʹ�õ�ɫ����Ŀ����� */
int fillup = 1, smooth = TRUE;    

static long LineOffset, position, BMPInfoSize;
static UWORD32 height, width, (**NextLineM) (void);
static UWORD16 ColorCount = 0;
static PalRelated *ColorAttr;
static PalEntry *Color;
static PalStatist *ColorData;


/*********************************************************************
  RGB2Lab:    ����Microsoft/ Hewlett����ԭɫ�淶��sRGB specification����
  						����Ӧ����ԭɫ�ĵ�ɫ����Ŀת��ΪCIE L*a*b* ɫ�ʿռ�
  						converts palette entries in monitor RGB coordinates to
              the CIE L*a*b* colorspace using the Microsoft/ Hewlett
              Packard sRGB specification
  parameter:  ָ���ɫ����Ŀ��ָ��
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

  // ����ȷ��RGBɫ�ʿռ�ת��Ϊ��Ӧ��CIE XYZ 
  for (i = 3; i--; )
  {
    XYZ[i] = 0.0;
    for (j = 3; j--; )
      XYZ[i] += convD65[i][j] * cRGB[j];

    // and further to f(X/Xn) resp. Y and Z 
    XYZ[i] = XYZ[i] > 0.008856 ? pow (XYZ[i], 1.0 / 3.0)
                               : XYZ[i] * 7.787 + 16.0 / 116.0;
  }

  // ��ת��Ϊ��Ӧ��CIE  L*a*b*ɫ�ʿռ䣬��ת��Ϊ�ʵ��ı���: 0 <= L <= 100 in 0x0000 - 0xFFFF
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
  Lab2RGB:    ����ɫ����Ŀ��CIE L*a*b*ת������ԭɫɫ�ʿռ� 
  parameter:  ָ���ɫ����Ŀ��ָ��
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
  Mod2Pixel:  ���ڴ��н����������ݱ��ز��뵽һ��8���ص����ݿ���
  parameter:  ָ��UBYTE�����ָ��,�����С
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
  ModPixel:   ���ڴ��н����������ݱ��ز��뵽һ��8���ص����ݿ��У�
  						�����һ���ֽ�ת�Ƶ����ݳ���
  						injects the length bits into a block of 8 bit data
              in memory, switches to data routine after last byte
  parameter:  ָ��UBYTE�����ָ��,�����С
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

    if (BitsLeft)                    // �ڴ���ʱ���7λ?
    {
      if (--BitsLeft == 0)
      {
        ModData = (void (*) (void *, size_t)) Mod2Pixel;
        Mod2Pixel (block, count); // ���³��������������ݿ� 
        return;
      }
    }
    else if (LastLength)                  // ���һ���ֽڵĵ�һλ 
      BitsLeft = 7;
  }
}
*/

/*********************************************************************
  UpdateStat: ���������ݴ���ƫ�������ݣ�������ɫֵ��Ƶ��
  						registers bit to be stored with pixel contents, updates
              color count and frequency
  parameter:  ����ֵ
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
  GetBitFromPixel: ��ȡָ������ɫ����Ŀ��ͼ���ļ���һ�����صı���ֵ��������0
  						get the bit assigned to the palette entry of the
              next pixel of the disk file, or 0
  parameter:  ��־λ���������ֵ���Ա����أ�
  						Flag if bit should be returned
  returns:    ��������ͱ���ֵ
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
  FindNearest: ��������ɫ���ҵ�����һ������ֵ��ӽ�����ɫ
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
    // ֻ����ͼƬ��ʹ�õ�ͼƬ�ſ��Գ�Ϊ�����ɫ only colors used in the picture may be partners 
    if (ColorData[i].Total)
    {
      for (j = i + 1; j < ColorCount; j++)
      {
        if (ColorData[j].Total)
        {
          ActDist = mDeltaE (i, j);
          // ֻ������ˮӡ��������ɫ��Ҫ��� only colors used for steganography need partners 
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
  FillGaps:    ����δʹ�õĵ�ɫ����Ŀ��Ϊ��Щ������Բ������ı��ص����ֵ
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

  // �ҵ�û����Ե���ɫ 
  for (i = ColorCount; i--; )  // �����Ԫ�صĵ�һ����ɫfirst colors to highest elements 
    if (ColorData[i].Distance > (fillup > 1 ? MINDIST : MIDDIST))
      Least[top++] = i;

  // (����) ���վ�������ݶ����ǽ��з���  
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

  // ����δʹ�õĵ�ɫ����������ķ�����
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
  AssignBits: �ҳ�����������ɫ��˵��õı���ֵ��������ԣ���������ɫ
  						�ҵ��������ʺϱ���ֵ��ÿ����һ��1�������Ӷ��������ɫ��
  						ͶƱ����0����٣�һ����ɫ��ѡƱ��������Ա���ֵ�м�ȥ��
  						ѡƱ����׼ƫ���У����
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
    // ���ѡƱ�Ѿ����ռ�������������ɫû�����ֵ���ͼ���
    //if vote already collected or color without partner: advance 
    if (ColorAttr[i].Assigned > 1 || ! UseCount (i))
      continue;

    // ͶƱ�ռ�ÿһ����ѡƱ���ı�ÿһ���ı��
    //vote and collect votes on the way, change sign every step 
    q = i;
    vote = 0.0;
    do
    {
      vote = WeightedVote (q) - vote;
      ColorAttr[q].Assigned = 2;           // ��־: �Ѿ�ͶƱ 
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

    //��¼ѡƱ�ͱ�־��Ŀ����ΪͶƱ��
    //register votes and mark entry as ballot box 
    ColorData[q].Deposit -= vote;
    ColorAttr[q].Assigned = 3;
  }

  // �������ֵ���Գ�Ϊ��ɫ�����
  //assign bits to colors opposite to partner's 

  for (i = 0; i < ColorCount; i++)
  {
    // ��û������ĵ���ɫ  
    if (ColorAttr[i].Assigned > 1)
    {
      p = 0;
      q = i;
      while (ColorAttr[q].Assigned == 2) // ��ѡ�У�����û����� 
      {
        p ^= 1u;
        q = ColorAttr[q].Partner;
      }

      if (ColorAttr[q].Assigned == 3)   // ͶƱ��û�й�ֵ�� ballot box not evaluated 
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
  AdjustColors: �Է���ı�����Ϊ����ȷ����ɫ�����ƥ�䣬��RED[0]�洢�������ֵ
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
      Color[i].RGB[0] &= ~1u;   // �����ں�ɫ�������Чλ�Ϸ������ 
      Color[i].RGB[0] |= ColorAttr[i].Assigned;
    }
  }
}
*/

/*********************************************************************
  IsGreyScale:	ȷ����ɫ���Ƿ�����ǻ�ɫ��Χ(greyscale)������������е�i ��
  							Palette[i].Red = .Green = .Blue = i
  						determines if palette is true greyscale, i.e.
              Palette[i].Red = .Green = .Blue = i for all i
  parameter:  ��; ʹ��BMPInfoSize��ȷ����ɫ��ƫ����
  returns:    �������UWORD16����
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
  FirstLine:  ��256ɫBMPͼ���ڵ�һ��nextblock����ʱ�����ʣ����еĳ�ʼ
  						������ɫ����񻥲���������ѻ�����������У�����ָ��
							���н�Զ�ĵ��õ�BMPNextBlock
  						is called on the 1st nextblock call with 256 colore BMPs;
              all initialization, palette and pixel analysis and
              optimization is done here; chains then to and directs
              all further calls to BMPNextBlock;
  parameter:  none; uses external variables:
              ��־λ�� hiding, fillup, smooth
              BMPInfoSize, position, width, height, LineOffset
              MediaLength, StegLength
  returns:    BMPNextBlock���صĴ���
              �ı���ⲿ����
              ColorAttr[256]�����б���ֵ���������ɫ������
*********************************************************************/
/*
UWORD32 FirstLine (void)
{
  int   i, j;
  long  ls, lc;

  if ((ColorAttr = calloc (256, sizeof (PalRelated))) == NULL)
    NoMemory ();

  // ת����ɫ�� 
  if (fseek (StegFile, 14 + BMPInfoSize, SEEK_SET))
    UnexpEOF ();

  if (hiding)
  {
    if (verbose > 1)
    {
      fputs ("\n������ɫ�� ... ", stderr);
      fflush (stderr);
    }

    if ((Color = calloc (256, sizeof (PalEntry))) == NULL)
      NoMemory ();

    for (i = 0; i < 256; i++)
    {
      // �������еĵ�ɫ����Ŀ��ת��ΪLab 
      for (j = 3; j--; )
        Color[i].RGB[j] = GetStegByte ();

      RGB2Lab (&Color[i]);

      if (BMPInfoSize != 12)  //if RGBQUAD: dummy read 
        GetStegByte ();
    }

    if (verbose > 1)
    {
      fputs ("Done.\n����ͼƬ ... ", stderr);
      fflush (stderr);
    }

    if ((ColorData = calloc (256, sizeof (PalStatist))) == NULL)
      NoMemory ();

    //������е����أ������ĸ�����ֵ���Ա��洢
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
    // ����Ƕ������Ŀ�ʼ���� 
    NextByte = GetLengthByte;
    LastLength = FALSE;
    StegLeft = StegLength;
    ResetScrambler ();

    if (verbose > 1)
    {
      fputs ("���.\n������ѱ������� ... ", stderr);
      fflush (stderr);
    }

    // �����Ǵ�����Ҫ���� 
    FindNearest ();
    if (fillup)
      FillGaps ();
    AssignBits ();

    if (verbose > 1)
    {
      fputs ("���.\n �Ż���ɫ�� ... ", stderr);
      fflush (stderr);
    }

    AdjustColors ();

    if (fillup && BMPInfoSize != 12)
    {
      // ��BMPͼƬ�ļ���Ϣͷ������ClrUsed/ClrImportant-Fields
      fseek (StegFile, 46, SEEK_SET);
      PutStegWord (ColorCount);
      PutStegWord (0);
      PutStegWord (ColorCount);
      PutStegWord (0);
    }

    // д�ı��˵ĵ�ɫ��
    fseek (StegFile, 14 + BMPInfoSize, SEEK_SET);
    for (i = 0; i < 256; i++)
    {
      // д���еĵ�ɫ����Ŀ
      for (j = 3; j--; )
        fputc (Color[i].RGB[j], StegFile);

      if (BMPInfoSize != 12)  // ���RGBQUAD: д0 
        fputc (0, StegFile);
    }

    free (ColorData);
    free (Color);

    if (verbose > 1)
    {
      fputs ("���.\nΪ���ݱ��� ... ", stderr);
      fflush (stderr);
    }
  }
  else
  {
    if (verbose > 1)
    {
      fputs ("\n��ȡ�������� ... ", stderr);
      fflush (stderr);
    }

    // ȷ����Red LSBָ����λ 
    for (i = 0; i < 256; i++)
    {
      GetStegByte ();
      GetStegByte ();
      ColorAttr[i].Assigned = GetStegByte () & 1u;
      if (BMPInfoSize != 12)  // ���RGBQUAD: dummy read 
        GetStegByte ();
    }

    if (verbose > 1)
    {
      fputs ("���.\nΪ���ݽ��� ... ", stderr);
      fflush (stderr);
    }
  }

  *NextLineM = BMPNextBlock;
  return BMPNextBlock ();
}
*/

/*********************************************************************
  BMPNextBlock: ָ��ͼ���ļ���һ��ɨ����(scanline)��ʼ���Ķ�ָ��λ��
  parameter:  �ޣ�ʹ���ⲿ����
  						λ�ã�ɨ����������ļ���ʼ����ƫ����(offset)
  						��ȣ�ɨ���еĿ�ȣ��ֽڣ�
  						�߶ȣ�ɨ���е�����
  						��ƫ�ƣ�����һ��ɨ���е�ƫ����
  returns:    ɨ���г��ȣ��ֽڣ��������EOF�ͷ���0
  						�ı��˵��ⲿ����
  						λ�ã�ָ����һ��ɨ����
  						�߶ȣ�ʣ��ɨ��������
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
  CheckIfBMP: �������ͼ���ļ������������λͼ��ʽ������ˮӡ�㷨�����жϣ�
              # of planes != 1, # of bits not 8 or 24, λͼ��ѹ����Ϊ
              BMPNextBlock�����ⲿ����
              λ�ã�ɨ����������ļ���ʼ����ƫ����
  						��ȣ�ɨ���еĿ�ȣ��ֽڣ�
  						�߶ȣ�ɨ���е�����
  						��ƫ�ƣ�����һ��ɨ���е�ƫ����
							��Ƕ��ʱ����8bit��ɫ��������£�
              1. with newer format and fewer than maximum # of colors
                 used or important these are reset to maximum for ste-
                 ganography almost certainly changes this number
              2. the palette is optimized it switches are set
  parameter:  ָ��'NextBlock'�����ָ���λ��
  						�ļ���ָ��ڶ����ֽڵĶ�ָ��
  returns:    ������Ͽ�Ϊλͼ��ʽ��ΪTRUE
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
  position = GetStegLong ();    /* ��Ե�һ��ɨ���е�ƫ���� */

  BMPInfoSize = GetStegLong ();
  switch (BMPInfoSize)//ͨ��ͷ�ļ���С����֪��λͼ��ʽ�������40����Windows�µ�λͼ�ļ�
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
    fputs ("λͼ��ʽ <", stderr);
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
        fputs ("24λ���ɫ (16.7 Mio", stderr);
        break;
      default:
        fprintf (stderr, "δ֪����ɫλ�� (? = 2 ^ %i", BitCount);
    }
    fputs (" colors).\n", stderr);

    if (verbose > 1)
      fprintf (stderr, "λͼ��С�� %li x %li ����.\n", width, height);
  }

  if (BMPInfoSize > 12)
  {
    if (GetStegLong ())
      ErrorExit (BMPCOMPRESS,
                 "����(BMP): λͼ��һ��(RLE-)ѹ�� - ����֧��.\n");

    if (verbose > 1)
    {
	/*������������һ�δ��룬����ȡ��ͼ��ͷ�ļ������ĸ��ֽ����ݣ����ĸ��ֽ���
	�����ݿ��Ա�ʶ����ͼ���ļ���ʵ���ֽڴ�С��Ҳ����Ϊ0��Ҫ�������Ϊ0ʱ����
	���������ʽ���֣�λͼ��ʵ�ʴ�С�ǣ�XX�ֽڡ�
	����ȡ�����ֽ�����ʹ��GetStegLong�������� ��steg.c�ļ��ﱻ���壬�������⣬���Բμ���ش��룻���ն������Ϣʹ��fprintf������
	���ǵľ����÷����Բο��ϡ����ĵ���ش��롣
		*/  
		
		long pam1=GetStegLong();
		long pam2=GetStegLong();		
		
		if(pam1!=0 && pam2!=0)
		{
			fprintf (stderr, "λͼ�ֱ��ʣ�ˮƽ����ÿӢ�� %li�� ",
				(pam1 * 127 + 2500) / 5000);
			fprintf (stderr, "����ֱ����ÿӢ�� %li�㡣\n",
				(pam2 * 127 + 2500) / 5000);
		}
    }
  }

  if (height == 0 || width == 0)
    ErrorExit (BMPNOPIXELS, "����(BMP): û��ͼƬ ?!?\n");

  if (planes != 1)
    ErrorExit (BMPNOT1PLANE,
               "����(BMP): ���ر��ֳɳ���һ��ƽ��.\n");

  switch (BitCount)
  {
    case 24:
      width *= 3;
    case 8:
      break;
    default:
      ErrorExit (BMPCOLORCNT,
        "����(BMP): ��ɫ�����޷�����ˮӡҪ��.\n");
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
    NextLineM = NextBlockP;         // ����ָ��λ�� 
		*/  
  }
  /*���������������ĸ�ֵ���̡������������ֱ��ǣ�
  �û�ѡ�����ÿ�ֽ�Ƕ���ˮӡ����λ����DataSize����һ�㳣��Ϊ1��
  ͼ����õ��ֽ����ݴ�С��MediaLength�����������ֽڿ�ȣ�width��
  �������ظ߶ȣ���ƫ������������Ϊ�ֽڿ�ȡ�
  */
  DataSize = 1;
  MediaLength = width * height;
  LineOffset = width + 3 & ~3ul;   // ����ɨ����ΪWORD32�߽� 

  return TRUE;
}
