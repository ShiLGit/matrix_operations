#ifndef MATRIXH
#define MATRIXH

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
struct maytrix{
  float** matrix;
  int m, n;
  char var;
  int aug;
  struct maytrix *next;
};
typedef struct maytrix matrix;

//parsing func prototypes
char* whiteSpace(char*);
char**cmdList (char*);
char* parse(char*, int*);
char* replace(char*, char*, int);
int alphabetical(char);
/*****************************************PARSING FUNCTIONS!!!!!!!**********************************************/
/*
deletes all whitespace (SPACES)
*/
char* whiteSpace(char* string)
{
  char *str = string;
  int prev = 1;
  int i = 0;
  int j, shift;

  while(str[i] != '\0')
  {
    //printf("\n%c %d", str[i], prev);
    if (str[i] == ' ')
    {
      //shift whole string to close consecutive space gaps
      j = i;
      shift = 0;
      while(str[j] == ' ')
      {
        shift++;
        j++;
      }

      j = i;
      while(str[j - 1] != '\0')
      {
        str[j] = str[j + shift];
        j++;
      }
    }
    else
      prev = 0;

    i++;
  }

  //trim end of string if its last char is '|'
  if(str[i - 2] == ' ')
  {
    str[i -2] ='\0';
  }
  if(str[i - 1] == '\n')
    str[i - 1] = '\0'; //delete '\n' appended to end of string from fgets()

  return str;
}
/*
Returns 2D array of strings: list of operations to perform
-last operation has '#' appended to end to signal end of list (since returns ptr of indeterminate size)
-DOES NOT check if variables are valid
-must neglect spaces
*/
char** cmdList(char *string)
{
  int numCmds = 0, rIndex, dummy;
  char** r = malloc(sizeof(char*));
  char* substring = malloc(sizeof(char) * (strlen(string) + 1));
  strcpy(substring, string);
  do
  {
      numCmds++;
      substring = parse(substring, &rIndex);

    if(substring == NULL)
      return NULL;

    if(strcmp(substring, parse(substring, &dummy)) == 0)//use a dummy int to allow for parse() calling while not altering the value stored @ rIndex
    {
      r = realloc(r, sizeof(char*) * (numCmds));
      r[numCmds - 1] = substring;

      printf("\ncalling replace(%s, %s, %d)", string, substring, rIndex);
      substring = replace(string, substring, rIndex);
      printf("\ncmd%d = '%s'", numCmds, r[numCmds-1]);
    }
  }while(strcmp(r[numCmds - 1], "$") != 0);

  return r;
}

