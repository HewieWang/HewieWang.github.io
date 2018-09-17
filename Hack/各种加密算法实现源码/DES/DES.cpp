#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <afxwin.h>

typedef struct {
    unsigned int k0246[16], k1357[16];
    unsigned int iv0, iv1;
} DESContext;

struct DesKo
{
	unsigned int KeyOPC1_Before[2];
	unsigned int KeyOPC1_Later[2];
	unsigned int KeyOPC2_Before[17][2];
	unsigned int KeyOPC2_Later[17][2];
	int    KeyOSiftLeft[16];
};

struct DesEo
{
	unsigned int ItLR[17][2];
};

struct DesFo
{
	unsigned int FoR;
	unsigned int FoKey[2];
	unsigned int FoSPBefore[2];
	unsigned int FoSPLater;
};

struct DesEno
{
	unsigned int IPBeforeLR[2];
	unsigned int IPLaterLR[2];
	unsigned int FPBeforeLR[2];
	unsigned int FPLaterLR[2];
	DesEo eo;
	DesFo fo[17];
	DesKo keyInfo;
};

//! 32 位数x 循环向左移动c位
#define rotl(x, c) ( (x << c) | (x >> (32-c)) )
//! 32位存储的28位数x循环向左移动c比特,低28位有效
#define rotl28(x, c) ( ( (x << c) | (x >> (28-c)) ) & 0x0FFFFFFF)
//!　初始置换表
#define IP(L, R) (\
    bitswap(R, L,  4, 0x0F0F0F0F), \
    bitswap(R, L, 16, 0x0000FFFF), \
    bitswap(L, R,  2, 0x33333333), \
    bitswap(L, R,  8, 0x00FF00FF), \
    bitswap(R, L,  1, 0x55555555))
//! 终结置换表
#define FP(L, R) (\
    bitswap(R, L,  1, 0x55555555), \
    bitswap(L, R,  8, 0x00FF00FF), \
    bitswap(L, R,  2, 0x33333333), \
    bitswap(R, L, 16, 0x0000FFFF), \
    bitswap(R, L,  4, 0x0F0F0F0F))
//! 位交换
#define bitswap(L, R, n, mask) (\
    swap = mask & ( (R >> n) ^ L ), \
    R ^= swap << n, \
    L ^= swap)

