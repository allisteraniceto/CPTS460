#ifndef QUEUE_H
#define QUEUE_H
#include "structs.h"

PROC *get_proc(PROC **list);
void put_proc(PROC **list,PROC *p);
void enqueue(PROC **queue, PROC *p);
PROC *dequeue(PROC **queue);
void printQueue(PROC *queue);
int scheduler();

#endif
