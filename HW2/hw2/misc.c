#include "structs.h"
#include "misc.h"

int body()
{
    char c;
	char pid;
    while(1)
    {
        // change the text color based on the process id!
        color = 0x01 + (running->pid % NPROC); 
        printf("\n******************************\n");
        printf("Currently Running Process #%d\n", running->pid);      
    	printf("Ready Queue: ");
		printQueue(readyQueue);
		printf("Infput a command [s | f | q | r | t | c | z | a | k | ?]:");
		c = getc();
		switch(c){
			case 's': //call twsitch() to switch process
				do_tswitch();
				break;
			case 'f': //kfork() a child process
				do_kfork();
				break;				
			case 'q': //call do_exit() to make running process die
				do_exit();
				break;
			case 'r': //resurrect all zombie processes
                printf("ressurct");
				break;
			case 't': //stops current process
				do_stop();
				break;
			case 'c': //continue stopped process
				printf("Enter Process PID to continue: "); //get pid from user
				pid = getc()
				do_continue(pid);
				break;
			case 'z': //ksleep() to put running to sleep on event value
				//ksleep(eventValue);
				break;
			case 'a': //kwakeup() to wakeup all procs sleeping on event
				//kwakeup(eventValue);
				break;
			case 'k': //kexit for process termination
				//kexit()
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

// 8. print info on available commands.
help()
{
	//print the info for commands q, f, r, ?
    printf("\nAvailable Commands: \n");
    printf(" - s: Switch to the next ready process.\n");
	printf(" - q: Makes the running process die.\n");
	printf(" - f: Fork a child process\n");
	printf(" - t: Stop running process\n");
	printf(" - c: Let stopped process continue\n");
	printf(" - z: Put running process to sleep\n");
	printf(" - a: Wakeup all procs sleeping on the event\n");
	printf(" - k: Process Termination\n");
	printf(" - r: Resurrect all zombie processes\n");
	printf(" - ?: Display help instructions\n");
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