static const unsigned int SPboxes[8][64] = {
    {0x01010400, 0x00000000, 0x00010000, 0x01010404,
     0x01010004, 0x00010404, 0x00000004, 0x00010000,
     0x00000400, 0x01010400, 0x01010404, 0x00000400,
     0x01000404, 0x01010004, 0x01000000, 0x00000004,
     0x00000404, 0x01000400, 0x01000400, 0x00010400,
     0x00010400, 0x01010000, 0x01010000, 0x01000404,
     0x00010004, 0x01000004, 0x01000004, 0x00010004,
     0x00000000, 0x00000404, 0x00010404, 0x01000000,
     0x00010000, 0x01010404, 0x00000004, 0x01010000,
     0x01010400, 0x01000000, 0x01000000, 0x00000400,
     0x01010004, 0x00010000, 0x00010400, 0x01000004,
     0x00000400, 0x00000004, 0x01000404, 0x00010404,
     0x01010404, 0x00010004, 0x01010000, 0x01000404,
     0x01000004, 0x00000404, 0x00010404, 0x01010400,
     0x00000404, 0x01000400, 0x01000400, 0x00000000,
     0x00010004, 0x00010400, 0x00000000, 0x01010004L},

    {0x80108020, 0x80008000, 0x00008000, 0x00108020,
     0x00100000, 0x00000020, 0x80100020, 0x80008020,
     0x80000020, 0x80108020, 0x80108000, 0x80000000,
     0x80008000, 0x00100000, 0x00000020, 0x80100020,
     0x00108000, 0x00100020, 0x80008020, 0x00000000,
     0x80000000, 0x00008000, 0x00108020, 0x80100000,
     0x00100020, 0x80000020, 0x00000000, 0x00108000,
     0x00008020, 0x80108000, 0x80100000, 0x00008020,
     0x00000000, 0x00108020, 0x80100020, 0x00100000,
     0x80008020, 0x80100000, 0x80108000, 0x00008000,
     0x80100000, 0x80008000, 0x00000020, 0x80108020,
     0x00108020, 0x00000020, 0x00008000, 0x80000000,
     0x00008020, 0x80108000, 0x00100000, 0x80000020,
     0x00100020, 0x80008020, 0x80000020, 0x00100020,
     0x00108000, 0x00000000, 0x80008000, 0x00008020,
     0x80000000, 0x80100020, 0x80108020, 0x00108000L},

    {0x00000208, 0x08020200, 0x00000000, 0x08020008,
     0x08000200, 0x00000000, 0x00020208, 0x08000200,
     0x00020008, 0x08000008, 0x08000008, 0x00020000,
     0x08020208, 0x00020008, 0x08020000, 0x00000208,
     0x08000000, 0x00000008, 0x08020200, 0x00000200,
     0x00020200, 0x08020000, 0x08020008, 0x00020208,
     0x08000208, 0x00020200, 0x00020000, 0x08000208,
     0x00000008, 0x08020208, 0x00000200, 0x08000000,
     0x08020200, 0x08000000, 0x00020008, 0x00000208,
     0x00020000, 0x08020200, 0x08000200, 0x00000000,
     0x00000200, 0x00020008, 0x08020208, 0x08000200,
     0x08000008, 0x00000200, 0x00000000, 0x08020008,
     0x08000208, 0x00020000, 0x08000000, 0x08020208,
     0x00000008, 0x00020208, 0x00020200, 0x08000008,
     0x08020000, 0x08000208, 0x00000208, 0x08020000,
     0x00020208, 0x00000008, 0x08020008, 0x00020200L},

    {0x00802001, 0x00002081, 0x00002081, 0x00000080,
     0x00802080, 0x00800081, 0x00800001, 0x00002001,
     0x00000000, 0x00802000, 0x00802000, 0x00802081,
     0x00000081, 0x00000000, 0x00800080, 0x00800001,
     0x00000001, 0x00002000, 0x00800000, 0x00802001,
     0x00000080, 0x00800000, 0x00002001, 0x00002080,
     0x00800081, 0x00000001, 0x00002080, 0x00800080,
     0x00002000, 0x00802080, 0x00802081, 0x00000081,
     0x00800080, 0x00800001, 0x00802000, 0x00802081,
     0x00000081, 0x00000000, 0x00000000, 0x00802000,
     0x00002080, 0x00800080, 0x00800081, 0x00000001,
     0x00802001, 0x00002081, 0x00002081, 0x00000080,
     0x00802081, 0x00000081, 0x00000001, 0x00002000,
     0x00800001, 0x00002001, 0x00802080, 0x00800081,
     0x00002001, 0x00002080, 0x00800000, 0x00802001,
     0x00000080, 0x00800000, 0x00002000, 0x00802080L},

    {0x00000100, 0x02080100, 0x02080000, 0x42000100,
     0x00080000, 0x00000100, 0x40000000, 0x02080000,
     0x40080100, 0x00080000, 0x02000100, 0x40080100,
     0x42000100, 0x42080000, 0x00080100, 0x40000000,
     0x02000000, 0x40080000, 0x40080000, 0x00000000,
     0x40000100, 0x42080100, 0x42080100, 0x02000100,
     0x42080000, 0x40000100, 0x00000000, 0x42000000,
     0x02080100, 0x02000000, 0x42000000, 0x00080100,
     0x00080000, 0x42000100, 0x00000100, 0x02000000,
     0x40000000, 0x02080000, 0x42000100, 0x40080100,
     0x02000100, 0x40000000, 0x42080000, 0x02080100,
     0x40080100, 0x00000100, 0x02000000, 0x42080000,
     0x42080100, 0x00080100, 0x42000000, 0x42080100,
     0x02080000, 0x00000000, 0x40080000, 0x42000000,
     0x00080100, 0x02000100, 0x40000100, 0x00080000,
     0x00000000, 0x40080000, 0x02080100, 0x40000100L},

    {0x20000010, 0x20400000, 0x00004000, 0x20404010,
     0x20400000, 0x00000010, 0x20404010, 0x00400000,
     0x20004000, 0x00404010, 0x00400000, 0x20000010,
     0x00400010, 0x20004000, 0x20000000, 0x00004010,
     0x00000000, 0x00400010, 0x20004010, 0x00004000,
     0x00404000, 0x20004010, 0x00000010, 0x20400010,
     0x20400010, 0x00000000, 0x00404010, 0x20404000,
     0x00004010, 0x00404000, 0x20404000, 0x20000000,
     0x20004000, 0x00000010, 0x20400010, 0x00404000,
     0x20404010, 0x00400000, 0x00004010, 0x20000010,
     0x00400000, 0x20004000, 0x20000000, 0x00004010,
     0x20000010, 0x20404010, 0x00404000, 0x20400000,
     0x00404010, 0x20404000, 0x00000000, 0x20400010,
     0x00000010, 0x00004000, 0x20400000, 0x00404010,
     0x00004000, 0x00400010, 0x20004010, 0x00000000,
     0x20404000, 0x20000000, 0x00400010, 0x20004010L},

    {0x00200000, 0x04200002, 0x04000802, 0x00000000,
     0x00000800, 0x04000802, 0x00200802, 0x04200800,
     0x04200802, 0x00200000, 0x00000000, 0x04000002,
     0x00000002, 0x04000000, 0x04200002, 0x00000802,
     0x04000800, 0x00200802, 0x00200002, 0x04000800,
     0x04000002, 0x04200000, 0x04200800, 0x00200002,
     0x04200000, 0x00000800, 0x00000802, 0x04200802,
     0x00200800, 0x00000002, 0x04000000, 0x00200800,
     0x04000000, 0x00200800, 0x00200000, 0x04000802,
     0x04000802, 0x04200002, 0x04200002, 0x00000002,
     0x00200002, 0x04000000, 0x04000800, 0x00200000,
     0x04200800, 0x00000802, 0x00200802, 0x04200800,
     0x00000802, 0x04000002, 0x04200802, 0x04200000,
     0x00200800, 0x00000000, 0x00000002, 0x04200802,
     0x00000000, 0x00200802, 0x04200000, 0x00000800,
     0x04000002, 0x04000800, 0x00000800, 0x00200002L},

    {0x10001040, 0x00001000, 0x00040000, 0x10041040,
     0x10000000, 0x10001040, 0x00000040, 0x10000000,
     0x00040040, 0x10040000, 0x10041040, 0x00041000,
     0x10041000, 0x00041040, 0x00001000, 0x00000040,
     0x10040000, 0x10000040, 0x10001000, 0x00001040,
     0x00041000, 0x00040040, 0x10040040, 0x10041000,
     0x00001040, 0x00000000, 0x00000000, 0x10040040,
     0x10000040, 0x10001000, 0x00041040, 0x00040000,
     0x00041040, 0x00040000, 0x10041000, 0x00001000,
     0x00000040, 0x10040040, 0x00001000, 0x00041040,
     0x10001000, 0x00000040, 0x10000040, 0x10040000,
     0x10040040, 0x10000000, 0x00040000, 0x10001040,
     0x00000000, 0x10041040, 0x00040040, 0x10000040,
     0x10040000, 0x10001000, 0x10001040, 0x00000000,
     0x10041040, 0x00041000, 0x00041000, 0x00001040,
     0x00001040, 0x00040040, 0x10000000, 0x10041000L}
};

