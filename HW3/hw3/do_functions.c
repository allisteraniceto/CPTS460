#include "misc.h"
#include "structs.h"


int do_tswitch(){
    if(running->time > 0){ //if there is leftover cpu time
        current_time = running->time; //use current_time to add to next running process
    }
    running->time = 5 + current_time; //default 5 time quantum + any leftover
    tswitch();
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

    if (pid < 0 || pid > NPROC){ //validate pid and p e.g. 0 < pid  < NPROC
        printf("PID Invalid!\n");
        return 0;
    }
    if (pri < 0 || pri > NPROC){
        printf("Priority Number Invalid!");
        return 0;
    }

    //call chpriority()
    chpriority(pid, pri);
}

