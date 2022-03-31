1. Explain what memory-mapped I/O is and how it works.  

ans:  
memory-mapped I/O is a method that use the same address space to address both memory and I/O devices.  
it maps the memory and registers of the device to a address value. if use this method, the OS can treat the device as a file.

2. Explain what DMA is and how it works.  

ans:  
DMA is scheme which relieve the burden of CPU when doing I/O operation.  
when access a device, CPU will waste lots of time for the entire duration of the read or write operation.  
but with DMA, CPU first initiates the transfer, and then let DMA controller deal with all the I/O issue.  
before the interrupt from the DMA controller(which means the transfer is done), the CPU can perform with other operation.  
so that, the waiting time won't be wasted.

3. Consider the following set of processes, with the length of the CPU-burst time given in milliseconds:  
| Process | Brust Time | Priority |
| ------- | ---------- | -------- |
| $P_1$   | 8 | 4 |
| $P_2$   | 1 | 1 |
| $P_3$   | 2 | 3 |
| $P_4$   | 1 | 5 |
| $P_5$   | 6 | 2 |

The processes are assumed to have arrived in the order $P_1 , P_2 , P_3 , P_4 , P_5$, all at time $0$.  

ans:  

(a)


4. A UNIX process has two parts—the user part and the kernel part. Is the kernel part like a subroutine and a coroutine? Why?  

ans:  
more like a subroutine, since when the user process require a kernel service, the kernel space starts at the same place.  
however, a coroutine can stop and resume its job at the same place.
