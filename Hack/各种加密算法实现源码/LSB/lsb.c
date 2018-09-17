/*********************************************************************
  LSB数字水印工具:
  	利用LSB算法在BMP图像文件中隐藏文件.
  	这个项目所要隐藏或提取的文件必须被直接指明,或者通过管道以及stdin/stdout重定向指明.

  用法: 
  lsb [-v|-q] [-nf] [-nc] [-u|-nu] <bmpfile> [<file-to-hide>]
  lsb -x [-v|-q] <bmpfile>  [<destinationfile>]
  lsb -i <bmpfile>
  lsb <bmpfile> <watermarkfile>

  参数可以没有顺序(但第一个文件必须总是欲嵌入或提取水印的文件)并可以变换组合.

  对于非UNIX系统USE_FREOPEN和USE_SETMODE都必须被定义
*********************************************************************/

/* includes */
#define USE_FREOPEN
#define USE_SETMODE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#if defined (UNIX)
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <utime.h>
#else
  #include <sys\types.h>
  #include <sys\stat.h>
  #include <sys\utime.h>
  #if defined (USE_SETMODE)
    #include <fcntl.h>
    #include <io.h>
  #endif
#endif
#include "lsb.h"
#include "utils.h"
#include "StdAfx.h"

#define BytesForLength(x) (x < 64 ? 8 : x < 41024 ? 16 : x < 1089600 ? 24 : 32)

/*********************************************************************
  hello:  显示初始化提示和版权信息
*********************************************************************/

void hello (void)
{
  fputs ("LSB算法水印工具 - 在BMP图像中隐藏信息 \n"
         "(c) 2007 吉林中软吉大信息技术有限公司\n\n", stderr);
}


/*********************************************************************
  usage:  显示用法
*********************************************************************/

void usage (void)
{
  hello ();
  ErrorExit (NOARGUMENTS,
    "用法：\n"
    "    lsb [-v|-q] [-nf] [-nc] [-u|-nu] <bmpfile> [<file-to-hide>]\n"
    "    lsb -x [-v|-q] <bmpfile> [<destinationfile>]\n"
    "    lsb -i <bmpfile>\n"
    "    lsb <bmpfile> <watermarkfile>\n"
    "\n");
}


/*********************************************************************
  help:  显示详细帮助信息
*********************************************************************/

void help (void)
{
  hello ();
  ErrorExit (HELPNOACTION,
    " 查询一张BMP图片基本信息及其隐藏数据容易：\n"
    "    lsb -i <mediafile>\n"
    " 在BMP图片中隐藏信息,mediafile是BMP图片,secet.in是水印文件：\n"
    "    lsb [-options] <mediafile> <secret.in>\n"
    " 或使用管道：\n"
    "    <other program> | lsb [-options] <mediafile>\n"
    " 从图片中导出隐藏信息到secret.out：\n"
    "    lsb -x [-options] <mediafile> <secret.out>\n"
    "   或使用管道：\n"
    "    lsb -x [-options] <mediafile> | <other program>\n"
    "\n"
    " 以下是一些可能用到的命令选项：\n"
    "  -x  图像操作 (默认为嵌入水印)      -v  Verbose: 详细报告显示\n"
    "  -i  关于文件和隐藏能力的信息       -q  Quiet: 只报严重错误\n"
    "  -h  (或者是 ?) 帮助\n"
    "                                         'NOT'-options:\n"
    "                                    -nf  不伪造文件修改时间\n"
    " 只适用于256色位图（BMP）：         -nc  不改变调色板颜色\n"
    "  -u  使用更多没用过的调色板条目    -nu  不使用任何没用过的调色板条目\n");
}


/*********************************************************************
  main:       解析命令行,读图像文件的第一个字节,用以辨别所读取的文件
  						是不是BMP文件,然后调用调用类型、数据处理程序来处理下面
  						的大部分工作（水印嵌入、提取）
  						parses the command line, reads the first byte in the
              StegFile, decides which file type to assume, and then
              does most of the work by vectorized calls to type and
              data width specific routines
  parameters: 一个必需的文件和一些可选参数
  						one filename (required) and optional switches
*********************************************************************/

