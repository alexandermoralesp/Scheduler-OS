/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

struct node {
    Task *task;
    struct node *next;
};

// struct rbnode_t
// {
//   struct rbnode_t *next;
//   struct rbNode *node;
//   int vruntime;
// };

// struct rbNode
// {
//   int vruntime, color;
//   Task *task;
//   struct rbNode *link[2];
// };

// insert and delete operations.
void insert(struct node **head, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);
void insert_priority(struct node **head, Task *task);
// void insert_back(struct rbnode_t* head, struct rbNode* new_node);