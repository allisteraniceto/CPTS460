#include "t.c"

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
	}
	return p;
}

//6. print the queue entries in [pid, priority]->  format;
printQueue(PROC *queue)
{
    PROC *tmp = queue;
    // if we don't have a queue, say "There are no processes in this queue"
    // if we do, print each item in the queue
	if (queue == NULL){
		return;
	}
	//print queue list
	while (tmp != NULL){
		printf("[%d, %d]->",tmp->pid, tmp->priority);
		tmp = tmp->next;
	}
	printf("NULL\n"); //NULL at the end of list
}
