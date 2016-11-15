/*Team David, Joey and Edwin
  CS 422S - Operating Systems
 */

#include "types.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

//Our global counter
int globalCounter = 0;

void addToCounter(){
  globalCounter++;
  printf(1, "counter is: %x\n", globalCounter);
  exit();
}

int
main(int argc, char *argv[])
{

 printf(1, "Testing Thread Library:\n");

 //thread_create((void *)&addToCounter, &globalCounter);

 //thread_join();
 printf(1, "Finished Joined.\n");
 exit();
 // return 0;
}
