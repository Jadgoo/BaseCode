#include "BaseCode.h"

#define MAX_SIZE 1024

void cleanBuffer()
{
   char c = 0;
   while ((c = getchar()) != '\n' && c != EOF);
}

int main()
{
   int lengthStr = 0;
   int codeType = 1;
   int codeMode = 0;
   uchar *pOutputStr = NULL;
   uchar *encodeSym = "Encoding";
   uchar *decodeSym = "Decoding";

   uchar inputStr[MAX_SIZE] = { 0 };          

   while (codeType)
   {
      printf("*** Please select coding type(\"16\" -- > Base16, \"32\" --> Base32, \"64\" --> Base64), enter \"0\" to exit the program ***\n");
      scanf("%d", &codeType);
      cleanBuffer();

      if (!codeType)
      {
         break;
      }
      else if((codeType != 16) && (codeType != 32) && (codeType != 64))
      {
         printf("\n*** Invalid value of coding type, please enter again ***\n");
         continue;
      }

      printf("\n*** Please select coding method(\"0\" --> Encoding, \"1\" --> Decoding) ***\n");
      scanf("%d", &codeMode);
      cleanBuffer();
      
      printf("\n*** Please enter a string ***\n");
      memset(inputStr, 0, sizeof(inputStr));
      scanf("%[^\n]", inputStr);

      lengthStr = (int)strlen(inputStr);

      if (codeType == 16)
      {
         pOutputStr = Base16(inputStr, lengthStr, codeType, codeMode);
      }
      else if (codeType == 32)
      {
         pOutputStr = Base32(inputStr, lengthStr, codeType, codeMode);
      }
      else if(codeType == 64)
      {
         pOutputStr = Base64(inputStr, lengthStr, codeType, codeMode);
      }

      printf("\n*** After Base%d %s *** \n%s\n\n", codeType, codeMode ? decodeSym : encodeSym, pOutputStr);

      if (pOutputStr)
      {
         free(pOutputStr);
         pOutputStr = NULL;
      }
   }

   printf("\n*** Exit ***\n");
   system("pause");
   return 0;
}
