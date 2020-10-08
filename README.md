## Mutual Exclusion and Condition Variables in Linux

### GWU CSCI 3411 - Fall 2020 - Lab 6

---

## Topics

1.  Introduction

2.  Exercises

3.  Discussion

4.  Linux multi-threading, mutual exclusion, and condition variable functions

# 1. Introduction

In Lab 5, we practiced multi-threading with shared memory and mutual exclusion in Linux using `pthreads`. Mutual exclusion protects access to shared memory, but does not guarantee optimal usage of processing resources. Using mutual exclusion alone produces a form of _busy waiting_, so threads end up spinning and consume processing resources when other threads or processes could use the processor instead. Condition variables allow for threads to use _block waiting_ instead.

In this lab, we will review pthread basics, study the difference between busy waiting and block waiting. We will implement a simple project twice, using mutual exclusion and using mutual exclusion with condition variables. At the end of lab, we'll consider parts of the POSIX Threads library that we haven't formally covered.

_Note: Each Exercise is followed by a set of Questions. Read through the Exercise and the Questions and discuss the questions with your lab mates before attempting to implement a solution. In your solution, try to find a way to examine the questions empirically._

## 1.1 Install additional documentation

As with last week, run the following command to have access to missing manual pages

```sh
sudo apt-get install glibc-doc manpages-posix manpages-posix-dev
```

## 1.2 Review Basic POSIX Thread APIs

At this point, you've used threads in several labs and exercises. Take a moment and review the following with your group.

- pthreads(7)
- pthread_create(3)
- pthread_exit(3)
- pthread_join (3)

Discuss the following with your group

- How do you create a thread with default arguments?
- How does one pass multiple arguments to a pthread?
- How can you return a return code from a pthread without allocating it on the heap?
- What happens if you call exit() from a thread instead of pthread_exit()?
- What happens if you call fork() from a thread?

# 2. Exercises

π can be approximated using a monte carlo approximation by randomly sampling a set of points within a square and then computing the ratio between points inside the unit circle and points inside the square. If random points are generated inside the upper left cartesian quadrant, where x and y are in the range [0,1], π is approximated by four times the ratio between the number of points inside the unit circle and the number of points in the overall set of samples. Note that as the number of samples increases, the accuracy of the π estimation increases.

In the repository for this lab, `circle.h` and `circle.c` provide all of the structures and functions necessary to compute π using the Monte Carlo approach. The repository also contains a `Makefile` and two skeleton files, `ex1.c` and `ex2.c`.

## 2.1 Exercise 1

These APIs should be familiar at this point:

- pthread_mutex_init(3)
- pthread_mutex_lock(3)
- pthread_mutex_trylock(3)
- pthread_mutex_unlock(3)
- pthread_mutex_destroy(3)

Discuss the following with your group to validate understand and refer the manual as needed.

- Does a mutex sleep or spin?
- What is trylock used for?

Use multi-threading and the mutex API to compute the monte carlo approximation of π. Implement this exercise in `ex1.c`

Each worker thread will sample a point and determine whether or not the sample is inside the unit circle. The main thread will accumulate a total count of samples and a count of samples that are inside the unit circle. _Note: The main thread does not need to know the point itself as the main thread only needs to know whether a sample is inside or outside the unit circle._

To communicate between the worker threads and the main thread, shared memory is required. The information shared will be encoded using a very simple scheme, and all threads will communicate through a single integer variable. The variable may have the values of -1, 0, and 1. If the value of the variable is -1, the main thread is waiting for a sample. If the value of the variable is 0, a worker thread has determined a sample to be outside the unit circle. If the value of the variable is 1, a worker thread has determined a sample to be inside the unit circle.

Your code should support the ability to change the number of worker threads easily and the ability to change the total number of samples easily. Using a `#define` is a simple way to control these parameters and your thread creation code should use a loop.

Hints:

- Spend time identifying the critical section
- You will need to use some form of busy waiting to successfully implement this exercise.
- Does a worker need to hold the lock when calling `circle()`?

When testing your code, use two threads and a sample size of 10. Once your program works correctly, run your program to compute π using 1 million samples with 2 threads, 4 threads, and 40 threads. While the program is running, look at CPU usage by opening `top` in a second terminal window to get an idea of how much CPU the program is using. If the program completes too quickly for it to show up in top, increase the sample count.

### 2.1.1 Questions

System design involves tradeoffs and there is typically no clear one size fits all policy.

