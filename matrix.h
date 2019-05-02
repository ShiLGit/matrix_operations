#ifndef MATRIXH
#define MATRIXH

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct maytrix{
  float** matrix;
  int m, n;
  int id;
  int aug;
  struct maytrix *next;
};
typedef struct maytrix matrix;

//CHECK STRING FOR NONNUMERIC CHARS
int isNum(char *str, int len)
{
  int i = 0;
  for(i = 0; i < len; i++)
  {
    if(!isdigit(str[i]))
      return 0;
  }
  return 1;
}

//CREATE, FILL MATRIX
matrix *createMatrix()
{
  /*to do: errircheck input for matrix (line of numbers)*/
  matrix *rMat = malloc(sizeof(matrix));
  int r, c;
  char inpt[10];

  do
  {
    printf("\nEnter ID#: ");
    scanf("%s", inpt);
  }while (isNum(inpt, strlen(inpt)) == 0);
  sscanf(inpt, "%d", &rMat->id);

  printf("\nNOTE: augmented matrices will count last column as the augment rather than part of the coeff. matrix.\n");
  do {
    printf("Augmented (1 or 0): ");
    scanf("%s", inpt);
    sscanf(inpt, "%d", &rMat->aug);
  } while(rMat->aug != 0 && rMat->aug != 1);

  do
  {
    printf("m = ");
    scanf("%s", inpt);
  }while(isNum(inpt,strlen(inpt)) == 0);
  sscanf(inpt, "%d", &rMat->m);

  do {
    printf("n = ");
    scanf("%s", inpt);
  } while(isNum(inpt, strlen(inpt)) == 0);
  sscanf(inpt, "%d", &rMat->n);

  //allocate memory for matrix member
  rMat->matrix = malloc(sizeof(float*)*rMat->m);
  for(r = 0; r < rMat->m; r++)
    rMat->matrix[r] = malloc(sizeof(float)*rMat->n);

  //fill matrix
  for(r = 0; r < rMat->m; r++)
  {
    printf("Enter row %d: ", r + 1);
    for(c = 0; c < rMat->n; c++)
    {
      scanf("%f", &rMat->matrix[r][c]);
    }
  }
  return rMat;
}
//FIND MATRIX GIVEN ID
matrix *findM(matrix* head, int idF)
{
  matrix *dummy = head;

  while(dummy->id != idF)
  {
    dummy = dummy->next;
    if(dummy == NULL)
      return NULL;
  }
  return dummy;
}
//PRINT MATRIX
void printM(matrix p)
{
  int i, j;
  int m = p.m;
  int n = p.n;
  printf("\nMatrix ID = %d:\n", p.id);
  for(i = 0; i < m; i++)
  {
    printf("\n");
    for(j = 0; j < n; j++)
    {
      if(p.aug == 1 && j == p.n - 1)
        printf("|");

      if(p.matrix[i][j] == 0)//avoid printing -0
        printf(" %3.2f", 0.00);
      else
        printf(" %3.2f", p.matrix[i][j]);
    }
  }
}

//SUBTRACT ROW FROM ALL OTHER ROWS
void rowSubtract(matrix *mat, int rowIndex)
{
  int r, c;
  float pivot = mat->matrix[rowIndex][rowIndex];
  float pivotMult;

  //reduce pivot to = 1
  if(pivot == 0)
  {
    return;
  }
  for(c = 0; c < mat->n; c++)
    mat->matrix[rowIndex][c] = mat->matrix[rowIndex][c]/pivot;

  pivot = 1;

  //subtract pivot row from all entries below
  for(r = rowIndex; r < mat->m - 1; r++)
  {
    pivotMult = mat->matrix[r + 1][rowIndex];
    printf("\nr = %d; pivotmult = %.2f", r, pivotMult);
    for(c = 0; c< mat->n; c++)
    {
      mat->matrix[r + 1][c] = mat->matrix[r + 1][c] - mat->matrix[rowIndex][c]*pivotMult;
    }
  }

  //subtract pivot row from all entries above
  if(rowIndex > 0)
  {
    for(r = rowIndex; r > 0; r--)
    {
      pivotMult = mat->matrix[r-1][rowIndex];
      for(c = 0; c < mat->n; c++)
      {
        mat->matrix[r - 1][c] = mat->matrix[r - 1][c] - mat->matrix[rowIndex][c]*pivotMult;
      }
    }
  }

  printM(*mat);
}
//CHANGES MATRIX (BYREF) TO RREF FORM
void rref(matrix *mat)
{
  //todo: infinite solutions? singular matrix?
  int i;
  int r = 0;
  int c = mat->n;
  float *temp;

  if (r>c)
    printf("\nNOTE: RREF may not exist. (m > n)\n");

  for(i = 0; i < mat->m; i++)
  {
    //make sure pivot is not 0; swap with immediate next row if == 0
    if(mat->matrix[i][i] == 0)
    {
      r = 1;
      do
      {
        if((i + r < mat->m) == 0)
          break;

        temp = mat->matrix[i + r];
        mat->matrix[i + r] = mat->matrix[i];
        mat->matrix[i] = temp;

        r++;
      }while ((mat->matrix[i][i] == 0));
    }
    rowSubtract(mat, i);
  }

  return;
}

matrix *multMatrix(matrix *m1, matrix *m2)
{
  //errorcheck
  if(m1 == NULL || m2 == NULL)
  {
    printf("\nOne or more of the specified matrices don't exist.");
    return NULL;
  }
  else if(m1->n != m2->m)
  {
      printf("\nInvalid matrix sizes. (m1.n = %d; m2.m = %d)", m1->n, m2->m);
      return NULL;
  }

  //actual matrix mult in case of valid input
  int r, c;
  int i;
  matrix *ret = malloc(sizeof(matrix));
  float term;

  ret->m = m1->m;
  ret->n = m2->n;
  ret->matrix = malloc(sizeof(float*) * ret->m);
  for(r = 0; r < ret->m; r++)
    ret->matrix[r] = malloc(sizeof(float) * ret->n);

  //acsual multiplication
  for(r = 0; r < ret->m; r++)
  {
    for(c = 0; c< ret->n; c++)
    {
      term = 0;
      for(i = 0; i < m1->n; i++)
      {
        term += m1->matrix[r][i] * m2->matrix[i][c];
      }
      ret->matrix[r][c] = term;
    }
  }

  return ret;
}

matrix *addMatrix(matrix *m1, matrix *m2)
{

  if(m1 == NULL || m2 == NULL)
  {
    printf("\nOne or more of the specified matrices don't exist.");
    return NULL;
  }
  else if(m1->n != m2->n || m1->m != m2->m)
  {
    printf("\nInvalid matrix size. (Must be same size: %dx%d and %dx%d...)", m1->m, m1->n, m2->m, m2->n);
    return NULL;
  }

  int r, c;
  matrix *ret = malloc(sizeof(matrix));
  ret->matrix = malloc(sizeof(float*) * m1->m);

  for(r = 0; r < m1->m; r++)
    ret->matrix[r] = malloc(sizeof(float) * m1->n);
  for(r = 0; r < m1->m; r++)
  {
    for(c = 0; c < m1->n; c++)
    {
      ret->matrix[r][c] = m1->matrix[r][c] + m2->matrix[r][c];
    }
  }

  printM(*ret);
  return ret;
}

//CHECK HEAD?????????
void freeMatrix(matrix *m)
{
  int i;

  for(i = 0; i < m->m; i++)
  {
    free(m->matrix[i]);
  }
  free(m->next);

  return;
}
#endif
