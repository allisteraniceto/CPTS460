#ifndef STRUCTS_H
#define STRUCTS_H

#define NPROC     9        
#define SSIZE  1024                /* kstack int size */

/* proc statii     */
#define FREE      0
#define READY     1
#define SLEEP     2
#define BLOCK     3
#define ZOMBIE    4
#define DEAD      5
#define STOP      6

// define null, cause it ain't defined otherwise!
#define NULL  0
#define null  0

// 1. Define the structure PROC 
typedef struct proc{
    struct proc *next; //next pointer;   
    int *ksp;    /* saved sp(stack pointer); offset = 2 */
	int pid;         //the process pid
	int ppid;        //the parent pid
    int status;       /* FREE|READY|SLEEP|BLOCK|ZOMBIE */
    int priority;       // the priority!
    int kstack[SSIZE];       // kmode(kernel mode)stack of task. SSIZE = 1024.
    int event;
}PROC;

/**** USE YOUR OWN io.c with YOUR printf() here *****/
PROC proc[NPROC], *running, *freeList, *readyQueue;
int  procSize = sizeof(PROC);
int  color = 0x0C;
// define the functions so things don't break! cause c!
int body();  
int initialize();
PROC *get_proc(PROC **list);
put_proc(PROC **list, PROC *p);
enqueue(PROC **queue, PROC *p);
PROC *dequeue(PROC **queue);
printQueue(PROC *queue);
PROC *kfork();
int scheduler();
help();

#endif