- Does this solution block or spin? Which lines do this blocking or spinning?
- What performance differences do you note between 2, 4, and 40 threads? Why do you suppose these differences exist?
- How would these tradeoffs differ if context switches took 1/10th the time? 10x the time?
- How would these tradeoffs differ if the calculation in circle.c took 10x the time, 100x the time?

## 2.2 Exercise 2

In this exercise, you will extend the program developed in Exercise 1 to use condition variables instead of mutexes. Copy your existing solution into `ex2.c`

You should have noticed in the last exercise that both the main thread and the workers had to busy loop on the condition variable. This meant that your workers and main thread never slept and consumed all available CPU.

On my 16 core machine, top showed the following:

```
top - 21:08:39 up 12:13,  0 users,  load average: 3.87, 1.48, 1.72
Tasks:  40 total,   2 running,  38 sleeping,   0 stopped,   0 zombie
%Cpu(s): 30.4 us, 47.0 sy,  0.0 ni, 22.6 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem : 26212536 total, 21697672 free,  2797560 used,  1717304 buff/cache
KiB Swap:  7340032 total,  7340032 free,        0 used. 23006224 avail Mem

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
16039 sean      20   0  129632    792    712 R  1512  0.0   1:19.72 ex1
    1 root      20   0    1180    828    512 S   0.0  0.0   0:00.14 init
   11 root      20   0     892     76     16 S   0.0  0.0   0:00.00 init
   12 root      20   0     892     76     16 S   0.0  0.0   0:00.00 init
   13 sean      20   0    4628    852    784 S   0.0  0.0   0:00.00 sh
   14 sean      20   0    4628    880    812 S   0.0  0.0   0:00.00 sh
   19 sean      20   0    4628    768    700 S   0.0  0.0   0:00.00 sh
   21 sean      20   0 1048764 127204  31100 S   0.0  0.5   0:33.18 node
   48 sean      20   0 1243612 403504  27912 S   0.0  1.5   0:29.18 node
  269 sean      20   0 1413508 219520  33588 S   0.0  0.8   0:37.56 node
  286 sean      20   0   23796   5916   3560 S   0.0  0.0   0:00.02 bash
  494 sean      20   0  789104  96312  28948 S   0.0  0.4   0:03.04 node
  605 sean      20   0 2302220 526464   9804 S   0.0  2.0   0:19.78 cpptools
```

The fact that ex1 consumed 1,512% means that I only had 88% of one core remaining to run everything else on my computer!

Not good! And most of this was just threads running a spin loop.

Condition variables allow you to adopt a similar pattern, but actually block and wakeup threads based on a condition. This eliminates extra work spinning, but it adds system overheads for threads sleeping and waking up.

Before modifying your code, since condition variables are new, split up and have each group member read one of these man pages. When all members are done, each member should give a 1 minute summary of the API.

**Additionally, as a group, come up with 2 fun facts about condition variables and post them to the 2pm/4pm Discord channel**

Relevant manual pages:

- pthread_cond_init(3)
- pthread_cond_signal(3)
- pthread_cond_broadcast(3)
- pthread_cond_wait(3)
- pthread_cond_destroy(3)

Now that you've reviewed the APIs, the following design hints should make sense.

Hints:

1. You are adding two condition variables: One is for the producers and one is for the consumers (in our case, just the main thread)
2. You do not have to add any new mutexes. Both conditions relate to the same critical section and thus both use the existing mutex.
3. The workers should sleep on the wait queue of the producer condition variable. The main thread should signal the producer condition variable and then wait on the consumer condition variable. After the worker writes the value to var, it should signal the consumer condition variable
4. Because there are multiple worker threads, you might be tempted to use `pthread_cond_broadcast` to wake up all workers when var is `WAITING_FOR_SAMPLE`, but realistically this probably causes a "thundering herd" problem. You probably are better off using `pthread_cond_signal` in both directions.

### 2.2.1 Questions

By blocking with condition variables, we are \*targeting\* which process to take the mutex next. This should remove overhead from spinning. How much overhead from spinning is removed?

While the program is running, look at CPU usage by opening `top` in a second terminal window. How much CPU is this program using? How does this differ from the last exercise?

In the case of my 16-core system, top resembled the following.

