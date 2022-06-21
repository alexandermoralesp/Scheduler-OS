#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "list.h"
#include "cpu.h"
#include "task.h"

enum nodeColor
{
  RED,
  BLACK
};

struct rbNode
{
  int vruntime, color;
  Task *task;
  struct rbNode *link[2];
};

// Number of process in the system
int number_of_process = 0;
struct rbNode *root = NULL;
int nextTid = 0;

// Create a red-black tree
struct rbNode *createNode(int vruntime, Task *task)
{
  struct rbNode *newnode;
  newnode = (struct rbNode *)malloc(sizeof(struct rbNode));
  newnode->vruntime = vruntime;
  newnode->color = RED;
  newnode->task = task;
  newnode->link[0] = newnode->link[1] = NULL;
  return newnode;
}

// Insert an node
void insertion_rb(int vruntime, Task *task)
{
  number_of_process++;
  struct rbNode *stack[98], *ptr, *newnode, *xPtr, *yPtr;
  int dir[98], ht = 0, index;
  ptr = root;
  if (!root)
  {
    root = createNode(vruntime, task);
    return;
  }

  stack[ht] = root;
  dir[ht++] = 0;
  while (ptr != NULL)
  {
    // if (ptr->vruntime == vruntime) {
    //   printf("Duplicates Not Allowed!!\n");
    //   return;
    // }
    index = (vruntime - ptr->vruntime) > 0 ? 1 : 0;
    stack[ht] = ptr;
    ptr = ptr->link[index];
    dir[ht++] = index;
  }
  stack[ht - 1]->link[index] = newnode = createNode(vruntime, task);
  while ((ht >= 3) && (stack[ht - 1]->color == RED))
  {
    if (dir[ht - 2] == 0)
    {
      yPtr = stack[ht - 2]->link[1];
      if (yPtr != NULL && yPtr->color == RED)
      {
        stack[ht - 2]->color = RED;
        stack[ht - 1]->color = yPtr->color = BLACK;
        ht = ht - 2;
      }
      else
      {
        if (dir[ht - 1] == 0)
        {
          yPtr = stack[ht - 1];
        }
        else
        {
          xPtr = stack[ht - 1];
          yPtr = xPtr->link[1];
          xPtr->link[1] = yPtr->link[0];
          yPtr->link[0] = xPtr;
          stack[ht - 2]->link[0] = yPtr;
        }
        xPtr = stack[ht - 2];
        xPtr->color = RED;
        yPtr->color = BLACK;
        xPtr->link[0] = yPtr->link[1];
        yPtr->link[1] = xPtr;
        if (xPtr == root)
        {
          root = yPtr;
        }
        else
        {
          stack[ht - 3]->link[dir[ht - 3]] = yPtr;
        }
        break;
      }
    }
    else
    {
      yPtr = stack[ht - 2]->link[0];
      if ((yPtr != NULL) && (yPtr->color == RED))
      {
        stack[ht - 2]->color = RED;
        stack[ht - 1]->color = yPtr->color = BLACK;
        ht = ht - 2;
      }
      else
      {
        if (dir[ht - 1] == 1)
        {
          yPtr = stack[ht - 1];
        }
        else
        {
          xPtr = stack[ht - 1];
          yPtr = xPtr->link[0];
          xPtr->link[0] = yPtr->link[1];
          yPtr->link[1] = xPtr;
          stack[ht - 2]->link[1] = yPtr;
        }
        xPtr = stack[ht - 2];
        yPtr->color = BLACK;
        xPtr->color = RED;
        xPtr->link[1] = yPtr->link[0];
        yPtr->link[0] = xPtr;
        if (xPtr == root)
        {
          root = yPtr;
        }
        else
        {
          stack[ht - 3]->link[dir[ht - 3]] = yPtr;
        }
        break;
      }
    }
  }
  root->color = BLACK;
}

