author: B103040012 
date: 2022/5/1 

# Part1
1. What is the difference between a hard link and a symbolic link? Give an advantage of each one.  

__ans:__  
hard link uses the same inode of the target file, namely they're the same files, while the inode of symbolic link is different from the target file, it's just like a shortcut.  

the advantage of symbolic link is that it's able to link to another directory or file, and even link to the file on another disk, which hard link can't do.  
and when the target file of symbolic link is removed, the symbolic link won't be able to open till a file with the same target name be created again.  

the advantage of hard link is that there's no space and time cost, compared to symbolic link.  
when the target file of hard link is removed, the hard link still exists, since "it's the file itself".  

2. A disk has 4000 cylinders, each with 8 tracks of 512 blocks. A seek takes 1 msec per cylinder moved. If no attempt is made to put the blocks of a file close to each other, two blocks that are logically consecutive (i.e., follow one another in the file) will require an average seek, which takes 5 msec. If, however, the operating system makes an attempt to cluster related blocks, the mean interblock distance can be reduced to 2 cylinders and the seek time reduced to 100 microsec. How long does it take to read a 100 block file in both cases, if the rotational latency is 10 msec and the transfer time is 20 microsec per block?  

__ans:__  
disk cost time: seek + ratational latency + transfer time.  

non-optimial algorithm: (5 + 10 + 0.02) * 100 = 1502 ms.
optimial algorithm: (0.1 * 2 + 10 + 0.02) * 100 = 1022 ms.

3. Consider the following page reference string: "1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6". How many page faults would occur for the following replacement algorithms, assuming one, two, three, four, five, six, or seven frames? Remember all frames are initially empty, so your first unique pages will all cost one fault each.  
(a) LRU  
(b) Optimal  

__ans:__  
(a) LRU:  
1. 20
2. 18
3. 15
4. 10
5. 8
6. 7
7. 7

(b) Optimal:  
1. 20
2. 15
3. 11
4. 8
5. 7
6. 7
7. 7

4. Consider a file currently consisting of 100 blocks. Assume that the file control block (and the index block, in the case of indexed allocation) is already in memory. Calculate how many disk I/O operations are required for contiguous, linked, and indexed (single-level) allocation strategies, if, for one block, the following conditions hold. In the contiguous allocation case, assume that there is no room to grow in the beginning, but there is room to grow in the end. Assume that the block information to be added is stored in memory.

(a) the block is added at the beginning.  
(b) the block is added in the middle.  
(c) the block is added at the end.  
(d) the block is removed from the beginning.  
(e) the block is removed from the middle.  

__ans:__  

(a) contiguous: 201, linked: 1, indexed: 1
(b) contiguous: 101, linked: 52, indexed: 1
(c) contiguous: 1, linked: 3, indexed: 1
(d) contiguous: 198, linked: 1, indexed: 0
(e) contiguous: 98, linked: 52, indexed: 0


5. A certain computer provides its users with a virtual-memory space of $2^{32}$ bytes. The computer has $2^{18}$ bytes of physical memory. The virtual memory is implemented by paging, and the page size is 4096 bytes. A user process generates the virtual address 11123456. Explain how the system establishes the corresponding physical location. Distinguish between software and hardware operations.

__ans:__  

assume 11123456 is in decimal, the binary form of it is, 0b0000 0000 1010 1001 1011 1011 0000 0000.  
the higher 20 bits will be used as the index of page table, whose content inside is used as the physical page number, and the lower 12 bits will be used as the offset of the physical page.  
if 11123456 is in hex, the process is the same. just the binary form is different.  

distinguish between software and hardware operations:  
the hardware part handles the address translation dynamically. and the software part takes care of page IO.


