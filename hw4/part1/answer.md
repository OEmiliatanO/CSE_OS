author: B103040012 
date: 2022/4/11 

# Part1
1. A computer has four page frames. The time of loading, time of last access, and the R and M bits for each page are as shown below (the times are in clock ticks):

| Page | Loaded | Last Reference | R   | M   |
| ---- | ------ | -------------- | --- | --- |
| 0    | 126    | 279            | 0   | 0   |
| 1    | 230    | 260            | 1   | 0   |
| 2    | 120    | 272            | 1   | 1   |
| 3    | 160    | 280            | 1   | 1   |

	a. Which page will NRU replace?  
	b. Which page will FIFO replace?  
	c. Which page will LRU replace?  
	d. Which page will second chance replace?  

__ans:__  

a. Page 0. since NRU pick from the RM bitset (0,0), (0,1), (1,0), (1,1).  
b. Page 2. since it's the oldest page.  
c. Page 1. since it's last reference is the earliest.  
d. Page 0.  

2. A small computer has 8 page frames, each containing a page. The page frames contain virtual pages A, C, G, H, B, L, N , and D in that order. Their respective load times were 18, 23, 5, 7, 32, 19, 3, and 8. Their reference bits are 1, 0, 1, 1, 0, 1, 1, and 0 and their modified bits are 1, 1, 1, 0, 0, 0, 1, and 1, respectively. Which page will the second chance page replacement algorithm replace?

__ans:__

sort them into FIFO order:  

|                | N | G | H | D | A | L | C | B |
| -------------- | - | - | - | - | - | - | - | - |
| Last Reference | 3 | 5 | 7 | 8 | 18| 19| 23| 32|
| R              | 1 | 1 | 1 | 0 | 1 | 1 | 0 | 0 |
| M              | 1 | 1 | 0 | 1 | 1 | 0 | 1 | 0 |  

so second chane replace algorithm will pick page D to replace.  

3. What is the difference between a physical address and a virtual address?  

__ans:__  

a virtual address is limited by the word length in the machine. for example, a 32-bit machine can address $0 \~ 2^{32}-1$ in virtual address, and a 64-bit machine can address $0 \~ 2^{64} - 1$ in virtual machine.  
but a physical address is limited by the real memory size. for example, assume there's a 1MB RAM, the physical address will be in $[0,2^{20}-1]$.  

a virtual address points to a virtual store position viewed by a process.  
but a physical address points to a position on a physical memory.  

4. Are there any circumstances in which clock and second chance choose different pages to replace? If so, what are they? 

__ans:__  

they are the same essentially, just represent in different way.  
so there's no circumstance where clock and second chance choose different pages to replace.  

5. A small computer has four page frames. At the first clock tick, the R bits are 0111 (page 0 is 0, the rest are 1). At subsequent clock ticks, the values are 1011, 1010, 1101, 0010, 1010, 1100, and 0001. If the aging algorithm is used with an 8-bit counter, give the values of the four counters after the last ticks.  

__ans:__  

| pages  | counter  |
| ------ | -------- |
| page 0 | 01101110 |
| page 1 | 01001001 |
| page 2 | 00110111 |
| page 3 | 10001011 |