```
top - 21:37:44 up 12:42,  0 users,  load average: 0.40, 0.76, 1.09
Tasks:  40 total,   2 running,  38 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.4 us,  5.1 sy,  0.0 ni, 94.4 id,  0.0 wa,  0.0 hi,  0.1 si,  0.0 st
KiB Mem : 26212536 total, 21500240 free,  2981764 used,  1730532 buff/cache
KiB Swap:  7340032 total,  7340032 free,        0 used. 22822016 avail Mem

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
19997 sean      20   0  129632    796    716 R 186.0  0.0   0:09.33 ex2
10116 sean      20   0 1321800 217316  34116 S   0.7  0.8   3:25.44 node
    1 root      20   0    1180    828    512 S   0.0  0.0   0:00.14 init
   11 root      20   0     892     76     16 S   0.0  0.0   0:00.00 init
   12 root      20   0     892     76     16 S   0.0  0.0   0:00.00 init
   13 sean      20   0    4628    852    784 S   0.0  0.0   0:00.00 sh
   14 sean      20   0    4628    880    812 S   0.0  0.0   0:00.00 sh
   19 sean      20   0    4628    768    700 S   0.0  0.0   0:00.00 sh
   21 sean      20   0 1047856 125892  31100 S   0.0  0.5   0:36.23 node
```

186% is a substantial decrease from 1,512%! However, running the simulation was noticeably slower due to the cost of threads sleeping and waking up. In particular, the main thread sleeps and wakes up in exceptionally short bursts. As such, the main thread is bottlenecking parallelism, and most of our workers just sleep all the time.

# 3 Discussion

Thread pools are collections of threads that take an argument from a main coordinator thread and compute on it. Arguments might be webpage requests, simulation requests, etc.

- What are the tradeoffs between sleeping and spinning?
- Which is better for latency? throughput? power efficiency?
- How would our system change if our workers could write to a vector of cells equal to the number of workers?
- How would our system change if our workers could write to a ring buffer that could store 1000 values?

# 4 Looking at other parts of the POSIX Threads Library

We are wrapping up our investigation of POSIX threads. In the remaining time, split up your group and direct each individual to investigate one of the four areas below. Relate the APIs to what we've covered in class. When everyone is ready, round robin around your groups presenting a summary of your findings and discussing the points listed below.

## Spin Locks

- pthread_spin_destroy (3) - initialize or destroy a spin lock
- pthread_spin_init (3) - initialize or destroy a spin lock
- pthread_spin_lock (3) - lock and unlock a spin lock
- pthread_spin_trylock (3) - lock and unlock a spin lock
- pthread_spin_unlock (3) - lock and unlock a spin lock

### Discussion Points

- What are the tradeoffs between spinlocks and mutexes?
- What is the difference between `pthread_spin_trylock` and `pthread_spin_unlock`?

## Read Write Locks

- pthread_rwlock_destroy (3posix) - destroy and initialize a read-write lock object
- pthread_rwlock_rdlock (3posix) - lock a read-write lock object for reading
- pthread_rwlock_wrlock (3posix) - lock a read-write lock object for writing
- pthread_rwlock_tryrdlock (3posix) - lock a read-write lock object for reading
- pthread_rwlock_trywrlock (3posix) - lock a read-write lock object for writing
- pthread_rwlock_unlock (3posix) - unlock a read-write lock object

### Discussion Points

- What is the difference between a read/write lock and a normal lock?
- What sort of software systems would benefit from this design?
- Do pthread read/write locks protect against writer starvation? Why or why not?

## Barrier

- pthread_barrier_destroy (3posix) - destroy and initialize a barrier object
- pthread_barrier_wait (3posix) - synchronize at a barrier

### Discussion Points

- What is a barrier?
- What are possible use cases for such a thing?
- Where are the rest of the man pages for this API? [Hint](https://www.kernel.org/doc/man-pages/missing_pages.html)

## Cancellation

- pthread_cancel (3) - send a cancellation request to a thread
- pthread_cleanup_pop (3) - push and pop thread cancellation clean-up handlers
- pthread_cleanup_push (3) - push and pop thread cancellation clean-up handlers
- pthread_setcancelstate (3) - set cancelability state and type
- pthread_setcanceltype (3) - set cancelability state and type
- pthread_testcancel (3) - request delivery of any pending cancellation request

### Discussion Points

- How is cancelling a thread like "killing" a process? How is it different?
- Can a thread be cancelled at any time or can this only happen during certain points? What APIs affect this?
- How might cancellation be handled fully synchronously?

_fin_
