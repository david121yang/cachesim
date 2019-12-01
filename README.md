# cachesim
Program simulating cache used by MIPS operating system. To be run on terminal.

Run parameters:
./csim  
  number of sets in the cache (a positive power-of-2)  
  number of blocks in each set (a positive power-of-2)  
  number of bytes in each block (a positive power-of-2, at least 4)  
  write-allocate or no-write-allocate  
  write-through or write-back  
  lru (least-recently-used) or fifo evictions  
followed by trace file in stdin.  

Example run command:
./csim 256 4 16 write-allocate write-back fifo < gcc.trace
