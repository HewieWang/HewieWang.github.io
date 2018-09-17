/*********************************************************************
  Header-file for all modules of lsb by Heinz Repp
  last modified: 12/04/99
*********************************************************************/

/* defines */
#define FALSE 0
#define TRUE 1
/* redefine NULL due to a problem with IBMCPP headers */
#undef NULL
#define NULL ((void *) 0)

/* Returncodes */
#define HELPNOACTION  0
#define NOARGUMENTS   1
#define INVALIDSWITCH 2
#define THIRDFILENAME 3
#define NOFILENAME    4
#define OPENERROR     5
#define TEMPERROR     6
#define UNKNOWNTYPE   7
#define CAPACITYEX    8
#define INVALIDLENGTH 9
#define OUTOFMEMORY  10
#define UNEXPEOF     11
#define OUTOFSYNC    12
#define BMPNOPIXELS  20
#define BMPNOT1PLANE 21
#define BMPCOLORCNT  22
#define BMPCOMPRESS  23

/* typedefs */
typedef signed char SBYTE;
typedef unsigned char UBYTE;
typedef short int WORD16;
typedef unsigned short int UWORD16;
typedef long int WORD32;
typedef unsigned long int UWORD32;

/* extern functions */
int CheckIfBMP (UWORD32 (**) (void));
