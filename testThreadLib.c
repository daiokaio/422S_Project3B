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
void printWord(void * number){
  *(int*) number = *(int*)number + (*(int*)number);
  printf(1, "The Word is: %x\n", *((int *) number));
  exit();
}

int
main(int argc, char *argv[])
{
  int counter = 1;
 printf(1, "Testing Thread Library:\n");

 int x;
 for(x =0; x < 10; x++){
   thread_create((void *)&printWord, &counter);
 }

 int y;
 for(y =0; y < 10; x++){
   thread_join();
 }

 printf(1, "Finished Joined.\n");
 exit();
 return 0;
}
