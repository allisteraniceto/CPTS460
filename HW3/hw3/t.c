/************ t.c file **********************************/
#include "structs.h"
#include "queue.h"
#include "misc.h"

// 10. Run the os!
main()
{
    printf("\nWelcome to the CS460 Multitasking System New User!\n");
    
    printf("initializing...");
    initialize();
    
    printf("forking...");
    kfork();
    
    printf("switching...");
    do_tswitch();
    
    printf("\nGoodbye User!\n");
}



