#include "structs.h"
#include "misc.h"

int body()
{
    char c;
	int pid;
	int t_flag; //if wrong command or switch, do not subtract time
    while(1)
    {
		t_flag = 1;
		if (running->time == 0){ //if time reaches 0, switch
			running->time = 0; //reset process time to 0;
			remaining_time = 0; //0 cpu time left
			do_tswitch();
		}
		if (rflag){ //if reschedule flag set, we need to reschedule running process
			printf("\nproc %d: reschedule\n", running->pid);
			rflag = 0; //reset resechedule flag
			tswitch(); //give up cpu
		}
        // change the text color based on the process id!
        color = 0x01 + (running->pid % NPROC); 
        printf("\n******************************\n");
        printf("Currently Running Process #%d\n", running->pid);
		printf("Running Queue: ");
		printQueue(running);      
    	printf("Ready Queue: ");
		printQueue(readyQueue);
		printf("Running Process Time: %d\n", running->time);
		printf("Infput a command [s | f | q | r | t | c | z | a | k | p | ?]:");
		c = getc();
		switch(c){
			case 's': //call twsitch() to switch process
				do_tswitch();
				t_flag = 0;
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
				do_continue();
				break;
			case 'z': //ksleep() to put running to sleep on event value
				do_sleep();
				break;
			case 'a': //kwakeup() to wakeup all procs sleeping on event
				do_wakeup();
				break;
			case 'k': //kexit for process termination
				do_kexit();
				break;
			case 'w':
				do_wait();
				break;
			case 'p':
				do_chpriority();
				break;
			case 'd': //kforkcustom()
				do_kforkcustom(); 
			case '?': //print help instructions
				help();
				break;
			default:
				printf("INVALID CHARACTER");
				t_flag = 0;
                break;
		}
		//decrement running process by for each command executed
		if(t_flag){ //time flag is true, subtract
			running->time -= 1;
			remaining_time = running->time;
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
	nproc = 1;
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
	//printf(" - r: Resurrect all zombie processes\n");
	printf(" - w: Wait for Child Process Termination\n");
	printf(" - p: Change priority of a process\n");
	printf(" - d: Preform custom kfork\n");
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