// delete_rb a node
void deletion_rb(int vruntime)
{
  struct rbNode *stack[98], *ptr, *xPtr, *yPtr;
  struct rbNode *pPtr, *qPtr, *rPtr;
  int dir[98], ht = 0, diff, i;
  enum nodeColor color;

  if (!root)
  {
    printf("Tree not available\n");
    return;
  }
  number_of_process--;
  ptr = root;
  while (ptr != NULL)
  {
    if ((vruntime - ptr->vruntime) == 0)
      break;
    diff = (vruntime - ptr->vruntime) > 0 ? 1 : 0;
    stack[ht] = ptr;
    dir[ht++] = diff;
    ptr = ptr->link[diff];
  }

  if (ptr->link[1] == NULL)
  {
    if ((ptr == root) && (ptr->link[0] == NULL))
    {
      // free(ptr);
      root = NULL;
    }
    else if (ptr == root)
    {
      root = ptr->link[0];
      // free(ptr);
    }
    else
    {
      stack[ht - 1]->link[dir[ht - 1]] = ptr->link[0];
    }
  }
  else
  {
    xPtr = ptr->link[1];
    if (xPtr->link[0] == NULL)
    {
      xPtr->link[0] = ptr->link[0];
      color = xPtr->color;
      xPtr->color = ptr->color;
      ptr->color = color;

      if (ptr == root)
      {
        root = xPtr;
      }
      else
      {
        stack[ht - 1]->link[dir[ht - 1]] = xPtr;
      }

      dir[ht] = 1;
      stack[ht++] = xPtr;
    }
    else
    {
      i = ht++;
      while (1)
      {
        dir[ht] = 0;
        stack[ht++] = xPtr;
        yPtr = xPtr->link[0];
        if (!yPtr->link[0])
          break;
        xPtr = yPtr;
      }

      dir[i] = 1;
      stack[i] = yPtr;
      if (i > 0)
        stack[i - 1]->link[dir[i - 1]] = yPtr;

      yPtr->link[0] = ptr->link[0];

      xPtr->link[0] = yPtr->link[1];
      yPtr->link[1] = ptr->link[1];

      if (ptr == root)
      {
        root = yPtr;
      }

      color = yPtr->color;
      yPtr->color = ptr->color;
      ptr->color = color;
    }
  }

  if (ht < 1)
    return;

  if (ptr->color == BLACK)
  {
    while (1)
    {
      pPtr = stack[ht - 1]->link[dir[ht - 1]];
      if (pPtr && pPtr->color == RED)
      {
        pPtr->color = BLACK;
        break;
      }

      if (ht < 2)
        break;

      if (dir[ht - 2] == 0)
      {
        rPtr = stack[ht - 1]->link[1];

        if (!rPtr)
          break;

        if (rPtr->color == RED)
        {
          stack[ht - 1]->color = RED;
          rPtr->color = BLACK;
          stack[ht - 1]->link[1] = rPtr->link[0];
          rPtr->link[0] = stack[ht - 1];

          if (stack[ht - 1] == root)
          {
            root = rPtr;
          }
          else
          {
            stack[ht - 2]->link[dir[ht - 2]] = rPtr;
          }
          dir[ht] = 0;
          stack[ht] = stack[ht - 1];
          stack[ht - 1] = rPtr;
          ht++;

          rPtr = stack[ht - 1]->link[1];
        }

        if ((!rPtr->link[0] || rPtr->link[0]->color == BLACK) &&
            (!rPtr->link[1] || rPtr->link[1]->color == BLACK))
        {
          rPtr->color = RED;
        }
        else
        {
          if (!rPtr->link[1] || rPtr->link[1]->color == BLACK)
          {
            qPtr = rPtr->link[0];
            rPtr->color = RED;
            qPtr->color = BLACK;
            rPtr->link[0] = qPtr->link[1];
            qPtr->link[1] = rPtr;
            rPtr = stack[ht - 1]->link[1] = qPtr;
          }
          rPtr->color = stack[ht - 1]->color;
          stack[ht - 1]->color = BLACK;
          rPtr->link[1]->color = BLACK;
          stack[ht - 1]->link[1] = rPtr->link[0];
          rPtr->link[0] = stack[ht - 1];
          if (stack[ht - 1] == root)
          {
            root = rPtr;
          }
          else
          {
            stack[ht - 2]->link[dir[ht - 2]] = rPtr;
          }
          break;
        }
      }
      else
      {
        rPtr = stack[ht - 1]->link[0];
        if (!rPtr)
          break;

        if (rPtr->color == RED)
        {
          stack[ht - 1]->color = RED;
          rPtr->color = BLACK;
          stack[ht - 1]->link[0] = rPtr->link[1];
          rPtr->link[1] = stack[ht - 1];

          if (stack[ht - 1] == root)
          {
            root = rPtr;
          }
          else
          {
            stack[ht - 2]->link[dir[ht - 2]] = rPtr;
          }
          dir[ht] = 1;
          stack[ht] = stack[ht - 1];
          stack[ht - 1] = rPtr;
          ht++;

          rPtr = stack[ht - 1]->link[0];
        }
        if ((!rPtr->link[0] || rPtr->link[0]->color == BLACK) &&
            (!rPtr->link[1] || rPtr->link[1]->color == BLACK))
        {
          rPtr->color = RED;
        }
        else
        {
          if (!rPtr->link[0] || rPtr->link[0]->color == BLACK)
          {
            qPtr = rPtr->link[1];
            rPtr->color = RED;
            qPtr->color = BLACK;
            rPtr->link[1] = qPtr->link[0];
            qPtr->link[0] = rPtr;
            rPtr = stack[ht - 1]->link[0] = qPtr;
          }
          rPtr->color = stack[ht - 1]->color;
          stack[ht - 1]->color = BLACK;
          rPtr->link[0]->color = BLACK;
          stack[ht - 1]->link[0] = rPtr->link[1];
          rPtr->link[1] = stack[ht - 1];
          if (stack[ht - 1] == root)
          {
            root = rPtr;
          }
          else
          {
            stack[ht - 2]->link[dir[ht - 2]] = rPtr;
          }
          break;
        }
      }
      ht--;
    }
  }
}