/******************************************************************************/
//	名称：bitsel
//	功能：位选择器
//	参数：input:用于指定选择数据大于32比特的情况;
//		  bitnums:指定要选择的位号
//		  size:指定bitnums大小
//	返回：由选择位构成的一个32位数

/******************************************************************************/
unsigned int bitsel(unsigned int * input, const int *bitnums, int size)
{
    unsigned int ret = 0;
    while(size--) 
	{
		int bitpos = *bitnums++;
		ret <<= 1;
		if (bitpos >= 0)
			ret |= 1 & (input[bitpos / 32] >> (bitpos % 32));
    }

    return ret;
}

unsigned int f(unsigned int &R, unsigned int &K0246,
			  unsigned int &K1357, DesFo* pfo=NULL) 
{	
	DesFo fo;
	ZeroMemory(&fo,sizeof(DesFo));
	fo.FoR = R; 
	

	fo.FoKey[0] = K0246;
	fo.FoKey[1] = K1357;

	unsigned int s0246, s1357;
    s0246 = R ^ K0246;
    s1357 = R ^ K1357;
    s0246 = rotl(s0246, 28);
	
	fo.FoSPBefore[0] |= (((s0246 >> 24) & 0x3F)	  );
	fo.FoSPBefore[0] |= (((s1357 >> 24) & 0x3F) << 6 );
	fo.FoSPBefore[0] |= (((s0246 >> 16) & 0x3F) << 12);
	fo.FoSPBefore[0] |= (((s1357 >> 16) & 0x3F) << 18);
	fo.FoSPBefore[0] |= (((s0246 >>  8) & 0x3F) << 24);
	fo.FoSPBefore[0] |= (((s1357 >>  8) & 0x3F) << 30);
	fo.FoSPBefore[1] |= (((s1357 >>  8) & 0x3F) >> 2 );
	fo.FoSPBefore[1] |= (((s0246      ) & 0x3F) << 4 );
	fo.FoSPBefore[1] |= (((s1357	   ) & 0x3F) << 10);

	fo.FoSPLater = ( SPboxes[0] [(s0246 >> 24) & 0x3F] | 
	SPboxes[1] [(s1357 >> 24) & 0x3F] | 
	SPboxes[2] [(s0246 >> 16) & 0x3F] | 
	SPboxes[3] [(s1357 >> 16) & 0x3F] | 
	SPboxes[4] [(s0246 >>  8) & 0x3F] | 
	SPboxes[5] [(s1357 >>  8) & 0x3F] | 
	SPboxes[6] [(s0246      ) & 0x3F] | 
	SPboxes[7] [(s1357      ) & 0x3F]);
	if( pfo != NULL )
	{
		(*pfo) = fo;
	}

	return  fo.FoSPLater;
}

