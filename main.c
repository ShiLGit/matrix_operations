#include <stdio.h>
#include "matrix.h"
int main()
{
  char str[50];
  char *print, *nLine;
  char **ops;
  int sIndex;
  fgets(str, 50, stdin);

  str[strlen(str) -1];
  print = str;
  //printf("\nadd[i] = %p", print);
  nLine= strchr(print, '\n');
  if(nLine != NULL)
    *nLine = '\0';


  ops = cmdList(print);
//  printf("\nparsed: %s", parse(print, &sIndex));
  //printf("\nadd[f] = %p", print);
   return 0;
}
