#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "cpu.h"
struct node *head = NULL; // empty header
int nextTid = 0;

Task *picknexttask()
{

    return head->task;
}

void add(char *name, int priority, int burst)
{ 
    // add task to list
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = nextTid++;
    newTask->priority = priority;
    newTask->burst = burst;
    insert_priority(&head, newTask);
    printf("Added = [%s] [%d] [%d] for %d units.\n",head->task->name, head->task->priority, head->task->burst, 12);
}

void schedule() // required by problem definition
{
    Task *current;
    traverse(head); // check list exists
    while (head != NULL)
    {
        current = picknexttask();
        run(current, current->burst);
        delete (&head, current);
    }
}
