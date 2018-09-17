/*********************************************************************
  LSB����ˮӡ����:
  	����LSB�㷨��BMPͼ���ļ��������ļ�.
  	�����Ŀ��Ҫ���ػ���ȡ���ļ����뱻ֱ��ָ��,����ͨ���ܵ��Լ�stdin/stdout�ض���ָ��.

  �÷�: 
  lsb [-v|-q] [-nf] [-nc] [-u|-nu] <bmpfile> [<file-to-hide>]
  lsb -x [-v|-q] <bmpfile>  [<destinationfile>]
  lsb -i <bmpfile>
  lsb <bmpfile> <watermarkfile>

  ��������û��˳��(����һ���ļ�����������Ƕ�����ȡˮӡ���ļ�)�����Ա任���.

  ���ڷ�UNIXϵͳUSE_FREOPEN��USE_SETMODE�����뱻����
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
  hello:  ��ʾ��ʼ����ʾ�Ͱ�Ȩ��Ϣ
*********************************************************************/

void hello (void)
{
  fputs ("LSB�㷨ˮӡ���� - ��BMPͼ����������Ϣ \n"
         "(c) 2007 ������������Ϣ�������޹�˾\n\n", stderr);
}


/*********************************************************************
  usage:  ��ʾ�÷�
*********************************************************************/

void usage (void)
{
  hello ();
  ErrorExit (NOARGUMENTS,
    "�÷���\n"
    "    lsb [-v|-q] [-nf] [-nc] [-u|-nu] <bmpfile> [<file-to-hide>]\n"
    "    lsb -x [-v|-q] <bmpfile> [<destinationfile>]\n"
    "    lsb -i <bmpfile>\n"
    "    lsb <bmpfile> <watermarkfile>\n"
    "\n");
}


/*********************************************************************
  help:  ��ʾ��ϸ������Ϣ
*********************************************************************/

void help (void)
{
  hello ();
  ErrorExit (HELPNOACTION,
    " ��ѯһ��BMPͼƬ������Ϣ���������������ף�\n"
    "    lsb -i <mediafile>\n"
    " ��BMPͼƬ��������Ϣ,mediafile��BMPͼƬ,secet.in��ˮӡ�ļ���\n"
    "    lsb [-options] <mediafile> <secret.in>\n"
    " ��ʹ�ùܵ���\n"
    "    <other program> | lsb [-options] <mediafile>\n"
    " ��ͼƬ�е���������Ϣ��secret.out��\n"
    "    lsb -x [-options] <mediafile> <secret.out>\n"
    "   ��ʹ�ùܵ���\n"
    "    lsb -x [-options] <mediafile> | <other program>\n"
    "\n"
    " ������һЩ�����õ�������ѡ�\n"
    "  -x  ͼ����� (Ĭ��ΪǶ��ˮӡ)      -v  Verbose: ��ϸ������ʾ\n"
    "  -i  �����ļ���������������Ϣ       -q  Quiet: ֻ�����ش���\n"
    "  -h  (������ ?) ����\n"
    "                                         'NOT'-options:\n"
    "                                    -nf  ��α���ļ��޸�ʱ��\n"
    " ֻ������256ɫλͼ��BMP����         -nc  ���ı��ɫ����ɫ\n"
    "  -u  ʹ�ø���û�ù��ĵ�ɫ����Ŀ    -nu  ��ʹ���κ�û�ù��ĵ�ɫ����Ŀ\n");
}


