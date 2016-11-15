/*Team David, Joey and Edwin
  CS 422S - Operating Systems
 */

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

volatile int globalCounter = 0;

void addToCounter(){
  globalCounter++;
  printf(1, "counter is: %x\n", globalCounter);
  exit();
}

int
main(int argc, char *argv[])
{
 int pid;
 void ** stack = malloc(4096);

 printf(1, "Running testUserCalls:\n");
 globalCounter++;
 printf(1, "before cloning counter is: %x\n", globalCounter);

 int x;
 for(x=0; x<10; x++){
   pid = clone((void *) &addToCounter, (void *) &globalCounter, (void *) *stack);

   printf(1, "user pid: %d\n", pid);

 }

 printf(1, "join pid %d\n", join((void **) stack));
 globalCounter++;
 printf(1, "joined\n");

 printf(1, "The globalCounter should be 12 (C): and it is = %x\n", globalCounter);
 exit();
 return 0;
}
