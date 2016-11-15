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

struct user_thread {
    int pid;
    int used;
    void * ustack;
};

//We are going initialize the table, and create 64 of them.
int thread_table_init = 0;
struct user_threads threads[64]; 