/*
Given expression, returns first term to be evaluated; errorchecking included
+ alters startIndex byRef >> index of where the first expn starts in the original string;
*/
char* parse(char* string, int* startIndex)
{
  string = whiteSpace(string);
  char* r, *reString, *returnR;
  char replace[4];
  int flags[3] = {0, 0, 0}; //flag[0] == 1 if brackets, [1] == 1 if mult, [2] == 1 if add
  int i = 0;
  int copyBounds[2];
  int j, k;

  int freezeLeft =0;
  int freezeRight = 0;
  int numSteps = 0;
  int multIndex, addIndex;

  while(string[i] != '\0')
  {
    if(!alphabetical(string[i]) && !isdigit(string[i]) && !(string[i] == '*' || string[i] == '+' || string[i] == '-' || string[i] == '(' || string[i] == ')' || string[i] == '$'))
    {
      printf("\nERROR: Expression contains illegal character(s) '%c' at index = %d.", string[i], i);
      return NULL;
    }
    i++;
  }
  i = 0;

  //Preliminary errorcheck: illegal characters?

  //Do brackets, mult, addition exist?
  while(string[i] != '\0')
  {
    if(string[i] == '(')
    {
      flags[0] = 1;
      break;
    }
    else if(string[i] == '*')
    {
      if(flags[1] == 0)//NOT REEALLY necessary, but allows for first occurence of '*' to get returned first: better debugging nd follows precence more "traditionally"
        multIndex = i;

      flags[1] = 1;
    }
    else if(string[i] == '+' || string[i] == '-')
    {
      if(flags[2] == 0)
        addIndex = i;//same logic as last comment!!!!!!!!!!!!!!!!!!!!

      flags[2] = 1;
    }
    i++;
  }

  if(flags[0] == 1)//1) Bracket parsing: highest priority
  {
    int openBracks = 1;
    int closeBracks = 0;
    i++;//you will start counting brackets on a '(' char so openB count artificially gets increased by 1 if index is unaltered
    copyBounds[0] = i;

    //handle entries like "A*($)" >> "return A*$" rather than "$"
    if(string[i + 1] == ')')
    {
      //remove brackets
      j =k = 0;
      do
      {
        printf("\ns = %c", string[j]);
        if(string[j] != '(' && string[j] != ')')
        {
            replace[k] = string[j];
            printf(", r[%d] = %c",k, replace[k]);
            k++;
        }
        j++;
      }while(string[j] != '\0');
      replace[k] = '\0';

      reString = malloc(sizeof(char) * (strlen(replace) + 1 ));
      strcpy(reString, replace);
//      return reString;
      r = parse(reString, &j);
      free(reString);

      return r;
    }

    do
    {
      if(string[i] == ')')
        closeBracks++;
      else if (string[i] == '(')
        openBracks++;
      else if(string[i] == '\0')
      {
        printf("\nERROR: Check your syntax (hint: brackets are off somewhere).");
        return NULL;
      }

      i++;
    }while(openBracks != closeBracks && openBracks != 0);
    copyBounds[1] = i - 2;
    r = malloc(sizeof(char) * (copyBounds[1] - copyBounds[0] + 2));
  }

  else if(flags[1] == 1)//2)Multiplication parsing
  {
    i = multIndex;
    j = i - 1;
    k = i + 1;

    if(j < 0)
    {
      printf("\nERROR: Expression starts with '*'.");
      return NULL;
    }


    do
    {
//      printf("\n%c, %d", string[k], freezeRight);
      if( string[j] == '+'|| string[j] == '*'|| string[j] == '-' || j == 0)
        {
          freezeLeft = 1 ;
          if(j< i - 1)
          {
            j++;
          }

        }
      if ( string[k] == '*' || string[k] == '+'|| string[k] == '-'|| string[k] == '\0')
        {
          freezeRight = 1;
    //      printf("\nR frozen at '%c'", string[k]);
        }
      if ( (numSteps > 0 && alphabetical(string[j]) && !freezeLeft) || (numSteps > 0 &&alphabetical(string[k]) && !freezeRight))
      {
        printf("\nERROR: all multiplication must be denoted by '*'. (e.g. 'a*b' rather than 'ab').");
        return NULL;
      }

      if(!freezeLeft)
        j--;
      if(!freezeRight)
        k++;

      numSteps++;
    }while(!freezeLeft || !freezeRight);

    if(string[k + 1] == '\0' && string[k] == '*')
    {
      printf("\nERROR: Expression ends in '*'.");
      return NULL;
    }
    copyBounds[0] = j;
    copyBounds[1] = k - 1;

    r = malloc(sizeof(char) * (copyBounds[1] - copyBounds[0]) + 2);
  }
  //Addition parsing
  else if(flags[2] == 1)
  {
    if(string[0] == '+')
    {
      printf("\nERROR: Expression starts with '+'.");
      return NULL;
    }

    j = addIndex -1;
    k = addIndex + 1;
    do
    {
      if(string[j] == '+' || string[j] == '-' || string[j] == '*' || j == 0)
      {

        if(j < addIndex - 1 && !freezeLeft)
          j++;

        freezeLeft = 1;
      }
      else if (numSteps > 0 && !isdigit(string[j]) )
      {
        printf("\nERROR: All multiplication must be denoted with '*' (e.g. 'a*b' instead of 'ab').");
        return NULL;
      }

      if(string[k] == '+' || string[k] == '-' || string[k] == '*' || string[k] == '\0')
      {
        if(k > addIndex + 1 && !freezeRight)
          k--;
        freezeRight = 1;
      }
      else if (numSteps > 0 && !isdigit(string[k]) &&string[k] !='\0' )
      {
        printf("\nERROR: All multiplication must be denoted with '*' (e.g. 'a*b' instead of 'ab').");
        return NULL;
      }

      numSteps++;
      if(!freezeLeft)
        j--;
      if(!freezeRight)
        k++;

    }while(!freezeLeft || !freezeRight);
    if(string[k] == '+' || string[k] == '-')
    {
      printf("\nERROR: Expression ends in '+' or '-'.");
    }
    copyBounds[0] = j;
    copyBounds[1] = k;

    r = malloc(sizeof(char) * (copyBounds[1] - copyBounds[0] + 2));
  }
  else
  {
    returnR = malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(returnR, string);
    return returnR;
  }

  j = 0;
  for(i = copyBounds[0]; i <= copyBounds[1]; i++, j++)
    r[j] = string[i];

  *startIndex = copyBounds[0];

  returnR = malloc(sizeof(char) * (strlen(r) + 1));
  strcpy(returnR, r);
  return returnR;
}

char* replace(char* big, char *small, int start)
{
  int shift = strlen(small) -1, i= start, j = 1, dummy;
  big[i] = '$';

  for(i = start + 1; big[i] != '\0'; i++)
  {
    big[i] = big[i + shift];
    j++;
  }
  return big;
}

int alphabetical(char c)
{
  if(c >= 'a' && c <= 'z')
    return 1;
  else if (c >= 'A' && c <= 'Z')
    return 1;
  else
    return 0;
}
/***************************************************************************************************************/

#endif
