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

// //wait for a zombie child 
// int kwait(int *status){
//     int i;
//     int zpid;
//     PROC *p;
//     if (p->ppid == 0){ //if no parent pp
//         return -1;
//     }
//     while(1){ //caller has children
//     //search for any zombie child
//         for(i=1; i<NPROC; i++){
//             p = &proc[i];
//             if (p->status == ZOMBIE){
//                 zpid = p->pid; //get zombie child pid

//                 return zpid;
//             }
//         }
//     }
// }

//running process waits for child. if child a zombie, collect exitCode and free resources
int kwait(int *status){
    PROC *p;
    int i;
    int hasChild;

    while(1){
        for (i=1; i<NPROC; i++){
            p = &proc[i];
            if (p->status != FREE && p->ppid == running->pid){ //if proces is not free and the process parent pid matches running pid
                hasChild=1; //running process has child
                if (p->status == ZOMBIE){   //lay dead child to rest
                    *status = p->exitCode;  //collect dead child's exit code
                    p->status = FREE;       //free dead child's process
                    enqueue(&freeList, p);  //put proc back to free list
                    nproc--; //one less process            
                    return (p->pid);        //return its pid
                }
            }
        }
        if(!hasChild){ //if running has no child, just return
            return -1;
        }
        ksleep(running); //running porcess still has kids alive, so put process to sleep
    }

}

// //process termination
// int kexit(int exitValue){
//     //1. erase process user-mode context, e.g. close file descriptors, 
//     //   release resources, deallocate user-mode image memory, etc.  <-we dont have user context yet so do not implement
//     //2. dispose of children processes, if any. 
//     //3. record exitValue in PROC.exitCode for parent to get.
//     //4. become a ZOMBIE (but do not free the PROC)
//     //5. wakeup parent and, if needed, also the INIT process P1.
// }

//runnning process termination and sends children to orphanage
int kexit(int exitValue){
    PROC* p;
    int i;
    int wakeupP1 = 0;
    //if P1 (default process) has children, cannot kill yet
    if (running->pid == 1 && nproc>2){ //nproc variable = # of active procs
        for (i=1; i<NPROC; i++){ //search for children
            if (proc[i].status == FREE && proc[i].pid == proc[1].pid){
                printf("OTHER PROCS NOT DEAD, P1 CANT DIE YET\n");
                return i; //return pid of process that is a child
            }
        }
    }
    //send children (dead or alive) to p1's orphanage
    for (i=1; i<NPROC; i++){
        p = &proc[i];
        if (p->status != FREE && p->ppid == running->pid){ //if child
            p->ppid = 1; //reassign ppid to first process
            p->parent = &proc[1]; 
            wakeupP1++;
        }
    }
    //record exitValue and become a ZOMBIE
    running->exitCode = exitValue;
    running->status = ZOMBIE;
    //wakeup parent and also P1 if necessary 
    kwakeup(running->parent); // parent sleeps on its PROC address //
    if (wakeupP1){
        kwakeup(&proc[1]);
    }
    tswitch(); // give up CPU
    return -1;
}

//change priority of a process and reschedule if needed
int chpriority(int pid, int pri){
    PROC *p;
    int i, ok, reQ;

    if (pid == running->pid){
        running->priority = pri;
        if (pri < readyQueue->priority){
            rflag = 1; //resechedule flag bit set, need to reschedule because lower priority
        }
        return 1;
    }
    //if process not running
    for (i = 1; i<NPROC; i++){ //starts at 1 because cannot change process 1 priority (will always be 0)
        p = &proc[i];
        if (p->pid == pid && p->status != FREE){ //if pid matches and process is not free, 
            p->priority = pri;
            ok = 1;
            if (p->status == READY){ //in reqdyQueue, then rearrange ready queue
                reQ = 1; 
            }
        }
    }
    if (!ok){
        printf("chpriority failed\n");
        return -1;
    }
    if (reQ){ //if redo readyQueue set, call reschedule
        reschedule();
    }
}
