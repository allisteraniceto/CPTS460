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

//for unsigned int
typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define MTXSEG 0x1000

// Bool definition
typedef enum { false, true } bool;

// 1. Define the structure PROC 
typedef struct proc{
    struct proc *next; //next pointer;   
    int    *ksp;    /* saved sp(stack pointer); offset = 2 */
    int    uss, usp;           // at offsets 4,6
	int    pid;         //the process pid
	int    ppid;        //the parent pid
    struct proc *parent; //pointer to parent
    int    status;       /* FREE|READY|SLEEP|BLOCK|ZOMBIE */
    int    priority;       // the priority!
    int    kstack[SSIZE];       // kmode(kernel mode)stack of task. SSIZE = 1024.
    int    event; //sleep at an event
    int    exitCode; //exit code
    int    time;
    char   name[32];
}PROC;

/**** USE YOUR OWN io.c with YOUR printf() here *****/
PROC proc[NPROC], *running, *freeList, *readyQueue;
int  procSize = sizeof(PROC);
int  color = 0x0C;
int rflag; //re-schedule flag
// define the functions so things don't break! cause c!

//misc.c 
int nproc; //starting proc
int remaining_time;
int body();  
int initialize();
help();

//queue.c
PROC *get_proc(PROC **list);
int put_proc(PROC **list, PROC *p);
int enqueue(PROC **queue, PROC *p);
PROC *dequeue(PROC **queue);
int printQueue(PROC *queue);
int scheduler();
int reschedule();

//kernel.c
PROC *kfork();
int ksleep(int event);
int kwakeup(int event);
int kwait(int *status);
int kexit(int exitValue);
int kforkcustom(char *filename);

//do_functions.c
int do_tswitch();
int do_kfork();
int do_exit();
int do_stop();
int do_continue();
int do_sleep();
int do_wakeup();
int do_kforkcustom();

//int.c
int kcinth();
int kgetpid();
int kpd();
int kchname(char *name);
int kkfork();
int ktswitch();
int kkwait(int *status);
int kkexit(int value);

#endif