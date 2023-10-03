#include "t.c"

//kernal.c file
//kernal main set of functions: kfork, kprint, sleep, wakeup, kexit, kwait

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
    for(i = 1; i < 10; i++){ //infinite loop here
        p->kstack[SSIZE-i] = 0; // all 0's
	}
    p->kstack[SSIZE-1] = (int)body; //resume point = address of body()
    p->ksp = &p->kstack[SSIZE-9]; //proc saved sp
    enqueue(&readyQueue,p); //enter p into readyQueue by priority  

	return p; //return child PROC pointer:
}

//running proc to sleep on an event value
int sleep(int event){
    //record event value in running PROC.event
    //change running PROC.status to SLEEP
    //switch process
    running->event = event;
    running->status = SLEEP;
    tswitch();
}

//wake up all processes on the event
int wakeup(int event){
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
    
}