/*********************************************************************
  Header-file for the utility module of Hide4PGP by Heinz Repp
  last modified: 12/12/99
*********************************************************************/

/* extern variable declarations */
extern int hiding, verbose, fillup, smooth;
extern FILE *StegFile;
extern FILE *SecretFile;
extern WORD32 StegLength;
extern WORD32 StegLeft;
extern WORD32 MediaLength;
extern size_t DataSize;
extern int MaxBitsAllowed;
extern int LastLength;
extern void (*ModData) (void *, size_t);
extern unsigned int (*GetBits) (int);
extern UBYTE (*NextByte) (void);
extern void (*OutputByte) (UBYTE);

/* extern functions */
UBYTE GetStegByte (void);
UWORD16 GetStegWord (void);
UWORD32 GetStegLong (void);
size_t PutStegWord (UWORD16);
void ErrorExit (int, char *);
void UnexpEOF (void);
void NoMemory (void);
int NextIs (char *);
void TurnWheel (void);
void SetBlock (UWORD32);
void GetBlock (UWORD32);
unsigned int NextBits (int);
int BitsThisTime (void);
void ModLBytes (UBYTE *, size_t);
void ModLWords (UWORD16 *, size_t);
unsigned int GetBitsFromByte (int);
unsigned int GetBitsFromWord (int);
UBYTE GetLengthByte (void);
void LengthOut (UBYTE);
