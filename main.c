#include <stdio.h>
#include "matrix.h"

int main()
{
  char cmd[20];
  char chIndex[16];
  int index, i, continu;
  matrix *head, *ptr;
  int save;

  //vars for multMatrix() and addMatrix()
  int intId1, intId2, id;
  char id1[5], id2[5];

  head = NULL;
  while(strcmp(cmd, "exit") != 0)
  {
    printf("\nEnter command ('help' for cmd list): ");
    scanf("%s", cmd);

    //create matrix node
    if(strcmp(cmd, "new") == 0)
    {
      ptr = createMatrix();
      ptr->next = head;
      head = ptr;
      printM(*head);
    }

    //RREF!
    else if (strncmp(cmd, "rref", 4) == 0)
    {
      i = 0;
      do {
        chIndex[0] = cmd[i + 4];
        if(!isdigit(cmd[i + 4]))
        {
          printf("\nNon-numeric id. RREF TERMINATED@!!!!!");
          continu = 1;
          break;
        }
        i++;
      } while(cmd[i + 4] != '\0');
      if (continu == 1)
        continue;

      index = atoi(chIndex);
      ptr = findM(head, index);
      if(ptr == NULL)
      {
        printf("\nMatrix of id %d does not exist.", index);
      }
      rref(ptr);
    }
    else if (strcmp(cmd, "help") == 0)
    {
        printf("\nCOMMAND LIST\n*****\n'new' - create new matrix\n'rrefx' - reduce and print matrix of id = x into RREF form\n'printx' - print matrix of id = x\n");
        printf("\n'deletex' - delete matrix of id = x");
        printf("'multx,y' - multiply matrix of id = x with matrix of id = y\n'addx,y' - add matrix of id = x to matrix of id = y\n'all' - print all matrices\n'exit' - go figure u idiot!!!\n*****");
    }

    //PRINT A MATRIX!
    else if (strncmp(cmd, "print", 5) == 0)
    {
      i = 0;
      do {
        chIndex[0] = cmd[i + 5];
        if(!isdigit(cmd[i + 5]))
        {
          printf("\nNon-numeric id. RREF TERMINATED@!!!!!");
          continu = 1;
          break;
        }
        i++;
      } while(cmd[i + 5] != '\0');
      if (continu == 1)
        continue;

      index = atoi(chIndex);
      ptr = findM(head, index);
      if(ptr == NULL)
        printf("\nMatrix of id %d DNE.", index);
      else
        printM(*ptr);
    }

    //LIST ALL MATRICES
    else if (strcmp(cmd, "all") == 0)
    {
      printf("\n******ALL MATRICES******");
      ptr = head;
      while(ptr!= NULL)
      {
        printM(*ptr);
        ptr = ptr->next;
        printf("\n");
      }
      printf("\n******End of list.******");
    }


    //MULTIPLY MATRICES
    else if(strncmp(cmd, "mult", 4) == 0)
    {
      i = 0;
      int j = 0;

      //input checking
      while(cmd[i + 4] != '\0' && cmd[i + 4]!= ',')
      {
        if(cmd[i + 4] == ' ')
        {
          printf("\nEnter number argument...");
        }


        id1[i] = cmd[i + 4];
        i++;
      }
      id1[i] = '\0';

      while(cmd[i + 4] != '\0')
      {
        i++;
        id2[j] = cmd[i + 4];
        j++;
      }
      id2[j] = '\0';
      if(isNum(id1, strlen(id1)) == 0 || isNum(id2, strlen(id2)) == 0)
      {
        printf("\nEnter number argument...");
        continue;
      }

      sscanf(id1, "%d", &intId1);
      sscanf(id2, "%d", &intId2);

      ptr = multMatrix(findM(head, intId1), findM(head, intId2));
      if (ptr != NULL)
        printM(*ptr);
      else
        continue;

      while(save != 0 && save != 1)
      {
        printf("\nSave this matrix? (0 or 1): ");
        scanf("%d", &save);
        if(save == 1)
        {
          printf("\nID: ");
          scanf("%d", &id);
          ptr->id = id;
          ptr->next = head;
          head = ptr;
        }
        else if (save == 0)
        {
          freeMatrix(ptr);
        }
      }
    }

    // ADD MATRICES
    else if(strncmp(cmd, "add", 3) == 0)
    {
      i = 0;
      int j = 0;

      //input checking
      while(cmd[i + 3] != '\0' && cmd[i + 3]!= ',')
      {
        if(cmd[i + 3] == ' ')
        {
          printf("\nEnter number argument...");
        }

        id1[i] = cmd[i + 3];
        printf("\n%c", id1[i]);
        i++;
      }
      id1[i] = '\0';
      printf("\nid1 = %s", id1);

      while(cmd[i + 3] != '\0')
      {
        i++;
        id2[j] = cmd[i + 3];
        j++;
      }
      id2[j] = '\0';
      printf("\nid2 = %s", id2);

      if(isNum(id1, strlen(id1)) == 0 || isNum(id2, strlen(id2)) == 0)
      {
        printf("\nEnter number argument...|%s| |%s|", id1, id2);
        continue;
      }

      sscanf(id1, "%d", &intId1);
      sscanf(id2, "%d", &intId2);

//      printM(*head);
      ptr = addMatrix(findM(head, intId1), findM(head, intId2));
      if (ptr != NULL)
      {
        printM(*ptr);
        printf("\nSave this matrix? (0 or 1): ");
        scanf("%d", &save);

        if(save == 1)
        {
          printf("\nID: ");
          scanf("%d", &id);
          ptr->id = id;
          ptr->next = head;
          head = ptr;
        }
        else if (save == 0)
        {
          freeMatrix(ptr);
        }

      }
      else
        continue;

    }
    //DELETE MATRIX
    else if (strncmp(cmd, "delete", 6) == 0)
    {
      int deleteId;
      int lastNodeId;
      matrix *toDelete;
      sscanf(cmd, "delete%d", &deleteId);
      printf("\nSearching for matrix of id %d ... ", deleteId);
      ptr = head;

      while(ptr != NULL)
      {
        if(ptr->id == deleteId)
        {
          printf("\nDetected ID: %d", ptr->id);
          break;
        }
        lastNodeId = ptr->id;
        ptr = ptr->next;
      }
      toDelete = ptr;
      if(ptr == head)//you want to delete first node
      {
        head = head->next;
      }
      else if (ptr->next == NULL)//delete final node
      {
        ptr = findM(head, lastNodeId);
        ptr->next = NULL;
      }
      else//delete middle node
      {
        printf("\nBefore and after:::::::::::::");
        printM(*findM(head, lastNodeId));
        printM(*(ptr->next));
        ptr = findM(head, lastNodeId);
        ptr->next = ptr->next->next;
      }

      //free MATRIX
      for(i = 0; i < toDelete->m; i++)
      {
        free(toDelete->matrix[i]);
      }
      printf("\nMatrix deleted.");
    }
    else if (strcmp(cmd, "exit") != 0)
      printf("\nUnrecognized command.");

  }
  return 0;
}
