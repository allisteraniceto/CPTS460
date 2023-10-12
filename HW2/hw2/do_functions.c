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
do_continue(int pid){
    PROC *p;
    int i;
    //ask for pid to be continued (needs to be fixed)
    if (pid > 0 && pid < NPROC){ //validate pid e.g. 0 < pid  < NPROC
        printf("PID Invalid!\n");
        return 0;
    }
    //find the PROC BY pid
    for (i=0; i < NPROC; i++){
        if(proc[i].status == STOP){//if PROC.status is STOP, change its status to READY and enter into readyQueue 
            p = &proc[i];
            enqueue(&readyQueue, p);
        } 
    }
}

