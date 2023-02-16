In this project, you will implement a hash table data structure to emulate virtual memory

Understanding Virtual Memory

Memory is an array of bytes in the computer. This linear data structure is broken up into smaller chunks known as “pages”. When a process runs on a modern operating system, it accesses memory in such a way that it appears to be the only process running on the computer. The operating system achieves this by mapping pointers that the process wants to access (the “virtual” pointers) to different memory addresses in physical memory (the “physical” pointers). As an example, virtual pointers may lie in the range (0x00 to 0x100) while the corresponding physical memory is in the range (0x2343 to 0x2443). Such a mapping process should be fast, otherwise the operating system spends most of its time looking up addresses. The operating system achieves this using hashing. Each mapping process is given a unique integer key known as its “Process ID” (PID). This is used as the hash key in this project. This key maps to the location of an object that contains the start of the memory page allocated to that process. The virtual address is then used as the offset from the start of that page. For example, a process whose physical pointers begin at 0x2343 attempting to access virtual address 0x50 would be given access to memory address 0x2343 + 0x501.
You will likely want to create a class that represents a process. This should contain, at a minimum, the PID and the start address for the physical memory page. Then, you use your hash function to store these objects in an appropriate data structure. The process for looking up physical memory is to use the PID to get the page address. Hashing the addresses directly via the PID is possible but harder (since you need to ensure that collisions do not overwrite memory), but you may do so if you wish.
In this project, memory will be represented by an array of integers of size N. Pages are contiguous blocks of memory within this array of size P. The size of the hash table then is 𝑚 = 𝑁. For this project, N and P are chosen in such a way that m is
𝑃
an integer power of 2 and N is an integer multiple of P. You may assume that virtual pointers for all processes begin at virtual address 0 and end at P-1. When all 𝑚 blocks are allocated, the hash table is said to be “full”.

Sample Input 1:
```
OPEN
M 64 8
INSERT 3234234
SEARCH 3234234
SEARCH 12321
WRITE 3234234 1 50
READ 3234234 1
END
```
Sample Output 1:
```
success
success
found 3234234 in 2
not found
success
1 50
```
Sample Input 2:
```
ORDERED
M 16 8
INSERT 2
INSERT 10
SEARCH 10
WRITE 2 0 129
READ 2 0
INSERT 3
PRINT 1
PRINT 0
END
```
Sample Output 2:
```
success
success
failure
success
not found
failure 
failure
```
