/**
 * Template for Practice Lab 7 (Main Memory: Address Translation)
 * 
 * Outline: Assume that a system has a 32-bit virtual address with a
 * 4KB (2^12) page size. The physical memory address is also a 32-bit
 * address. Consider a small program that needs only 8 pages of
 * memory. Below is the page table for this program:
 * 
 * |--------------|
 * | Page Table   |
 * |--------------|
 * | Frame Number |
 * |--------------|
 * | 6            |
 * | 4            |
 * | 3            |
 * | 7            |
 * | 0            |
 * | 1            |
 * | 2            |
 * | 5            |
 * |--------------|
 * 
 * Task: Write a C program that simulates an MMU's (memory management
 * unit's) address translation capability. To simulate a program's
 * memory address requests, use the provided text file
 * (i.e. labaddr.txt). This text file contains a sample of 20 logical
 * addresses generated for this program. Your job is to read these
 * addresses and output the following for every address:
 *   - Logical/virtual address
 *   - Corresponding page number and offset
 *   - Corresponding physical address
 * 
 * Tip: There are 3 parts to this lab. They are:
 *   1. Reading from a file.
 *   2. Given a logical/virtual address, output its page number and
 *      offset
 *   3. Given the page table and the logical address output its
 *      corresponding physical address.
 * 
 * @author Dr. Neerja Mhaskar
 * @author Jatin Chowdhary
 * @course SFW-RENG 3SH3
 * @date March 21st, 2023
 */

#include <stdlib.h> // Standard Library
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdio.h>  // Standard I/O Operations

// STEP 1 : Appropriately set the following macros:
// HINTS  : PAGES = How many pages are in the table?
//          BUFFER_SIZE = What is the buffer size for reading?
//          PAGE_SIZE = What is the size of each page in bytes?
//          OFFSET_BITS = Number of bits to represent an offset,
//                        which is the number of bits needed to
//                        represent the page size
//          OFFSET_MASK = (PAGE_SIZE - 1)
#define PAGES 256
#define BUFFER_SIZE 10
#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255
#define TLB_SIZE 16
#define FRAME_COUNT 128

typedef struct {
    int page_number;
    int frame_number;
} TLBentry;

TLBentry TLB[TLB_SIZE];

int TLB_entries = 0;
int TLB_head = 0;


// Search the TLB for entry corresponding to given page number.

int search_TLB(int page_number) {
    int i;
    for (i = 0; i < TLB_entries; i++) {
        if (TLB[(TLB_head + i) % TLB_SIZE].page_number == page_number) {
            return TLB[(TLB_head + i) % TLB_SIZE].frame_number;
        }
    }
    return -1;
}


void TLB_Add(int page_number, int frame_number) {
    if (TLB_entries < TLB_SIZE) {
        TLB[TLB_entries++] = (TLBentry) { page_number, frame_number };
    } else {
        TLB[TLB_head++] = (TLBentry) { page_number, frame_number };
        TLB_head %= TLB_SIZE;
    }
}


/**
 * Main function
 */
