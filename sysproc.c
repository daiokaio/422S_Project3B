/*Team David, Joey and Edwin
  CS 422S - Operating Systems
 */

#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_clone(void){
  //Init the variables;
  void * fcn = 0;
  void * arg = 0;
  void * stack = 0;

  //Let's pass the user arguments to XV6 using argint()
  if(argint(0, (int *) &fcn) < 0){
      return -1;
  }

  if(argint(1, (int *) &arg) < 0){
    return -1;
  }

  if(argint(2, (int *) &stack) < 0){
    return -1;
  }

  //Now let's return the function clone();
  return clone(fcn, arg, stack);
}

int
sys_join(void){
  //Init the stack;
  void ** stack = 0;

  //Let's pass the user arguments to XV6 using argint()
  if(argint(0, (int *) &stack) < 0){
    return -1;
  }

  return join(stack);
}
