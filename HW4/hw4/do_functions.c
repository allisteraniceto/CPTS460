#include "misc.h"
#include "structs.h"


int do_tswitch(){
    if(running->time > 0){ //if there is leftover cpu time
        current_time = running->time; //use current_time to add to next running process
    }
    tswitch();
    running->time = 5 + current_time + 1; //default 5 time quantum + any leftover + twsitch doesn't take cpu time
}

int do_kforkcustom()
{
    kforkcustom("/bin/u1");
}

int do_kfork(){
    PROC *p = kfork();
    if (p==0){
        printf("Kfork failed\n");
        return -1;
    }
    printf("PROC %d kfork a child %d\n", running->pid, p->pid);
    return p->pid;
}

//kill running process
do_exit(){
    running->status = DEAD;
    tswitch();
}

//change running proc status to stop and call tswitch()
do_stop(){
    running->status = STOP;
    tswitch();
}

//let stopped process continue
do_continue(){
    PROC *p;
    int i, pid;
    char c;

    printf("Enter Process PID to continue: "); //get pid from user
    c = getc();
    pid = c - '0'; //ascii values from 0-9
    //ask for pid to be continued (needs to be fixed)
    if (pid < 0 && pid > NPROC){ //validate pid e.g. 0 < pid  < NPROC
        printf("PID Invalid!\n");
        return 0;
    }
    //find the PROC BY pid
    for (i=0; i < NPROC; i++){
        if(proc[i].pid == pid && proc[i].status == STOP){//if PROC.status is STOP, change its status to READY and enter into readyQueue 
            p = &proc[i];
            enqueue(&readyQueue, p);
        } 
    }
}

do_sleep(){
    char c;
    int event;

    printf("\nPlease enter event value: ");
    c = getc();
    event = c - '0';

    ksleep(event);
}

do_wakeup(){
    char c;
    int event;

    printf("\nPlease enter event value: ");
    c = getc();
    event = c - '0';

    kwakeup(event);
}

do_wait(){
    int pid, status;
    pid = kwait(&status); //pass in status and retreive exitCode
    if(pid == -1){
        printf("\nProcess has no children to wait for!");
    }else{
        printf("\npid=%d, status=%d\n", pid, status);
    }
    return 1;
}

do_kexit(){
    char c;
    int event;
    int value;

    printf("\nPlease enter event value: ");
    c = getc(); //events 0-9
    event = c - '0';

    value = kexit(event);

    if (value == -1){
        printf("\n Running Process has become a zombie!");
    }else{
        printf("\nP%d can't be killed while it has active children. At least P%d is active!\n", 1, value); //P1 (default process)
    }
}

int do_chpriority(){
    PROC *p;
    int i, pid, pri;
    char c, d;

    printf("\nEnter Process PID to continue: "); //get pid from user
    c = getc();
    pid = c - '0'; //ascii values from 0-9

    printf("\nEnter Priority Number to continue: ");
    d = getc();
    pri = d - '0'; //priority num from 0-9

    if (pid < 0 || pid > NPROC){ //validate pid and p e.g. 0 < pid  <= NPROC
        printf("PID Invalid! (0-9*)\n");
        return 0;
    }
    if (pri < 0 || pri > NPROC){
        printf("Priority Number Invalid! (0-9*)");
        return 0;
    }
    printf("PID entered: %d", pid);
    printf("Priority entered: %d", pri);

    //call chpriority()
    chpriority(pid, pri);
}