/******************************************************************************/
//	名称：des_key_setup
//	功能：产生子密钥；子密钥16轮循环
//	返回：无

/******************************************************************************/
void des_key_setup(unsigned int key_msw, unsigned int key_lsw,
				   DESContext * sched,DesKo* pK = NULL )
{

    static const int PC1_Cbits[] = {
	7, 15, 23, 31, 39, 47, 55, 63, 6, 14, 22, 30, 38, 46,
	54, 62, 5, 13, 21, 29, 37, 45, 53, 61, 4, 12, 20, 28
    };
    static const int PC1_Dbits[] = {
	1, 9, 17, 25, 33, 41, 49, 57, 2, 10, 18, 26, 34, 42,
	50, 58, 3, 11, 19, 27, 35, 43, 51, 59, 36, 44, 52, 60
    };

    static const int PC2_0246[] = {
	49, 36, 59, 55, -1, -1, 37, 41, 48, 56, 34, 52, -1, -1, 15, 4,
	25, 19, 9, 1, -1, -1, 12, 7, 17, 0, 22, 3, -1, -1, 46, 43
    };
    static const int PC2_1357[] = {
	-1, -1, 57, 32, 45, 54, 39, 50, -1, -1, 44, 53, 33, 40, 47, 58,
	-1, -1, 26, 16, 5, 11, 23, 8, -1, -1, 10, 14, 6, 20, 27, 24
    };
    static const int leftshifts[] =
	{ 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

    unsigned int C, D;
    unsigned int buf[2];
    int i;

    buf[0] = key_lsw;
    buf[1] = key_msw;
	unsigned int KeyOPC1_Before[2];
	unsigned int KeyOPC1_Later[2];
	unsigned int KeyOPC2_Before[16][2];
	unsigned int KeyOPC2_Later[16][2];
	KeyOPC1_Before[0] = key_msw;
	KeyOPC1_Before[1] = key_lsw;
    C = bitsel(buf, PC1_Cbits, 28);
    D = bitsel(buf, PC1_Dbits, 28);

	KeyOPC1_Later[0]	= C;
	KeyOPC1_Later[1]	= D;

    for (i = 0; i < 16; i++)
	{
		C = rotl28(C, leftshifts[i]);
		D = rotl28(D, leftshifts[i]);

		KeyOPC2_Before[i][0] = C;
		KeyOPC2_Before[i][1] = D;

		buf[0] = D;
		buf[1] = C;

		KeyOPC2_Later[i][0] = sched->k0246[i] = bitsel(buf, PC2_0246, 32);//bbbb00bbbbbb00bbbbbb00bbbbbb00bb
		KeyOPC2_Later[i][1] = sched->k1357[i] = bitsel(buf, PC2_1357, 32);//00bbbbbb00bbbbbb00bbbbbb00bbbbbb
    }

    sched->iv0 = sched->iv1 = 0;
	if(pK!=NULL)
	{
		memcpy(pK->KeyOPC1_Before,KeyOPC1_Before,sizeof(unsigned int)*2);
		memcpy(pK->KeyOPC1_Later,KeyOPC1_Later,sizeof(unsigned int)*2);
		memcpy(pK->KeyOSiftLeft,leftshifts,16*sizeof(int));
		for(int i=0; i < 16; ++i)
		{
			pK->KeyOPC2_Before[i][0] = KeyOPC2_Before[i][0];
			pK->KeyOPC2_Before[i][1] = KeyOPC2_Before[i][1];
			pK->KeyOPC2_Later[i][0] = KeyOPC2_Later[i][0];
			pK->KeyOPC2_Later[i][1] = KeyOPC2_Later[i][1];
			
		}
	}
}

void des_encipher(unsigned int *output, unsigned int L, unsigned int R,
				  DESContext *sched, DesEno *eno = NULL)
{
	int i = 0;
    unsigned int swap;
	unsigned int IPBeforeLR[2];
	unsigned int IPLaterLR[2];

	IPBeforeLR[0] = L;
	IPBeforeLR[1] = R;
    IP(L, R);
	IPLaterLR[0] = L;
	IPLaterLR[1] = R;

	DesFo fo[17];
	DesEo eo;
	ZeroMemory(fo,sizeof(DesFo)*17);
	for( i = 0; i < 16; ++i )
	{
		eo.ItLR[i][0] = L;
		eo.ItLR[i][1] = R;
		L ^= f(R, sched->k0246[i], sched->k1357[i],&fo[i]);
		L ^= R;
		R ^= L;
		L ^= R;
	}
	eo.ItLR[i][0] = L;
	eo.ItLR[i][1] = R;

    swap = L;
    L = R;
    R = swap;

	unsigned int FpBeforeLR[2];
	unsigned int FPLaterLR[2];

	FpBeforeLR[0] = L;
	FpBeforeLR[1] = R;

    FP(L, R);

	FPLaterLR[0] = L;
	FPLaterLR[1] = R;
	if( eno != NULL )
	{
		memcpy(eno->FPBeforeLR,FpBeforeLR,sizeof(unsigned int)*2);
		memcpy(eno->FPLaterLR, FPLaterLR,sizeof(unsigned int)*2);
		memcpy(eno->IPBeforeLR,IPBeforeLR,sizeof(unsigned int)*2);
		memcpy(eno->IPLaterLR ,IPLaterLR,sizeof(unsigned int)*2);
		memcpy(eno->fo,fo,sizeof(DesFo)*16);
		eno->eo = eo;
	}

    output[0] = L;
    output[1] = R;
}

void des_decipher(unsigned int * output, unsigned int L, unsigned int R,
			 DESContext * sched )
{
    unsigned int swap;

    IP(L, R);

    L ^= f(R, sched->k0246[15], sched->k1357[15]);
    R ^= f(L, sched->k0246[14], sched->k1357[14]);
    L ^= f(R, sched->k0246[13], sched->k1357[13]);
    R ^= f(L, sched->k0246[12], sched->k1357[12]);
    L ^= f(R, sched->k0246[11], sched->k1357[11]);
    R ^= f(L, sched->k0246[10], sched->k1357[10]);
    L ^= f(R, sched->k0246[9], sched->k1357[9]);
    R ^= f(L, sched->k0246[8], sched->k1357[8]);
    L ^= f(R, sched->k0246[7], sched->k1357[7]);
    R ^= f(L, sched->k0246[6], sched->k1357[6]);
    L ^= f(R, sched->k0246[5], sched->k1357[5]);
    R ^= f(L, sched->k0246[4], sched->k1357[4]);
    L ^= f(R, sched->k0246[3], sched->k1357[3]);
    R ^= f(L, sched->k0246[2], sched->k1357[2]);
    L ^= f(R, sched->k0246[1], sched->k1357[1]);
    R ^= f(L, sched->k0246[0], sched->k1357[0]);

    swap = L;
    L = R;
    R = swap;

    FP(L, R);

    output[0] = L;
    output[1] = R;
}

/******************************************************************************/
//	名称：des_encrypt
//	功能：DES加密函数
//  参数：k1:密钥低位; k2:密钥高位; m1:32位明文(左);m2:32位明文(右);c1:密文(左);c2:密文(右)
//	返回：无

/******************************************************************************/
void des_encrypt(unsigned k1,unsigned k2,unsigned m1,unsigned m2,unsigned& c1,unsigned& c2)
{
	unsigned int en_output[2];
	DESContext sched;
	des_key_setup(k1, k2 , &sched);

	des_encipher(en_output, m1 , m2 , &sched);
	c1 = en_output[0];
	c2 = en_output[1];
}

/******************************************************************************/
//	名称：des_decrypt
//	功能：DES解密函数
//  参数：k1:密钥低位; k2:密钥高位; c1:32位密文(左);c2:32密文(右);m1:明文(左);m2:明文(右);
//	返回：无

/******************************************************************************/
void des_decrypt(unsigned k1,unsigned k2,unsigned c1,unsigned c2,unsigned& m1,unsigned& m2)
{
	unsigned int de_output[2];
	DESContext sched ;

	des_key_setup( k1, k2 , &sched );
	des_decipher( de_output, c1, c2,&sched );

	m1 = de_output[0];
	m2 = de_output[1];
}