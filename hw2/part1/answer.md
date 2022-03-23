author: B103040012
date: 2022/3/23

# Part1
1. Consider a computer that does not have a TEST AND SET LOCK instruction but does have aninstruction to swap the contents of a register and a memory word in a single indivisible action.  
Can that beused to write a routine _enter\_region_ such as the one found in Fig. 2–12.  

__answer:__  
pesudo based on MIPS:  
```
ENTER_REGION:
	addiu $REG, $zero, 1 # move 1 to $REG
	swap $REG, $LOCK     # the assumption instruction
	bne $REG, $zero, ENTER_REGION # jump back to ENTER_REGION if the original value of LOCK isn't 0

	jr $ra
```

actually, x86 has a instruction called ```xchg``` which is just like the assumption instruction.  

2. Measurements of a certain system have shown that the average process runs for a time $T$ beforeb locking on I/O. A process switch requires a time $S$, which is effectively wasted (overhead). For round-robin scheduling with quantum $Q$, give a formula for the CPU efficiency (i.e., the useful CPU time divided by the total CPU time) for each of the following:  
(a) $Q=\infty$  
(b) $Q>T$  
(c) $S<Q<T$  
(d) $Q=S$  
(e) $Q$ nearly $0$  

__answer:__  
(a) since $Q$ is unlimited, a process will run as long as it can till blocked by IO. when blocked by IO, CPU switch to next process, and so on.  
so the efficiency is $\frac{T}{T+S}$  

(b) same as the last situation. the efficiency is $\frac{T}{T+S}$  

(c) a process now have to go through $\lceil\frac{T}{Q}\rceil$ times of switch to finish its task.  
so the efficiency is $\frac{T}{T+\lceil\frac{T}{Q}\rceil S}$

(d) use the formula derived above, let $Q=S$, $T=pS+q,q<S$.  
$\frac{T}{T+\lceil\frac{T}{S}\rceil S}\iff\frac{T}{T+pS+\lceil\frac{q}{S}\rceil S}$, so the efficiency is  
$\begin{cases}\frac12, & S\mid T\\\frac{T}{T+(\lfloor\frac{T}{S}\rfloor+1)S}, & \text{else}\end{cases}$

(e) $\lim_{Q\to0} \frac{T}{T+\lceil\frac{T}{Q}\rceil S}=0$, so the efficiency is $0$

3. Consider the interprocess-communication scheme where mailboxes are used. Suppose a process P wants to wait for two messages, one from mailbox A and one from mailbox B. What sequence of _send_ and _receive_ should it execute so that the messages can be received in any order?  

__answer:__  
assume _receive_ is implemented as ```void *receive(void* mboxWithMsg_s)``` where the parameter ```mboxWithMsg_s``` is a struct containing target mailbox and message being received.

we can using pthread to achieve the request:  

```c
pthread_t mboxThdA, mboxThdB;

pthread_create(&mboxThdA, NULL, receive, (void *)&msgA);
pthread_create(&mboxThdB, NULL, receive, (void *)&msgB);

pthread_join(mboxThdA, NULL);
pthread_join(mboxThdB, NULL);
```

**note that the order doesn't depend on send (in my answer)*  

4. 
```
A = 1
B = 1
C = 2
D = 2
```
![](https://i.imgur.com/Dfk3mvn.jpg)
we can only determine the value of A, B, C, D. any order can be possible.  
* *however, by comparing the costing time, we can know the probaly order.  
since P1 is after P fork() which is time-consuming, P1 will be probaly executed after that. but notice that P1 also fork() at first, so P2 will probaly be executed after that, and so on.  
so the probaly order: P fork() -> P1 fork() -> P2 fork() -> P3 pthread -> P printf() -> P1 printf() -> P2 pthread() -> P3 printf() -> P2 printf()
so the order may be
```
A = 1
B = 1
D = 2
C = 2
```
- *it's based on round-robin scheduling.  
- *i test the code on my computer, and always get the order: A, B, C, D. maybe linux put parent and child process in different priority?  
2022/2/25 update: no, linux doesn't put parent and child process in different priority. if put some time-consuming instruction in a process, it might be executed later.  

