/*************************** symbol.h *********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct symbolTag
{
  int sym;
  int left;
  int right;
  char value[39];
} symbol;
struct symbolTag *newSymbol(int sym, int left,
                            int right, char *value)
{
  struct symbolTag *p=malloc(sizeof(symbol));
  p->sym = sym;
  p->left = left;
  p->right = right;
  strcpy(p->value, value);
  return p;
}
char *symbolToString(struct symbolTag *p)
{
  static char str[256]="OK";
  sprintf(str,"sym=%d left=%d right=%d value=\"%-s\"",
          p->sym, p->left, p->right, p->value);
  return str;
}
