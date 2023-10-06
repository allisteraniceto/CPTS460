#ifndef STRUCTS_H
#define STRUCTS_H
/************ structs.h file **********************************/
#define NPROC     9        
#define SSIZE  1024                /* kstack int size */

/* proc statii     */
#define FREE      0
#define READY     1
#define SLEEP     2
#define BLOCK     3
#define ZOMBIE    4
#define DEAD	  5
#define STOP	  6

// define null, cause it ain't defined otherwise!
#define NULL	0
#define null	0

/**
 * Definition of process struct
*/
typedef struct proc{
           struct proc *next;    //next pointer;   
           int *ksp;             /* saved sp; offset = 2 */
           int status;           /* FREE|READY|SLEEP|BLOCK|ZOMBIE */
           int priority;         // the priority!
           int pid;              // the process pid
           int ppid;             // the parent pid
           int event;            // the event
           struct proc *parent;  // pointer to parent proc
           int kstack[SSIZE];    // kmode stack of task. SSIZE = 1024.
}PROC;

//////////////////////////////////////// VARIABLES ////////////////////////////////////////
////////// Global Variables  //////////
PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList,stoplist;
int procSize = sizeof(PROC);
int nproc = 0;
int color;

//////////////////////////////////////// FUNCTIONS ////////////////////////////////////////
////////// kernal.c //////////
PROC *kfork();
//bool ksleep(int event);
//int kwakeup(int event);
//include the remaining functions

////////// queue.c //////////
int scheduler();
PROC *get_proc(PROC **list);
void put_proc(PROC **list,PROC *p);
void enqueue(PROC **queue, PROC *p);
PROC *dequeue(PROC **queue);
void printQueue(PROC *queue);

////////// do_functions.c //////////
bool do_tswitch();
bool do_kfork();
//include the remaining functions

////////// functions.c //////////
bool help();
int initialization();
int body();

/*
//////////////////////////////////////// COMMAND TABLE ////////////////////////////////////////
COMMANDTABLE commands[] = {
{ 's', "Switch", do_tswitch, "Switch to the next ready process." },
{ 'f', "Fork", do_kfork, "Forks a new process from the free processes." },
{ 'q', "Kill", do_exit, "Makes the running PROC die." },
{ 'p', "PrintProcs", do_ps, "Print pid, ppid, and status of all processes." },
{ 'z', "Sleep", do_sleep, "Running proc to sleep on an event value." },
{ 'a', "Wake", do_wakeup, "Wakeup all procs sleeping on event." },
{ 'w', "Wait", do_wait, "Wait for a zombie child." },
{ 'r', "Free", do_free, "Frees the currently running process." },
{ 'u', "Resurrect", do_resurrect, "Puts all zombie processes on the free list." },
{ 'e', "Quit", do_quitprogram, "Quits the program by killing all running and ready processes." },
{ '?', "Help", do_help, "Brings up this help menu!" },
{ 0, 0, 0, 0 }
};*/

#endif