int main(int argc, char const *argv[]) {

    // STEP 2 : Declare, but do not initialize, the following:
    //          - page_number
    //          - frame_number
    //          - virtual_address
    //          - physical_address
    //          - offset
    // HINT   : Some of these will be unsigned int's. One of them is
    //          a regular int. To differentiate the two, think about
    //          which one is allowed to be negative and which one is
    //          not.
    int phys_memory[FRAME_COUNT];
    signed char *mmapfptr;
    unsigned page_number;
    int frame_number;
    unsigned virtual_address;
    unsigned physical_address;
    unsigned offset;
    // STEP 3 : Declare a character array called 'buffer' that can
    //          store/hold 10 characters. The size of the buffer is
    //          already defined above via a ``#define`` macro.
    // HINT   : char XYZ[size];
    
    char buff[BUFFER_SIZE];
    // STEP 4 : Declare and initialize an array, called 'page_table'
    //          to store the contents of the page table provided
    //          above. The size of the array is already defined above
    //          via a ``#define`` macro.
    int page_table[PAGES];
    for (int i = 0; i < 100; ++i){
        page_table[i] = -1;
    }

    // STEP 5 : Use the C library function ``fopen()`` to open the
    //         ``labaddr.txt`` file.
    // HINT   : FILE * name = fopen("file_name.txt", "permission");
    FILE *fptr = fopen("addresses.txt", "r");
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);
    mmapfptr = mmap(0, PAGE_SIZE * PAGES, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    // STEP 5A : What does "r" stand for? Are there other permissions
    //           we can use to access/modify the file?
    // HINT    : Refer to manual via `man fopen`
    // ANS     : The "r" stands for "read". It indicates that the file
    //           can only be read (i.e. Writing not allowed). Other
    //           permissions are "w" and "a". Both are used for
    //           writing, but "w" starts the stream at the beginning
    //           of the file, and "a" starts at the end of the file.

    // STEP 5B : Why is this a pointer? Why not just use a regular
    //           non-pointer data type?
    // ANS     : The type has to be a pointer, because the file
    //           exists somewhere else in memory. To access the
    //           contents of the file, we need to physically go to the
    //           location in memory.

    // STEP 5C : Which header file is ``fopen()`` found in?
    // ANS     : It is in ``stdio.h``

    // STEP 6 : Do a null check to ensure that the text file was
    //          opened correctly. Don't forget to return something
    //          a non-zero integer.
    
    if(fptr == NULL)
    {
        printf("file not opened correctly\n");
        return 1;
    }
    // STEP 6A : What is the point of doing a null check?
    // ANS     : 

    // STEP 6B : Why return a non-zero integer?
    // ANS     :

    // STEP 7 : Iterate through the file stream and output its
    //          contents to the terminal. Make sure you perform a
    //          NULL check to see if the stream has reached the end
    //          or not
    // HINT   : Inside the condition for the while loop, use
    //          ``fgets(A,B,C)``, where:
    //            - A = Destination (Where to output the contents?)
    //            - B = Buffer size (How many characters to read?)
    //            - C = Source (Where to read from?)
    while (fgets (buff, BUFFER_SIZE, fptr) != NULL) {
        printf("The buffer is: %s", buff);
        virtual_address = atoi(buff);
        page_number = virtual_address >> OFFSET_BITS;
        // frame_number = page_table[page_number];
        offset = virtual_address & OFFSET_MASK;
        // physical_address = (frame_number << OFFSET_BITS) | offset;
        // if page fault:
            phys_memory[oldest] = - 1
            memcpy(phys_memory + oldest, mmapfptr + PAGE_SIZE * page_number, PAGE_SIZE); //note page size = frame size
        printf("Virtual addr is %d: Page# = %d & Offset = %d.", virtual_address, page_number, offset);
        // Note: No need to include a "\n" because each line in the
        //       text file ends with a "\n".
    }

    // STEP 8 : Inside the while-loop, above, compute the following:
    //            - Virtual address (i.e. ``virtual_address``)
    //            - Page number (i.e. ``page_number``)
    //            - Frame number(i.e. ``frame_number``)
    //            - Offset (i.e. ``offset``)
    //            - Physical address (i.e. ``physical_address``)
    // HINT   : Virtual address = The addresses inside the text file
    //            are logical addresses. Logical addresses are
    //            synonmous with virtual addresses. However, it is a
    //            good idea to convert from a string to an integer
    //            before storing the address.
    //          Page number = Logical address right shifted by the
    //            offset bits
    //          Frame number = In the page table, it is the element
    //            that corresponds to page number
    //          Offset = Binary AND performed on the logical address
    //            and offset mask
    //          Physical address = Frame number left shifted by the
    //            offset bits and then a logical OR with the offset
    //            is performed on the result

    // STEP 9 : Inside the while-loop, above, print all corresponding
    //          information calculated in the step above (Step 8).

    // STEP 10 : Close the file stream via ``fclose``
    fclose(fptr);
    
    // STEP 10A : What is the relevance/importance behind closing a
    //            resource once it is no longer being used?
    // ANS      : 

    return 0;
}
 