// Print the inorder traversal of the tree
void inorderTraversal(struct rbNode *node)
{
  if (node)
  {
    inorderTraversal(node->link[0]);
    printf("%d  ", node->vruntime);
    inorderTraversal(node->link[1]);
  }
  return;
}

struct rbNode *get_lowest(struct rbNode *node)
{
  if (node->link[0])
  {
    return get_lowest(node->link[0]);
  }
  else
  {
    return node;
  }
}

int current_vruntime = 0;

struct noderb_t
{
  struct noderb_t *next;
  struct rbNode *node;
};

struct noderb_t *head = NULL;

// TODO: Implement in O(1)
void push_back_rb(struct noderb_t **header, struct rbNode *node)
{
  struct noderb_t *new_node = (struct noderb_t *)malloc(sizeof(struct noderb_t));
  new_node->node = node;
  new_node->next = NULL;
  if (*header==  NULL) {
    *header= new_node;
  }
  else {
    struct noderb_t *temp = *header;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = new_node;
  }
}
void print_all(struct noderb_t *header)
{
  struct noderb_t *temp = header;
  while (temp != NULL) {
    printf("%s ", temp->node->task->name);
    temp = temp->next;
  }
  printf("\n");
}

struct noderb_t *pop_front_rb(struct noderb_t **head)
{
  if (*head == NULL) {
    return NULL;
  } else {
    struct noderb_t *temp = *head;
    *head = (*head)->next;
    return temp;
  }
}

// Insertion in Red Black Tree
void add_cfs(char *name, int priority, int burst, int vruntime)
{
  printf("Adding %s with priority %d and burst %d\n", name, priority, burst);
  Task *task = (Task *)malloc(sizeof(Task));
  task->name = name;
  task->priority = priority;
  task->burst = burst;
  task->tid = ++nextTid;
  insertion_rb(vruntime, task);
}
// free head list
void free_list(struct noderb_t *head)
{
  struct noderb_t *temp = head;
  while (temp != NULL) {
    struct noderb_t *temp2 = temp;
    temp = temp->next;
    free(temp2);
  }
}
void schedule()
{
  while (number_of_process != 0)
  // for (size_t i = 0; i < 2; i++)
  {
    int current_number_of_process = number_of_process;
    printf("total number of process %d\n", current_number_of_process);
    int time_slice = ceil(QUANTUM / current_number_of_process);
    for (size_t i = 0; i < current_number_of_process; i++)
    {
      struct rbNode *node = get_lowest(root);
      push_back_rb(&head, node);
      print_all(head);
      deletion_rb(node->vruntime);
    }
    for (size_t i = 0; i < current_number_of_process-1; i++)
    {
      struct noderb_t *front= pop_front_rb(&head);
      if (front->node->task->burst < time_slice) {
        printf("if\n");
        run(front->node->task, front->node->task->burst);
      } else {
        printf("else\n");
        run(front->node->task, time_slice);
        add_cfs(front->node->task->name, front->node->task->priority, front->node->task->burst - time_slice, front->node->vruntime + time_slice);
      }
    }
    free_list(head);
    head = NULL;
  }
}