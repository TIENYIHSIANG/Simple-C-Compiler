/********************** resword.h ******************/
#include <stdlib.h>
#define RESWORDMAX 8
char reswords[RESWORDMAX][10] =
  {
  "int","if","else","while","scanf","printf","return","include" 
  };
int isResword(char *s)
{
  int i;
  for (i=0; i<RESWORDMAX; i++)
  {
    if (strcmp(s, reswords[i])==0) return i;
  }
  return -1;
}
