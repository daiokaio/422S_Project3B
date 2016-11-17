# Partners
1. David Ayeke
2. Edwin Munguia
3. Joseph Woodson

# Project 3B: XV6 Threading

# Code Explanation
Following with what fork() and wait() do, clone() and join() make use of most of that
architecture to create a threadable function that makes use of XV6's lock system.

#clone()
Located in proc.c, clone() will direct the thread to point to the user stack, receive the fcn
to the appropriate register and pass the arguments of that fcn as well. The PC is also given a
fake address of 0xFFFFFFFF. It finally sets the state of that thread's PID to RUNNABLE while
using the acquire() and release() lock functions.

#join()
Located in proc.c, join() mimics wait(), while making sure that the thread will release itself
and kill the process.

#Thread Library
The threadLibrary was built in ulib.c. thread_create(), thread_join() make use of the system calls clone()
and join(). thread_create creates a new user stack, and supplies it to the clone() function. There is a
struct of threadTable array that stores all the threads we create. As the join() begins, the threads are removed
from the array.

#Test assets
The MAKEFILE was modified to add two test assets: testUserCalls.c and testThreadLib.c.

testUserCalls will directly test clone() and join() system calls by simply keeping a volatile global
counter that should be incremented by '1' by our fcn addToCounter(). It will create 10 threads, and then
join them.

testThreadLib will make use of the threadLibrary built in ulib.c and add the value of our counter to itself
10 times through a for loop.

#Qemu: Compiling yields no errors or warnings. The following XV6 files were modified:
1. MAKEFILE
2. defs.h
3. user.h
4. ulib.c
5. usys.S
6. syscall.h
7. syscall.c
8. sysproc.c
9. proc.c
10. proc.h

11. testUserCalls.c
12. testThreadLib.c
