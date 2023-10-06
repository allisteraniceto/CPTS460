#include "misc.h"
#include "structs.h"


int do_tswitch(){
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
void do_exit(){
    running->status = DEAD;
    tswitch();
}

//change running proc status to stop and call tswitch()
void do_stop(){
    running->status = STOP;
    tswitch();
}

//let stopped process continue
do_continue(PROC *p){
    //ask for pid to be continued
    if (p->pid > 0 && p->pid < NPROC){ //validate pid e.g. 0 < pid  < NPROC
        return 0;
    }
    //find the PROC BY pid
    if(p->status == STOP){//if PROC.status is STOP, change its status to READY and enter into readyQueue 
        p->status = READY;
        enqueue(&readyQueue, p);
    } 
}

