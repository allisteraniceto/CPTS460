#ifndef KERNEL_C
#define KERNEL_C

#include "type.h"

// function to create a process DYNAMICALLY
int kfork(char *filename)
{
    // Variables
    int j, i;
    u16 segment;
    
 
    PROC *p = get_proc(&freeList);
    if (p == 0)
    {
   
        printf("No available free procs\n");
        return 0;
    }
    

    p->status = READY; 
    p->priority = 1; 
    p->ppid = running->pid; 
    p->parent = running;
    
   
    for (j = 1; j<10; j++)
        p->kstack[SSIZE - j] = 0;
        
    p->kstack[SSIZE - 1] = (int)body; 
    p->ksp = &(p->kstack[SSIZE - 9]); 
    
    // set it up to load umode properly!
    segment = (p->pid + 1) * MTXSEG;
    load(filename, segment);
    for(i = 0; i < 13; i++)
        put_word(0,segment, segment-(i * 2));
    put_word(0x0200, segment, segment-2);
    put_word(segment, segment, segment-4);
    put_word(segment, segment, segment-22);
    put_word(segment, segment, segment-24);
    p->uss = segment;
    p->usp = segment-24;
    

    enqueue(&readyQueue, p);
    printf("Proc[%d] forked child Proc[%d] at segment %x\n", running->pid, p->pid, segment);

    return (p->pid);
}

u8 get_byte(u16 segment, u16 offset){
 	 u8 byte;
 	 setds(segment);
 	 byte = *(u8 *)offset;
 	 setds(MTXSEG);
 	 return byte;
}

int get_word(u16 segment, u16 offset){
	u16 word;
	setds(segment);
	word = *(u16 *)offset;
	setds(MTXSEG);
	return word;
}

int put_byte(u8 byte, u16 segment, u16 offset){
  	setds(segment);
  	*(u8 *)offset = byte;
  	setds(MTXSEG);
}

int put_word(u16 word, u16 segment, u16 offset){
	setds(segment);
	*(u16 *)offset = word;
	setds(MTXSEG);
}

#endif
