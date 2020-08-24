#include "BaseCode.h"

#define BASE16_BIT 4
#define BASE32_BIT 5
#define BASE64_BIT 6
#define BYTE_BIT 8
#define SUM_NUMBER 10
#define SUM_LETTER 26

uchar BaseCodeTable(uchar inputChar, int codeType)
{
   uchar *charSet = NULL;
   if (codeType == 16)
   {
      charSet = "0123456789ABCDEF";
   }
   else if (codeType == 32)
   {
      charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
   }
   else if (codeType == 64)
   {
      charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   }

   return charSet[inputChar];
}

uchar *Base16(uchar *pInputStr, int lengthStr, int codeType, int codeMode)
{
   int i = 0;
   uchar *pCodeStr = NULL;
   uchar *pTempArrayStr = NULL;

   if (!codeMode)
   {
      pCodeStr = BaseEncode(pInputStr, lengthStr, codeType, BASE16_BIT, BYTE_BIT - BASE16_BIT);
      return pCodeStr;
   }

   pTempArrayStr = RequestStrMemory(lengthStr);
   if (!pTempArrayStr)
   {
      return NULL;
   }

   for (i = 0; i < lengthStr; ++i)
   {
      if (pInputStr[i] >= '0' && pInputStr[i] <= '9')
      {
         pTempArrayStr[i] = pInputStr[i] - '0';
      }
      else
      {
         pTempArrayStr[i] = pInputStr[i] - 'A' + SUM_NUMBER;
      }
   }

   pCodeStr = BaseDecode(pTempArrayStr, lengthStr, BASE16_BIT, BYTE_BIT - BASE16_BIT);
   return pCodeStr;
}

uchar *Base32(uchar *pInputStr, int lengthStr, int codeType, int codeMode)
{
   int i = 0;
   uchar *pCodeStr = NULL;
   uchar *pTempArrayStr = NULL;

   if (!codeMode)
   {
      pCodeStr = BaseEncode(pInputStr, lengthStr, codeType, BASE32_BIT, BYTE_BIT - BASE32_BIT);
      return pCodeStr;
   }

   pTempArrayStr = RequestStrMemory(lengthStr);
   if (!pTempArrayStr)
   {
      return NULL;
   }

   for (i = 0; i < lengthStr; ++i)
   {
      if (pInputStr[i] >= 'A' && pInputStr[i] <= 'Z')
      {
         pTempArrayStr[i] = pInputStr[i] - 'A';
      }
      else
      {
         pTempArrayStr[i] = pInputStr[i] - '2' + SUM_LETTER;
      }
   }

   pCodeStr = BaseDecode(pTempArrayStr, lengthStr, BASE32_BIT, BYTE_BIT - BASE32_BIT);
   return pCodeStr;
}

uchar *Base64(uchar *pInputStr, int lengthStr, int codeType, int codeMode)
{
   int i = 0;
   uchar *pCodeStr = NULL;
   uchar *pTempArrayStr = NULL;

   if (!codeMode)
   {
      pCodeStr = BaseEncode(pInputStr, lengthStr, codeType, BASE64_BIT, BYTE_BIT - BASE64_BIT);
      return pCodeStr;
   }

   pTempArrayStr = RequestStrMemory(lengthStr);
   if (!pTempArrayStr)
   {
      return NULL;
   }

   for (i = 0; i < lengthStr; ++i)
   {
      if (pInputStr[i] >= 'A' && pInputStr[i] <= 'Z')
      {
         pTempArrayStr[i] = pInputStr[i] - 'A';
      }
      else if (pInputStr[i] >= 'a' && pInputStr[i] <= 'z')
      {
         pTempArrayStr[i] = pInputStr[i] - 'a' + SUM_LETTER;
      }
      else if (pInputStr[i] >= '0' && pInputStr[i] <= '9')
      {
         pTempArrayStr[i] = pInputStr[i] - '0' + 2 * SUM_LETTER;
      }
      else if (pInputStr[i] == '+')
      {
         pTempArrayStr[i] = pInputStr[i] - '+' + 2 * SUM_LETTER + SUM_NUMBER;
      }
      else
      {
         pTempArrayStr[i] = 2 * SUM_LETTER + SUM_NUMBER + 1;
      }
   }

   pCodeStr = BaseDecode(pTempArrayStr, lengthStr, BASE64_BIT, BYTE_BIT - BASE64_BIT);
   return pCodeStr;
}

