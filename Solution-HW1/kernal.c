#include "structs.h"

PROC *kfork()
{
    int i; 
    PROC *p = get_proc(&freeList);  //to get a FREE PROC from freeList
    if (!p)
    {
        printf ("no more PROC, kfork() failed \n");
        return 0;                   //if none, return 0 for FAIL
    }

    p->status = READY;              //status   = READY;
    p->priority = 1;                //priority = 1;
    p->ppid = running->ppid;        //ppid = running pid;
    p->parent = running;            //parent = running;

    for (i=1; i < 10; i++)          //initialize new proc's kstack
        p->kstack [SSIZE - i] = 0;  //save CPU registers

    p->kstack [SSIZE -1] = (int)body;
    enqueue (&readyQueue, p);
    return p;
}



