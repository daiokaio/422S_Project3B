/*Team David, Joey and Edwin
  CS 422S - Operating Systems
 */

 #include "types.h"
 #include "stat.h"
 #include "user.h"
 #include "fs.h"
 #include "param.h"
 #include "fcntl.h"
 #include "syscall.h"
 #include "traps.h"
 #include "threads.h"
 #include "x86.h"

// *** This is the threads library ****

//Function call that creates the thread
int thread_create(void(*fcn)(*void), void *arg){

  //There might be an instance where the initial values of the threads
  //have not been initialized, so lets make sure our table is initialized
  if(thread_table_init != 0){
    int x;
    for(x=0; x < NPROC; x++){
      threads[x].pid = 0;
      threads[x].ustack = 0;
      threads[x].used = 0;
    }
    thread_table_init = 1;
  }

  //init the pid, and allocate space
  int pid;
  void * newStack = malloc(KSTACKSIZE);

  //Call system call clone, and add the threads to the list, return the pid
  pid = clone((void *) fcn, (void *)arg, (void*) newStack);

  //Lets add it to our table;
  int x;
  for(x=0, x < NPROC; x++){ //For the number of processes we have
    if(threads[x].used == 0){
      //If this thread is not being used, then get the address from the pid
      //and point to the user stack, and mark it as used.
      threads[x].pid = *pid;
      threads[x].ustack = ustack;
      threads[x].used = 1;
    }
  }

  return pid;
}

//Function call that will JOIN the threads
int thread_join(void){
  int x,y;
  for(x=0; x < NPROC; x++){
    if(threads[x].used == 1){
      y = join(&threads[x].ustack);

      //If the ustack from joining is greater than 0, then remove this thread.
      if(y > 0){
        int y;
        void * ustack;
        for(y=0; y < NPROC; y++){
          if(threads[y].used && threads[y].pid == *p){
            //If this thread is being used, and it matches its pid, then remove it.
            ustack = threads[y].ustack;
            free(ustack); //Free allocation
            threads[y].pid = 0;
            threads[y].ustack = 0;
            threads[y].used = 0;
          }
        }
        break;
      }
    }
  }
  return p;
}

/** ************************** LOCKS ARE HERE *************** */
void lock_acquire(struct _lock_t * lock){
    volatile uint x = (uint) 1;
    //Let's use the atomic function xchg to see if our lock flag is ready
    //to be acquired, wait until its acquired.
    while(xchg(&lock->flag, x) == x)
    ;
}

void lock_release(struct _lock_t * lock){
  lock->flag = 0;
}

void lock_init(struct _lock_t * lock){
  //init state of the lock is: 0 (not being used)
  lock->flag = 0;
}
