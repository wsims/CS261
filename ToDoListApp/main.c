#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "toDoList.h"


int main (int argc, const char * argv[])
{
    char cmd = ' ';
    DynArr* mainList = createDynArr(10);
    FILE* fileptr;
    char desc[100], fname[100];
    int priority;
    TaskP t;

  printf("\n\n** TO-DO LIST APPLICATION **\n\n");

  do
    {
      printf("Press:\n"
             "'l' to load to-do list from a file\n"
             "'s' to save to-do list to a file\n"
             "'a' to add a new task\n"
             "'g' to get the first task\n"
             "'r' to remove the first task\n"
             "'p' to print the list\n"
             "'e' to exit the program\n"
             );
      /* get input command (from the keyboard) */
      cmd = getchar();
      /* clear the trailing newline character */
      while (getchar() != '\n');
        if (cmd == 'l') {
            printf("Please enter the filename: ");
            scanf("%[^\n]%*c", fname);
            fileptr = fopen(fname, "r");
            if (fileptr != NULL) {
                loadList(mainList, fileptr);
                fclose(fileptr);
                printf("The list has been loaded from file successfully.\n\n");
            } else printf("Error opening file.\n");
        } else if (cmd == 's') {
            printf("Please enter the filename: ");
            scanf("%[^\n]%*c", fname);
            fileptr = fopen(fname, "w");
            if (fileptr != NULL) {
                saveList(mainList, fileptr);
                fclose(fileptr);
                printf("The list has been saved into the file successfully.\n\n");
            } else printf("Error opening file.\n");
        } else if (cmd == 'a') {
            printf("Please enter the task description: ");
            scanf("%[^\n]%*c", desc);
            printf("Please enter the tast priority (0-999): ");
            scanf("%d%*c",&priority);
            t = createTask(priority, desc);
            addHeap(mainList, t, compare);
            printf("The task '%s' has been added to your to-do list. \n\n", desc);
        } else if (cmd == 'g') {
            if (sizeDynArr(mainList) <= 0)
                printf("Your to-do list is empty!\n");
            else {
                t = getMinHeap(mainList);
                printf("Your first task is: %s\n\n", t->description);
            }
        } else if (cmd == 'r') {
            if (sizeDynArr(mainList) <= 0)
                printf("Your to-do list is empty!\n");
            else {
                t = getMinHeap(mainList);
                printf("Your first task is '%s' has been removed from the list\n\n", t->description);
                removeMinHeap(mainList, compare);
            }
        } else if (cmd == 'p') {
            if (sizeDynArr(mainList) <= 0)
                printf("Your to-do list is empty!\n");
            else
                printList(mainList);
        }
      /* Note: We have provided functions called printList(), saveList() and loadList() for you
         to use.  They can be found in toDoList.c */
    }
  while(cmd != 'e');
  /* delete the list */
    printf("Bye!\n");
    deleteDynArr(mainList);

  return 0;
}
