/********************* lexer.c *******************/
#include "stdio.h"
#include <stdlib.h>
#include "scanner.h"
struct symbolTag *token;
int main(int argc, char *argv[])
{
  FILE *f=fopen(argv[1], "r");
  scanner(f);
  while ((token=nextToken())!=NULL)
  {
    if (token->sym==symerror || argc==3)
    {
      if (token->sym==symerror)
        printf("***錯誤***");

    }
          printf("\t符記編號=%d\t列號=%d\t行號=%d"
        "\t符記名稱=\"%s\"\n", token->sym,
        token->left, token->right, token->value);
  }
  fclose(f);
  printf("\n  語彙分析完成。");
  return 0;
}