int main (int argc, char *argv[])
{
  UWORD32 (*NextBlock) (void);
  void    (*EditBlock) (UWORD32);
  int     c, MediaFilePos = 0, SecretFilePos = 0, fakedate = TRUE;
  UWORD32 block;
  struct utimbuf StegTime;

  if (argc < 3)
    usage ();

  /* 解析命令行 */
  while (--argc)		//输入参数不只一个
  {
    switch (*argv[argc])
    {
      case '-':
#if ! defined (UNIX)
      case '/':
#endif
        while (c = *++argv[argc])
        {
          switch (toupper (c))		//toupper为将输入转换为大写字母
          {
            case '?':
            case 'H':
              help ();		//参数为？或H就调用帮助
              break;

            case 'I':
              verbose = 3;
              hiding = FALSE;
              break;

            case 'N':
              c = *++argv[argc];
              switch (toupper (c))
              {
                case 'C':
                  smooth = FALSE;
                  break;

                case 'F':
                  fakedate = FALSE;
                  break;

                case 'U':
                  fillup = 0;
                  break;

                default:
                 if (verbose) hello ();
                  ErrorExit (INVALIDSWITCH,
                             "错误：不是一个可用的否定参数.\n");
              }
              break;

            case 'U':
              fillup = 2;
              break;

            case 'Q':
              verbose = 0;
              break;

            case 'V':
              verbose = 2;
              break;

            case 'X':
              hiding = FALSE;
              break;

            default:
              if (verbose) hello ();
              ErrorExit (INVALIDSWITCH, "错误: 参数错误.\n");
          }
        }
        break;

      default:
        if (! MediaFilePos)					//图像文件位置标识符未赋值
          MediaFilePos = argc;
        else                         /* 图像文件已经找到 */
          if (! SecretFilePos)			//水印文件位置标识符未赋值
          {
            SecretFilePos = MediaFilePos;
            MediaFilePos = argc;
          }
          else                       /* 水印文件也已经找到 */
          {
            if (verbose) hello ();
            ErrorExit (THIRDFILENAME,
              "错误: 超过两个文件被指定.\n");
          }
    }
  }

  if (verbose) hello ();

  if (! MediaFilePos)
    ErrorExit (NOFILENAME, "错误: 无法找到所需图像文件.\n");

  /* 打开图像文件以进行水印操作 */
  StegFile = fopen (argv[MediaFilePos], hiding ? "r+b" : "rb");
  if (StegFile == NULL)
  {
    fputs ("错误: 无法打开 <", stderr);
    fputs (argv[MediaFilePos], stderr);
    verbose = 0;
    ErrorExit (OPENERROR, "> for steganography.\n");
  }

  if (hiding && fakedate)			//如果要伪造图像修改时间，则需要保存图像原始时间，待嵌完水印后再把时间修改回来
  {
    struct stat StegStats;
    fstat (fileno (StegFile), &StegStats);
    StegTime.actime = StegStats.st_atime;
    StegTime.modtime = StegStats.st_mtime;
  }

  /* 通过文件第一个字节确定输入文件是否为BMP图片 */
  c = GetStegByte ();
  if (!(c == 'B' && CheckIfBMP (&NextBlock) ))
    ErrorExit (UNKNOWNTYPE, "错误: 输入的不是BMP图片.\n");

  if (verbose == 3)			//查看文件详情
  {
	  /*请在这里填入几行代码，它的作用是输出图像文件可以嵌入的最多的字节数，
	  在输出时请使用fprintf方法，它的用法可以参照前面用到的相关代码。这段
	  代码的关键是在已知图像宽度（以字节为单位）MediaLength、每字节最多可嵌
	  入比特数MaxBitsAllowed，要算出这个图像最多可以嵌入多少字节数据。要
	  注意这里数据指的是有意义的数据，要排除掉标识所有可嵌入数据的长度代码。
	  */  


    verbose = 0;

    goto SkipStegSection;     /* 这里为了不写出一个很长的else语句，使用跳转 */
  }

  if (SecretFilePos)
  {
    /* 打开水印文件以进行水印操作 */
    SecretFile = fopen (argv[SecretFilePos], hiding ? "rb" : "wb");
    if (SecretFile == NULL)
    {
      fputs ("错误: 无法打开欲嵌入的水印文件 <", stderr);
      fputs (argv[SecretFilePos], stderr);
      verbose = 0;
      ErrorExit (OPENERROR, ">.\n");
    }
  }
  else
  {
  /* 设置stdin/stdout 为二进制 */
#if ! defined (UNIX)
#if defined (USE_FREOPEN)
    if (hiding)
      freopen ("", "rb", stdin);
    else
      freopen ("", "wb", stdout);
#elif defined (USE_SETMODE)
    setmode (fileno (hiding ? stdin : stdout), O_BINARY);
#else
  #error 你必须定义一个宏来选择设置stdin/out为二进制的方法

#endif
#endif
    if (! hiding)
      SecretFile = stdout;
  }

  if (hiding)
  {
    if (SecretFilePos)
    {
      fseek (SecretFile, 0L, SEEK_END);
      StegLength = ftell (SecretFile);
    }
    else
    {
      if (verbose > 1)
      {
        fputs ("读取欲隐藏的数据 ... ", stderr);
        fflush (stderr);
      }

      /* 为从stdin读取的隐藏数据打开一个临时文件 */
      if ((SecretFile = tmpfile ()) == NULL)
        ErrorExit (TEMPERROR, "出错：无法打开临时文件。");

      /* 读取并计数从stdin到临时文件的隐藏数据量 */
      while ((c = getchar ()) != EOF)
      {
        putc (c, SecretFile);
        StegLength++;
      }

      if (verbose > 1)
        fputs ("完成.\n", stderr);
    }

    /*检查水印存储能力 */
    if (StegLength > (MediaLength - BytesForLength (StegLength))
                     * MaxBitsAllowed / 8)
      ErrorExit (CAPACITYEX, "出错：BMP图片容易不够，请选择大一点的图片添加水印，或者选择一个更小的水印图标。\n");

    StegLeft = StegLength;
    EditBlock = SetBlock;
    NextByte = GetLengthByte;
  }
  else
  {
    EditBlock = GetBlock;
    OutputByte = LengthOut;
  }

  if (verbose > 1)
  {
    fputs (hiding ? "正在嵌入 ... " : "正在提取 ... ", stderr);
    fflush (stderr);
  }

  /* 主要的工作循环，在做填程序第三步时请将以下两行的注释标识符去掉*/
/*  while (block = (*NextBlock) ())
	(*EditBlock) (block);
*/

  /* 清理和总结 */

  fclose (SecretFile);

SkipStegSection:			//从查看文件详情跳转过来
  fclose (StegFile);

  if (hiding && fakedate)
    utime (argv[MediaFilePos], &StegTime);

/*判断水印数据是否全部嵌入或提取，在做填程序第三步时请将以下两行的注释标识符去掉*/
/*  if (StegLeft)
	UnexpEOF ();
*/

  if (verbose)
  {
    if (verbose > 1)
      fprintf (stderr, "完成.\n%ld 字节 %s 文件 <%s>.\n",
               StegLength,
               hiding ? "隐藏于" : "提取于",
               argv[MediaFilePos]);
    fputs ("水印操作完成.\n", stderr);
  }

  return 0;
}
