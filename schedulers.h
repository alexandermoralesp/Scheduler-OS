#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list
void add(char *name, int priority, int burst);

// add a task to the list  with vruntime
void add_cfs(char *name, int priority, int burst, int vruntime);

// invoke the scheduler
void schedule();