/*********************************************************************
  main:       ����������,��ͼ���ļ��ĵ�һ���ֽ�,���Ա������ȡ���ļ�
  						�ǲ���BMP�ļ�,Ȼ����õ������͡����ݴ����������������
  						�Ĵ󲿷ֹ�����ˮӡǶ�롢��ȡ��
  						parses the command line, reads the first byte in the
              StegFile, decides which file type to assume, and then
              does most of the work by vectorized calls to type and
              data width specific routines
  parameters: һ��������ļ���һЩ��ѡ����
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

  /* ���������� */
  while (--argc)		//���������ֻһ��
  {
    switch (*argv[argc])
    {
      case '-':
#if ! defined (UNIX)
      case '/':
#endif
        while (c = *++argv[argc])
        {
          switch (toupper (c))		//toupperΪ������ת��Ϊ��д��ĸ
          {
            case '?':
            case 'H':
              help ();		//����Ϊ����H�͵��ð���
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
                             "���󣺲���һ�����õķ񶨲���.\n");
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
              ErrorExit (INVALIDSWITCH, "����: ��������.\n");
          }
        }
        break;

      default:
        if (! MediaFilePos)					//ͼ���ļ�λ�ñ�ʶ��δ��ֵ
          MediaFilePos = argc;
        else                         /* ͼ���ļ��Ѿ��ҵ� */
          if (! SecretFilePos)			//ˮӡ�ļ�λ�ñ�ʶ��δ��ֵ
          {
            SecretFilePos = MediaFilePos;
            MediaFilePos = argc;
          }
          else                       /* ˮӡ�ļ�Ҳ�Ѿ��ҵ� */
          {
            if (verbose) hello ();
            ErrorExit (THIRDFILENAME,
              "����: ���������ļ���ָ��.\n");
          }
    }
  }

  if (verbose) hello ();

  if (! MediaFilePos)
    ErrorExit (NOFILENAME, "����: �޷��ҵ�����ͼ���ļ�.\n");

  /* ��ͼ���ļ��Խ���ˮӡ���� */
  StegFile = fopen (argv[MediaFilePos], hiding ? "r+b" : "rb");
  if (StegFile == NULL)
  {
    fputs ("����: �޷��� <", stderr);
    fputs (argv[MediaFilePos], stderr);
    verbose = 0;
    ErrorExit (OPENERROR, "> for steganography.\n");
  }

  if (hiding && fakedate)			//���Ҫα��ͼ���޸�ʱ�䣬����Ҫ����ͼ��ԭʼʱ�䣬��Ƕ��ˮӡ���ٰ�ʱ���޸Ļ���
  {
    struct stat StegStats;
    fstat (fileno (StegFile), &StegStats);
    StegTime.actime = StegStats.st_atime;
    StegTime.modtime = StegStats.st_mtime;
  }

  /* ͨ���ļ���һ���ֽ�ȷ�������ļ��Ƿ�ΪBMPͼƬ */
  c = GetStegByte ();
  if (!(c == 'B' && CheckIfBMP (&NextBlock) ))
    ErrorExit (UNKNOWNTYPE, "����: ����Ĳ���BMPͼƬ.\n");

  if (verbose == 3)			//�鿴�ļ�����
  {
	  /*�����������뼸�д��룬�������������ͼ���ļ�����Ƕ��������ֽ�����
	  �����ʱ��ʹ��fprintf�����������÷����Բ���ǰ���õ�����ش��롣���
	  ����Ĺؼ�������֪ͼ���ȣ����ֽ�Ϊ��λ��MediaLength��ÿ�ֽ�����Ƕ
	  �������MaxBitsAllowed��Ҫ������ͼ��������Ƕ������ֽ����ݡ�Ҫ
	  ע����������ָ��������������ݣ�Ҫ�ų�����ʶ���п�Ƕ�����ݵĳ��ȴ��롣
	  */  


    verbose = 0;

    goto SkipStegSection;     /* ����Ϊ�˲�д��һ���ܳ���else��䣬ʹ����ת */
  }

  if (SecretFilePos)
  {
    /* ��ˮӡ�ļ��Խ���ˮӡ���� */
    SecretFile = fopen (argv[SecretFilePos], hiding ? "rb" : "wb");
    if (SecretFile == NULL)
    {
      fputs ("����: �޷�����Ƕ���ˮӡ�ļ� <", stderr);
      fputs (argv[SecretFilePos], stderr);
      verbose = 0;
      ErrorExit (OPENERROR, ">.\n");
    }
  }
  else
  {
  /* ����stdin/stdout Ϊ������ */
#if ! defined (UNIX)
#if defined (USE_FREOPEN)
    if (hiding)
      freopen ("", "rb", stdin);
    else
      freopen ("", "wb", stdout);
#elif defined (USE_SETMODE)
    setmode (fileno (hiding ? stdin : stdout), O_BINARY);
#else
  #error ����붨��һ������ѡ������stdin/outΪ�����Ƶķ���

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
        fputs ("��ȡ�����ص����� ... ", stderr);
        fflush (stderr);
      }

      /* Ϊ��stdin��ȡ���������ݴ�һ����ʱ�ļ� */
      if ((SecretFile = tmpfile ()) == NULL)
        ErrorExit (TEMPERROR, "�����޷�����ʱ�ļ���");

      /* ��ȡ��������stdin����ʱ�ļ������������� */
      while ((c = getchar ()) != EOF)
      {
        putc (c, SecretFile);
        StegLength++;
      }

      if (verbose > 1)
        fputs ("���.\n", stderr);
    }

    /*���ˮӡ�洢���� */
    if (StegLength > (MediaLength - BytesForLength (StegLength))
                     * MaxBitsAllowed / 8)
      ErrorExit (CAPACITYEX, "����BMPͼƬ���ײ�������ѡ���һ���ͼƬ���ˮӡ������ѡ��һ����С��ˮӡͼ�ꡣ\n");

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
    fputs (hiding ? "����Ƕ�� ... " : "������ȡ ... ", stderr);
    fflush (stderr);
  }

  /* ��Ҫ�Ĺ���ѭ������������������ʱ�뽫�������е�ע�ͱ�ʶ��ȥ��*/
/*  while (block = (*NextBlock) ())
	(*EditBlock) (block);
*/

  /* ������ܽ� */

  fclose (SecretFile);

SkipStegSection:			//�Ӳ鿴�ļ�������ת����
  fclose (StegFile);

  if (hiding && fakedate)
    utime (argv[MediaFilePos], &StegTime);

/*�ж�ˮӡ�����Ƿ�ȫ��Ƕ�����ȡ����������������ʱ�뽫�������е�ע�ͱ�ʶ��ȥ��*/
/*  if (StegLeft)
	UnexpEOF ();
*/

  if (verbose)
  {
    if (verbose > 1)
      fprintf (stderr, "���.\n%ld �ֽ� %s �ļ� <%s>.\n",
               StegLength,
               hiding ? "������" : "��ȡ��",
               argv[MediaFilePos]);
    fputs ("ˮӡ�������.\n", stderr);
  }

  return 0;
}
