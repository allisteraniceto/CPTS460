/************ t.c file **********************************/
#define NPROC     9        
#define SSIZE  1024                /* kstack int size */

/* proc statii     */
#define FREE      0
#define READY     1
#define SLEEP     2
#define BLOCK     3
#define ZOMBIE    4

// define null, cause it ain't defined otherwise!
#define NULL  0
#define null  0

// 1. Define the structure PROC 
typedef struct proc{
    struct proc *next; //next pointer;   
    int *ksp;    /* saved sp(stack pointer); offset = 2 */
    int status;       /* FREE|READY|SLEEP|BLOCK|ZOMBIE */
    int priority;       // the priority!
    int pid;       // the process pid
    int ppid;       // the parent pid
    //struct proc *parent;       // pointer to parent proc
    int kstack[SSIZE];       // kmode(kernel mode)stack of task. SSIZE = 1024.
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

//2. Initialize the proc's
int initialize()
{
    /****************************************************************
    Initialize the proc's as showed in class:

    running->proc[0]--> proc[1] --> proc[2] ... --> proc[NPROC-1] -->
    ^                                                                |
    |<---------------------------------------<-----------------------

    Each proc's kstack contains:
    retPC, ax, bx, cx, dx, bp, si, di, flag;  all 2 bytes
    *****************************************************************/
    PROC *p;
    int i, j;
    for (i=0; i<NPROC; i++){ //initalize all PROCs
        p = &proc[i]; 
        p->pid = i; //pid = 0,1,2,..NPROC-1
        p->next = &proc[i+1]; //point to the next PROC
        if(i){ //for PROCS other than proc[0] (process zero is the default)
            p->kstack[SSIZE-1]=(int)body; //entry address of body()
            for (j=2; j<10; j++){ //all saved registers = 0
                p->kstack[SSIZE-j] = 0;
            }
            p->ksp = &(p->kstack[SSIZE-9]); //saved sp in PROC.kap
        }
    }
	proc[NPROC-1].next = &proc[0]; //all procceses form a circular list
	running = &proc[0]; //p0 is running
	printf("init complete");
}

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
    printf("starting kfork()\n"); // this works
    p->status = READY; //status = ready
    p->priority = 1; //priority = 1 for all proc except p0
    p->ppid = running->pid; //parent = running
    printf("1 here\n");
    /*initialize new process' kstack[]*/
    for(i = 1; i<10; i++){
        p->kstack[SSIZE-i] = 0; // all 0's
    }
    p->kstack[SSIZE-1] = (int)body; //resume point = address of body()
    p->ksp = &p->kstack[SSIZE-9]; //proc saved sp
    enqueue(&readyQueue,p); //enter p into readyQueue by priority
    
	printf("finished running kfork()!\n");

	return p; //return child PROC pointer
}

// 4. Get a FREE PROC
// get a FREE PROC from freeList; return PROC pointer; 
// return 0 if no more FREE PROCs.
PROC *get_proc(PROC **list)
{
    // return the next proc (dequeue) from the freeList
    
	if (list != NULL){
        return dequeue(&list); //get a free process by dequeue'ing it from freeList
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
	if (list == NULL){ //if no free process, add first one to freeList
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
	PROC *prev, *current;
     // Case 1: empty queue. make the passed process a new queue!
    printf("makes it to enqueu\n");
    if ((*queue) == NULL)
    {
		*queue = p; //queue head point first proces
		p->next = NULL; //no next process
		return;
    }
    // Case 2: non-empty queue, new process has greatest priority. insert new process onto head of the queue.
    else if (p->priority > (*queue)->priority)
    {
		p->next = *queue; //new process will point to head
		*queue = p; //head will now point to new process
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

	PROC *p = *queue; //get pprocess from queue
	if (*queue != NULL){ //if process is not empty
		*queue = (*queue)->next;
	}
	return p;
}

//6. print the queue entries in [pid, priority]->  format;
printQueue(PROC *queue)
{
    PROC *p = queue;
    // if we don't have a queue, say "There are no processes in this queue"
    // if we do, print each item in the queue
	if (queue == NULL){
		return;
	}
	//print queue list
	while (queue != NULL){
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
	if (running->status == READY){ 
		enqueue(&readyQueue, running); //put running process in readyQueue
	}
	running = dequeue(&readyQueue); //remove from readyQueue to running
}


// 8. print info on available commands.
help()
{
	//print the info for commands q, f, r, ?
    printf("\nAvailable Commands: \n");
    printf(" - s: Switch to the next ready process.\n");
	printf(" - q: Exit.\n");
	printf(" - f: Fork a child process");
	printf(" - r: Resurrect all zombie processes");
	printf(" - ?: Display help instructions");
}
// Add new commands
/****************************************************************
In the body() function, add a new command 'f'. The commands are:
      's' : call tswitch to switch process;
      'q' : call exit()
      '?' : print the help instructions
      'f' : kfork() a child PROC;
print message showing success or failure.
*****************************************************************/
// figure out what the user wants to do!




// 10. Run the os!
main()
{
    printf("\nWelcome to the CS460 Multitasking System New User!\n");
    
    printf("initializing...");
    initialize();
    
    printf("forking...");
    kfork();
    
    printf("switching...");
    tswitch();
    
    printf("\nGoodbye User!\n");
}

int body()
{
    char c;
    while(1)
    {
        // change the text color based on the process id!
        color = 0x01 + (running->pid % NPROC); 
        printf("\n******************************\n");
        printf("Currently Running Process #%d\n", running->pid);      
    	printf("Ready Queue: ");
		printQueue(readyQueue);
		printf("Infput a command [s | q | f | r | ?]:");
		c = getc();
		switch(c){
			case 's': //call twsitch() to switch process
				tswitch();
				break;
			case 'q': //call quit() to exit program
				break;
			case 'f': //kfork() a child process
				break;
			case 'r': //resurrect all zombie processes
				break;
			case '?': //print help instructions
				break;
			default:
				printf("INVALID CHARACTER");
		}

	}
}

