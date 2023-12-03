/************ t.c file **********************************/
#include "structs.h"
#include "queue.h"
#include "misc.h"

// 10. Run the os!
main()
{
    printf("MTX starts in main()\n");
    initialize();      // initialize and create P0 as running
    kfork();     // P0 kfork() P1
    while(1){
        printf("P0 running\n");
      
        if (proc[1].status != READY){
	        printf("no runable process, system halts\n");            
        }
        while(!readyQueue);
        printf("P0 switch process\n");
        do_tswitch();   // P0 switch to run P1
    }
    printf("Exiting MTX!\n");
    
    printf("\nGoodbye User!\n");
}



