/************ t.c file **********************************/
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

// 1. Define the structure PROC 
typedef struct proc{
    struct proc *next; //next pointer;   
    int *ksp;    /* saved sp(stack pointer); offset = 2 */
	int pid;         //the process pid
	int ppid;        //the parent pid
    int status;       /* FREE|READY|SLEEP|BLOCK|ZOMBIE */
    int priority;       // the priority!
    int kstack[SSIZE];       // kmode(kernel mode)stack of task. SSIZE = 1024.
    int event;
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
    int i;
    for (i=0; i<NPROC; i++){ //initalize all PROCs
        p = &proc[i]; 
        p->pid = i; //pid = 0,1,2,..NPROC-1
		p->status = FREE;
        p->priority = 0;
		p->next = &proc[i+1]; //point to the next PROC
    }
	proc[NPROC-1].next = NULL; //last proc points to null
	freeList = &proc[0]; //all procs are in freeList
	readyQueue = 0;
	/*************** create p0 as running ****************/
	p = get_proc(&freeList);
	p->ppid = 0;
    p->status = READY;
	running = p;
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
			case 'q': //call exit() to exit program
				printf("quitting...");
				//exit();
				break;
			case 'f': //kfork() a child process
				kfork();
				break;
			case 'r': //resurrect all zombie processes
                printf("ressurct");
				break;
			case '?': //print help instructions
				help();
				break;
			default:
				printf("INVALID CHARACTER");
                break;
		}

	}
}

