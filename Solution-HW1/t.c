#include "structs.h"
#include "queue.h"
#include "misc.h"

int main()
{
    color = 0x000A;
    printf("\nWelcome to the MTX Operating System\n");
    printf("initializing...");
    initialize();
    printf("P0 forks P1\n");
    kfork();
    printf("P0 switches to P1\n");
    tswitch();
    printf("\nGoodbye User!\n");
}
