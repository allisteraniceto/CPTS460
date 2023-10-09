#include "structs.h"

//kernal.c file
//kernal main set of functions: kfork, kprint, sleep, wakeup, kexit, kwait

//3. Write a PROC *kfork()
// get the proc...
// if there were no procs, report kfork's failure  
// initialize the proc status, priority, ppid, parent...
// now to setup the kstack!
// first things first, lets clean up the registers by setting them to 0.
// now we need to make sure to call tswitch from body when the proc runs...
// set the ksp to point to the top of the stack  
// enter the proc into the readyQueue, since it's now ready for primetime!
// return the new proc!!!
// function to create a process DYNAMICALLY
PROC *kfork()
{
  /****************************************************************
  Instead of creating ALL the PROCs at once, write a
  PROC *kfork()
  function to create a process DYNAMICALLY.

  PROC *kfork()
  {
  (1). PROC *p = get_proc(); to get a FREE PROC from freeList;
  if none, return 0 for FAIL;

  (2). Initialize the new PROC p with
  --------------------------
  status   = READY;
  priority = 1;
  ppid = running pid;
  parent = running;
  --------------------------

  *********** THIS IS THE MAIN PART OF THE ASSIGNMENT!!!***********
  INITIALIZE p's kstack to make it start from body() when it runs.

  To do this, PRETNED that the process called tswitch() from the
  the entry address of body() and executed the SAVE part of tswitch()
  to give up CPU before.
  Initialize its kstack[ ] and ksp to comform to these.

  enter p into readyQueue;
  *****************************************************************

  return p;
  }
  *****************************************************************/
	int i;  
    PROC *p = get_proc(&freeList); //to get FREE PROC from freeList
	if (!p){ //if no proccesses, kfork() does not work
        printf("no more PROC, kfork() failed\n");
    	return 0;
	}
    p->status = READY; //status = ready
    p->priority = 1; //priority = 1 for all proc except p0
    p->ppid = running->pid; //parent = running
    /*initialize new process' kstack[]*/
    for(i = 1; i < 10; i++){
        p->kstack[SSIZE-i] = 0; // all 0's
	}
    p->kstack[SSIZE-1] = (int)body; //resume point = address of body()
    p->ksp = &p->kstack[SSIZE-9]; //proc saved sp
    enqueue(&readyQueue,p); //enter p into readyQueue by priority  

	return p; //return child PROC pointer:
}

//running proc to sleep on an event value
int ksleep(int event){
    //record event value in running PROC.event
    //change running PROC.status to SLEEP
    //switch process
    running->event = event;
    running->status = SLEEP;
    tswitch();
}

//wake up all processes on the event
int kwakeup(int event){
    //for every proc in the PROC array
    //if proc's SLEEP and event==event
    //  change proc to READY and enter into readyQueue
    int i;
    PROC *p;
    for (i=1; i<NPROC; i++){ //go through every process
        p = &proc[i];
        if (p->status == SLEEP && p->event == event){
            p->event = 0; //cancel proc's event
            p->status = READY;
            enqueue(&readyQueue, p);
        }
    }
}

//wait for a zombie child 
int kwait(int *status){
    int i;
    int zpid;
    PROC *p;
    if (p->ppid == 0){ //if no parent pp
        return -1;
    }
    while(1){ //caller has children
    //search for any zombie child
        for(i=1; i<NPROC; i++){
            p = &proc[i];
            if (p->status == ZOMBIE){
                zpid = p->pid; //get zombie child pid

                return zpid;
            }
        }
    }
}

//process termination
int kexit(int exitValue){
    //1. erase process user-mode context, e.g. close file descriptors, 
    //   release resources, deallocate user-mode image memory, etc.  <-we dont have user context yet so do not implement
    //2. dispose of children processes, if any. 
    //3. record exitValue in PROC.exitCode for parent to get.
    //4. become a ZOMBIE (but do not free the PROC)
    //5. wakeup parent and, if needed, also the INIT process P1.
}