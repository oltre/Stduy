#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#define NAME_SIZE 10

pid_t pid, prepid;

int loop, re, fd, MakeProc;

//////////////// File Read & Write ////////////////
void Normal_int (int * i);
int Repair_int (int * i);

void Normal_string (char * s);
int Repair_string (char * s);

void Normal_float (float * f);
int Repair_float (float * f);
///////////////////////////////////////////////////

//////////////// Process ////////////////
void Make_Process( );
void ProcessWait( );
/////////////////////////////////////////

void Transport_Signal( );
