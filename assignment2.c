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
#define PHYS_MEM_SIZE FRAME_COUNT * PAGE_SIZE

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

    int page_table[PAGES];
    

    for (int i = 0; i < PAGES; ++i){
        page_table[i] = -1;
    }

    FILE *fptr = fopen("addresses.txt", "r");
    
    if(fptr == NULL)
    {
        printf("file not opened correctly\n");
        return 1;
    }

    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);
    mmapfptr = mmap(0, PAGE_SIZE * PAGES, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    // STEP 7 : Iterate through the file stream and output its
    //          contents to the terminal. Make sure you perform a
    //          NULL check to see if the stream has reached the end
    //          or not
    // HINT   : Inside the condition for the while loop, use
    //          ``fgets(A,B,C)``, where:
    //            - A = Destination (Where to output the contents?)
    //            - B = Buffer size (How many characters to read?)
    //            - C = Source (Where to read from?)
    int page_corresponding_to_frame[FRAME_COUNT];
    int phys_memory[FRAME_COUNT * PAGE_SIZE];
    int phys_memory_entries = 0;
    int phys_memory_head = 0;
    int new_loc = 0;
    int page_faults = 0;
    int addresses = 0;
    while (fgets (buff, BUFFER_SIZE, fptr) != NULL) {
        addresses++;
        
        virtual_address = atoi(buff);
        page_number = virtual_address >> OFFSET_BITS;
        offset = virtual_address & OFFSET_MASK;
        if(page_table[page_number] == -1){  // page fault occured
            page_faults++;
            if (phys_memory_entries == PHYS_MEM_SIZE) {
                page_table[page_corresponding_to_frame[phys_memory_head/256]] = -1;
                phys_memory_head += 256;
                phys_memory_entries -= 256;
            }
            new_loc = (phys_memory_head + phys_memory_entries) % PHYS_MEM_SIZE;
            memcpy(phys_memory + new_loc, mmapfptr + (PAGE_SIZE * page_number), PAGE_SIZE);
            page_table[page_number] = new_loc / 256;
            page_corresponding_to_frame[new_loc/256] = page_number;
            phys_memory_entries += 256;
        }
        physical_address = (page_table[page_number] << OFFSET_BITS) | offset;
        printf("Virtual addr is %d: Physical address = %d & Value = %d.\n", virtual_address, new_loc, phys_memory[new_loc]);
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
    munmap(mmapfptr, PAGE_SIZE * PAGES);
    printf("Total addresses = %d\n", addresses);
    printf("Page_faults = %d\n", page_faults);
    printf("TLB Hits = %d\n", 0);
    return 0;
}
 