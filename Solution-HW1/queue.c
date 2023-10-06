#include "queue.h"
#include "structs.h"

/**
 * get_proc returns the first node from a list passed to it.
 */
PROC *get_proc(PROC **list)
{
    if ((*list) != NULL)
		return dequeue(list);
	return 0;
}

void put_proc(PROC **list,PROC *p)
{
   // p->status == FREE;
    p->next = list;
    list = p;
}

/**
 * enqueue adds a process to a list based on priority.
 */
void enqueue(PROC **queue, PROC *p)
{
    // in case 5 is best priority
    // Case 1: empty queue. make the passed process a new queue!
    if ((*queue) == NULL)
    {
        *queue = p;
        p->next = NULL;
    }
    // Case 2: non-empty queue, new process has greatest priority. insert new process onto head of the queue.
    else if (p->priority > (*queue)->priority)
    {
        p->next = *queue;
        *queue = p;
    }
    // Case 3: non-empty queue, new process needs to be inserted somewhere in it. look through the processes in the queue until we find a spot where the process' priority will be properly respected.
    else
    {
        PROC *current = *queue;
        while (current->next != NULL && current->priority >= p->priority) {
            current = current->next;
        }
        p->next = current->next;
        current->next = p;
    }
}

/**
 * dequeue removes the a process from a list that has the highest priority.
 * It returns a pointer to this process.
 */
PROC *dequeue(PROC **queue)
{
    PROC *p = *queue;
    if ((*queue) == NULL) {
        printf("\ndequeue was null, cannot remove PROC\n");
    } else {
        *queue = p->next;
        p->next = NULL;
    }
    return p;
}

/**
 * printQueue prints a list given to it.
 */
void printQueue(PROC *queue)
{
    PROC *p = queue;
    if ((queue) == NULL) {
        printf("\nThere are no processes in this queue\n");
    } else {
        while(p->next != NULL) {
            printf(" [%d, %d]-->", p->pid,p->ppid);
            p = p->next;
        }
        if (p->next == NULL) {
            printf(" [%d, %d]--> NULL\n", p->pid,p->ppid);
        }
    }
}

/**
 * Scheduler checks if the running process status is READY, if 
 * yes, then the running process is placed in the ready queue,
 * then a new running process is taken from the ready queue.
 */
int scheduler()
{
  //Use the MODIFIED scheduler() function propose in class
  if (running->status == READY) // if running is still READY
    enqueue(&readyQueue, running); // enter it into readyQueue
  running = dequeue(&readyQueue); // new running
}
