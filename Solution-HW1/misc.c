#include "structs.h"
#include "misc.h"

/**
 * Body is each process's code. It gives the user a choice of what should be done
 * next. Based on their input, a new action will be called.
 */
int body()
{
    char c;
    while(1)
    {
        color = 0x01 + (running->pid % NPROC); // change the text color based on the process id!
        printf("\n******************************\n");
        printf("Currently Running Process #%d", running->pid);
        printf("\nReady Queue: ");
        printQueue(readyQueue);
        printf("Free List: ");
        printQueue(freeList);
        printf("\nEnter choice: f | s | ?");
        c = getc();
        if (c == 's' || c == 'S') {
            tswitch();
        } else if (c == 'f' || c == 'F') {
            do_kfork();
        } else if (c == '?') {
            help();
	    } else {
            printf("\nInvalid choice entered.\n");
        }
    }
}


/**
 * Initialize creates NPROC and places them in the free list.
 * The first of these processes (p0) is set to running at start.
 */
int initialize()
{
    PROC *p; int i;
    for (i=0; i<NPROC; i++) {
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;
        p->next = &proc[i+1];
    }
    proc[NPROC-1].next = NULL;
    freeList = &proc[0];
    readyQueue = 0;
    p = get_proc(&freeList);
    p->ppid = 0;
    p->status = READY;
    running = p;
}

/**
 * Help prints a message to the user about what commands do.
 */
void help()
{
    printf("\nAvailable Commands: \n");
    printf(" - f: Fork a new process.\n");
    printf(" - s: Switch to the next ready process.\n");
    printf(" - ?: View help.\n");
}
