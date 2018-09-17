
#define MAX_NUM		63001
#define MAX_PRIME	251

//! ·µ»Ø´úÂë
#define OK					100
#define ERROR_NOEACHPRIME	101
#define ERROR_NOPUBLICKEY	102
#define	ERROR_GENERROR		103

unsigned int MakePrivatedKeyd( unsigned int uiP, unsigned int uiQ );
unsigned int GetPrivateKeyd( unsigned int iWhich );
unsigned int MakePairkey( unsigned int uiP, unsigned int uiQ, unsigned int uiD );
unsigned int GetPairKey( unsigned int &d, unsigned int &e );
void rsa_encrypt( int n, int e, char *mw, int iLength, int *&cw );
void rsa_decrypt( int n, int d, int *&cw, int cLength, char *mw );
