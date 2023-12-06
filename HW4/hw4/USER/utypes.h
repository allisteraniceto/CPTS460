#ifndef UTYPES_H
#define UTYPES_H

//////////////////////////////////////// DEFINITIONS ////////////////////////////////////////

#define NULL 0     // defines the value for null.
#define null 0     // defines the value for null (alt-form).

#define INPUTBUFFER 64

// BOOL DEFINITION
typedef enum { false, true } bool;

// COMMAND TABLE
typedef struct command_table{
	char *name;
	char *command;
	int (*f)();
	char *help;
} COMMANDTABLE;

//////////////////////////////////////// VARIABLES ////////////////////////////////////////
////////// Global Variables  //////////
int color;

//////////////////////////////////////// FUNCTIONS ////////////////////////////////////////
////////// ucode.c //////////
int show_menu();
int help();
int find_cmd(char *name);
int getpid();
int ps();
int chname();
int kfork();
int kswitch();
int wait();
int geti();
int exit();
int _exit(int exitValue);
int getc();
int putc(char c);
int invalid(char *name);

//////////////////////////////////////// COMMAND TABLE ////////////////////////////////////////
COMMANDTABLE commands[] = {
	{ "Get Process ID", "getpid", getpid, "Returns the proc's pid." },
	{ "Print Status", "ps", ps, "Print the status info of the proc." },
	{ "Change Name", "chname", chname, "Changes the proc's name." },
	{ "Fork", "kfork", kfork, "Forks a child." },
	{ "Switch", "switch", kswitch, "Switch processes." },
	{ "Wait", "wait", wait, "Causes proc to wait for a zombie child." },
	{ "Exit", "exit", exit, "Kill the proc." },
	{ "Help", "help", help, "Shows the help menu." },
	{0, 0, 0, 0}
};

#endif

#include "ucode.c"