uchar *BaseEncode(uchar *pInputStr, int lengthStr, int codeType, int baseBit, int diffBit)
{
   int i = 0, j = 0, n = 0;
   int sumBit = 0;
   int restBit = 0;
   int encodeStrSize = 0;
   uchar tempChar = 0;
   uchar *pEncodeStr = NULL;

   sumBit = lengthStr * BYTE_BIT;
   encodeStrSize = (sumBit + (baseBit - 1)) / baseBit + 1;

   pEncodeStr = RequestStrMemory(encodeStrSize);
   if (!pEncodeStr)
   {
      return NULL;
   }

   n = sumBit - sumBit % baseBit;
   restBit = BYTE_BIT;

   for (n; n > 0; n -= baseBit)
   {
      if (!restBit)
      {
         restBit = BYTE_BIT;
         i++;
      }

      if (restBit < baseBit)
      {
         tempChar = pInputStr[i++] << (BYTE_BIT - restBit);
         pEncodeStr[j++] = BaseCodeTable(((tempChar >> diffBit) | (pInputStr[i] >> (restBit + diffBit))), codeType);
         restBit += diffBit;
      }
      else
      {
         tempChar = pInputStr[i] << (BYTE_BIT - restBit);
         pEncodeStr[j++] = BaseCodeTable((tempChar >> diffBit), codeType);
         restBit -= baseBit;
      }
   }

   if (sumBit % baseBit)
   {
      tempChar = pInputStr[i] << (BYTE_BIT - restBit);
      pEncodeStr[j] = BaseCodeTable((tempChar >> diffBit), codeType);
   }

   return pEncodeStr;
}

uchar *BaseDecode(uchar *pTempArrayStr, int lengthStr, int baseBit, int diffBit)
{
   int i = 0, j = 0, n = 0;
   int restBit = 0;
   int sumBit = 0;
   int encodeStrSize = 0;
   uchar tempCharA = 0;
   uchar tempCharB = 0;
   uchar tempCharC = 0;
   uchar *pDecodeStr = NULL;

   sumBit = lengthStr * baseBit;
   encodeStrSize = (sumBit / BYTE_BIT + 1);

   pDecodeStr = RequestStrMemory(encodeStrSize);
   if (!pDecodeStr)
   {
      return NULL;
   }

   n = sumBit - sumBit % BYTE_BIT;
   restBit = baseBit;

   for (n; n > 0; n -= BYTE_BIT)
   {
      if (!restBit)
      {
         restBit = baseBit;
         i++;
      }

      if (restBit < diffBit)
      {

         tempCharA = pTempArrayStr[i++] << (BYTE_BIT - restBit);
         tempCharB = pTempArrayStr[i++] << (diffBit - restBit);
         tempCharC = pTempArrayStr[i] >> (restBit + baseBit - diffBit);

         pDecodeStr[j++] = tempCharA | tempCharB | tempCharC;
         restBit = restBit + baseBit - diffBit;
      }
      else
      {
         tempCharA = pTempArrayStr[i++] << (BYTE_BIT - restBit);
         tempCharB = pTempArrayStr[i] >> (restBit - diffBit);

         pDecodeStr[j++] = tempCharA | tempCharB;
         restBit = restBit - diffBit;
      }
   }

   if (pTempArrayStr)
   {
      free(pTempArrayStr);
      pTempArrayStr = NULL;
   }

   return pDecodeStr;
}

uchar *RequestStrMemory(int length)
{
   uchar *pTempArrayStr = NULL;

   pTempArrayStr = (uchar *)malloc(length * sizeof(uchar));
   if (!pTempArrayStr)
   {
      printf("Failed to request temp array in heap memory!\n");
      return NULL;
   }
   memset(pTempArrayStr, 0, length * sizeof(uchar));

   return pTempArrayStr;
}
