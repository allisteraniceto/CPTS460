#ifndef QUEUE_H
#define QUEUE_H
#include "structs.h"

PROC *get_proc(PROC **list);
put_proc(PROC **list,PROC *p);
enqueue(PROC **queue, PROC *p);
PROC *dequeue(PROC **queue);
printQueue(PROC *queue);
int scheduler();

#endif