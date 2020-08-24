#ifndef BIT_BASECODE_H_
#define BIT_BASE_CODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int uint;

uchar BaseCodeTable(uchar inputChar, int codeType);

uchar *Base16(uchar *pInputStr, int lengthStr, int codeType, int codeMode);
uchar *Base32(uchar *pInputStr, int lengthStr, int codeType, int codeMode);
uchar *Base64(uchar *pInputStr, int lengthStr, int codeType, int codeMode);

uchar *BaseEncode(uchar *pInputStr, int lengthStr, int codeType, int baseBit, int diffBit);
uchar *BaseDecode(uchar *pTempArrayStr, int lengthStr, int baseBit, int diffBit);

uchar *RequestStrMemory(int length);

#endif // BIT_BASE_CODE_H_
