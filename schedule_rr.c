#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "cpu.h"

#define TIMESLICE 2

struct node *head = NULL; // empty header
int nextTid = 0;
int total_burst_time = 0;

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
    total_burst_time += burst;
    insert(&head, newTask);
}

void schedule() // required by problem definition
{
    Task *current;
    traverse(head); // check list exists
    struct node *temp = head;

    // while (temp != NULL)
    // {
    //     current = temp->task;
    //     printf("%s\n", current->name);
    //     temp = temp->next;
    // }
    // temp->next = head;
    // head = temp;

    while (head != NULL)
    {
        if (total_burst_time == 0)
        {
            printf("No tasks remaining\n");
            break;
        }
        current = picknexttask();
        if (current->burst < TIMESLICE)
        {
            total_burst_time -= current->burst;
            run(current, current->burst);
            delete (&head, current);
        }
        else
        {
            total_burst_time -= TIMESLICE;
            current->burst -= TIMESLICE;
            run(current, TIMESLICE);
        }
        if (head->next == NULL) {
            head->next = temp;
        }
        head = head->next;
    }
}
