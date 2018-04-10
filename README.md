Full Name: Junha Choi
Student Id: 301291870
SFU Email: junhac@sfu.ca

Reference: https://stackoverflow.com/questions/8436841/how-to-recursively-list-directories-in-c-on-linux

Part 1: Virtual Memory Questions

1.  As a user process generates the virtual address 11123456, Memory Management Uni (MMU) strats calculating offset.
    Because the page is is 4096 bytes = 2^12, when we shift 3 hex bits from the virtual memory (virtual address / 4096),
    we can find the page number which is 1112 (in Hexadecimal). The shifted-out hex bits are an offset which is 456 (in Hexademcimal).
    
    Now, MMU looks at the page table and check if that page is valid or not in Physical memory. If it is valid,
    finds which physical address frame corresponds to the page number (1112), and calculate the physical memory address 
    ( frame number * 4096 + 0100,0101,0110 ) <= (frame number * page size + offset)

    If the invalid bit is set, check if page is beyond the end of the object, and trap if so. [Otherwise, find a free frame and schdule a disk read
    to read the page into the frame. Modify the page talbe to indicate the page is available after the read is done. Restart the process at the instruction
    interrupted by the page fault]. And calculate the physical memory like above.

    The part which is covered by [ ] is the software part. Otherwise, they are done by hardware.

2.  Lets say P is the page-fault rate. Then, the average access time is:

    ( 1 - P ) * 100 nanosecond + P ( 0.3 * 8 millisecond + 0.7 * 20 millisecond)

    which should be less than or equal to 200 nanosecond.
    
    =>     (1-P) * 100 + P * ( 0.3 * 8 * 10^6 + 0.7 * 20 * 10^6) <= 200
    =>     (1-P) + P * ( 24 + 140 ) <= 2
    =>     163999 * P <= 1
    =>      P <= 1/163999
    =>      P <= (0.0006 %)

    That is, P should be lower than 0.0006 %
