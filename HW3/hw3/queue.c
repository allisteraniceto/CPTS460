#include "queue.h"
#include "structs.h"

// 4. Get a FREE PROC
// get a FREE PROC from freeList; return PROC pointer; 
// return 0 if no more FREE PROCs.
PROC *get_proc(PROC **list)
{
    // return the next proc (dequeue) from the freeList
    
	if (*list != NULL){
        return dequeue(list); //get a free process by dequeue'ing it from freeList
    }
    return 0; //return 0 if there are no more free processes
}

// Enter p into freeList;
put_proc(PROC **list, PROC *p)
{
    // even though enqueue is similar, they're different enough to mean this should be unique...
    // set the status to free and add to the linked list freeList
    // if we had no free procs, this'll be our first!
	
	p->status = FREE; //set process' status to FREE (0 = True)
	if (*list == NULL){ //if no free process, add first one to freeList
		*list = p;
		p->next = NULL;
	}else{ //if freeList is not empty, insert between freeList and freeList->next
		p->next = *list;  
		*list = p; 
	}	
}


// 5.Enter p into queue (by priority in the next homework)
// create an enqueue method, use three cases:
// Case 1: empty queue. make the passed process a new queue!
// Case 2: non - empty queue, new process has greatest priority. Insert new process onto head of the queue.
// Case 3: non-empty queue, new process needs to be inserted somewhere in it. look through the processes in the queue until we find a spot where the process' priority will be properly respected.

enqueue(PROC **queue, PROC *p)
{	
	//for priority: 0-5, 5 is best

	PROC *prev, *current;
     // Case 1: empty queue. make the passed process a new queue!
    if ((*queue) == NULL)
    {
		(*queue) = p; //queue head point first proces
		(*queue)->next = NULL; //no next process
		return;
    }
    // Case 2: non-empty queue, new process has greatest priority. insert new process onto head of the queue.
    else if (p->priority > (*queue)->priority)
    {
		p->next = *queue; //new process will point to head
		(*queue) = p; //head will now point to new process
		return;
    }
    // Case 3: non-empty queue, new process needs to be inserted somewhere in it. look through the processes in the queue until we find a spot where the process' priority will be properly respected.
    else
    {	
		prev = NULL; //used for previous process
		current = *queue; //head process
		//traverse through list and compare priorities
		while(current && p->priority <= current->priority){
			prev = current; 
			current = current->next; //compare to next priority
		}
		//then insert process in queue
		prev->next = p;
		p->next = current;
	}
}

//Remove a PROC (highest priority in the next homework) 
//order by priority the first one in queue
// return its pointer;
PROC *dequeue(PROC **queue){
    // get the proc we need...
    // and then modify the queue to remove the proc we need...
    // return the proc we deserve!

	PROC *p = (*queue); //get pprocess from queue
	if (*queue != NULL){ //if process is not empty
		*queue = (*queue)->next;
		p->next = NULL;
	}
	return p;
}

//6. print the queue entries in [pid, priority]->  format;
printQueue(PROC *queue)
{
    PROC *p = queue;
    // if we don't have a queue, say "There are no processes in this queue"
    // if we do, print each item in the queue
	if (p == NULL){
		return;
	}
	//print queue list
	while (p != NULL){
		printf("[%d, %d]->",p->pid, p->priority);
		p = p->next;
	}
	printf("NULL\n"); //NULL at the end of list
}

// 7. Scheduler
/****************************************************************
Use the MODIFIED scheduler() function propose in class
*****************************************************************/
// schedule ALL the processes!
int scheduler()
{
  //Use the MODIFIED scheduler() function propose in class
	if (running->status == READY && running->pid != 0){ 
		running->priority = current_time + 1; //now if not running queue and ready, implement aging
		enqueue(&readyQueue, running); 
	}else if(running->status == READY){ //for process 0
		enqueue(&readyQueue, running); //put running process in readyQueue
	}
	running = dequeue(&readyQueue); //remove from readyQueue to running
}

int reschedule()
{
	PROC *p, *tempQ = 0;
	while ( (p=dequeue(&readyQueue)) ){ // reorder readyQueue
		enqueue(&tempQ, p);
	}
	readyQueue = tempQ;
	rflag = 0; // global reschedule flag back to 0
	if (running->priority < readyQueue->priority)
		rflag = 1;
}

