#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "fs.h"
#include "param.h"
#include "syscall.h"
#include "traps.h"

char*
strcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, void *vsrc, int n)
{
  char *dst, *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

/**  *********************** NEW THREAD LIBRARY  ************************/
struct user_thread threads[64];
//We are going initialize the table, and create 64 of them.
int thread_table_init = 0;

//Function call that creates the thread
int thread_create(void(*fcn)(void*), void *arg){

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
  for(x=0; x < NPROC; x++){ //For the number of processes we have
    if(threads[x].used == 0){
      //If this thread is not being used, then get the address from the pid
      //and point to the user stack, and mark it as used.
      threads[x].pid = (int) &pid;
      threads[x].ustack = newStack;
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
          if(threads[y].used && threads[y].pid == (int)&y){
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
  return y;
}

/** ************************** LOCKS ARE HERE *************** */
void lock_acquire(struct __lock_t * lock){
    volatile uint x = (uint) 1;
    //Let's use the atomic function xchg to see if our lock flag is ready
    //to be acquired, wait until its acquired.
    while(xchg(&lock->flag, x) == x)
    ;
}

void lock_release(struct __lock_t * lock){
  lock->flag = 0;
}

void lock_init(struct __lock_t * lock){
  //init state of the lock is: 0 (not being used)
  lock->flag = 0;
}
