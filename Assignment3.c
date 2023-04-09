#include <stdlib.h> // Standard Library
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdio.h>  // Standard I/O Operations

// declarations
#define BUFFER_SIZE 10
#define INT_SIZE 4 // Size of integer in bytes
#define INT_COUNT 20
#define MEMORY_SIZE INT_COUNT * INT_SIZE
// #define OFFSET_BITS 8
// #define OFFSET_MASK 255
// #define TLB_SIZE 16
// #define FRAME_COUNT 128
// #define PHYS_MEM_SIZE 32768
#define TLB_SIZE 16

int requests[INT_COUNT];
int sorted_requests[INT_COUNT];
int init_head;
int direction_right;
signed char *mmapfptr;

typedef struct {
    int page_number;
    int frame_number;
} TLBentry;

TLBentry TLB[TLB_SIZE];

int TLB_entries = 0;
int TLB_head = 0;
int TLBHit = 0;

 //function prototype
int search_TLB(int page_number);
void TLB_Add(int page_number, int frame_number);
void TLB_Update(int page_number, int frame_number);

// Search the TLB for entry corresponding to given page number.

int search_TLB(int page_number) {
    int i;
    for (i = 0; i < TLB_entries; i++) {
        if (TLB[(TLB_head + i) % TLB_SIZE].page_number == page_number) {
            TLBHit++;
            return TLB[(TLB_head + i) % TLB_SIZE].frame_number;
        }
    }
    return -2;
}

// Add to TLB
void TLB_Add(int page_number, int frame_number) {
    if (TLB_entries < TLB_SIZE) {
        TLB[TLB_entries] = (TLBentry) { page_number, frame_number };
        TLB_entries++;
    } else {
        TLB[TLB_head] = (TLBentry) { page_number, frame_number };
        TLB_head++;
        TLB_head %= TLB_SIZE;
    }
}

// Update TLB
void TLB_Update(int page_number, int frame_number) {
    int i;
    for (i = 0; i < TLB_entries; i++) {
        if (TLB[(TLB_head + i) % TLB_SIZE].page_number == page_number) {
            TLB[(TLB_head + i) % TLB_SIZE] = (TLBentry) { page_number, frame_number };
            return;
        }
    }
    
    TLB_Add(page_number, frame_number);
}

// FCFS function
int FCFS() {
    int head_movements = 0;
    for (int i = 0; i < INT_COUNT; i++) {
        printf("%d", requests[i]);
        if(i < INT_COUNT - 1)
        {
            printf(", ");
        }
        if(i == 0)
        {
            head_movements = head_movements + abs(init_head - requests[i]);
        }
        else
        {
            head_movements = head_movements + abs(requests[i] - requests[i - 1]);
        }
    }
    return head_movements;
}

int SSTF() {
    int head_movements = 0;
    int head = init_head;
    int closest_to_head_index;
    int closest_dist_to_head = 300;
    int seen[INT_COUNT] = {0};
    for (int i = 0; i < INT_COUNT; i++) {
        for (int x = 0; x < INT_COUNT; x++)
        {
            if(seen[x] == 0 && abs(head - requests[x]) < closest_dist_to_head)
            {
                closest_dist_to_head = abs(head - requests[x]);
                closest_to_head_index = x;
            }
        }
        
        seen[closest_to_head_index] = 1;
        printf("%d", requests[closest_to_head_index]);
        if(i < INT_COUNT - 1)
        {
            printf(", ");
        }
        head_movements = head_movements + closest_dist_to_head;
        closest_dist_to_head = 300;
        head = requests[closest_to_head_index];
    }
    return head_movements;
}

/**
 * Main function
 */
int main(int argc, char const *argv[]) {

    signed char *mmapfptr;
    
    int mmapfile_fd = open("request.bin", O_RDONLY);

    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    
    // read requests
    for (int i = 0; i < INT_COUNT; i++) {
        memcpy(requests + i, mmapfptr + 4*i, INT_SIZE);
    }

    munmap(mmapfptr, MEMORY_SIZE);

    // specify inital head
    init_head = atoi(argv[1]);
    
    // set direction
    if(strcmp(argv[2], "LEFT") == 0)
    {
        direction_right = 0;
    }
    else if (strcmp(argv[2], "RIGHT") == 0)
    {
        direction_right = 1;
    }
    else
    {
        printf("direction not specified correctly");
        return -1;
    }
    
    
    // initial output statements
    printf("Total requests = %d\n", INT_COUNT);
    printf("Initial Head Position: %d\n", init_head);
    printf("Direction of Head: %s\n", argv[2]);

    // do FCFS
    printf("\nFCFS DISK SCHEDULING ALGORITHM:\n\n");
    int fcfs_hm = FCFS();
    printf("\n\nFCFS - Total head movements = %d\n", fcfs_hm);

    // do SSTF
    printf("\nSSTF DISK SCHEDULING ALGORITHM:\n\n");
    int sstf_hm = SSTF();
    printf("\n\nSSTF - Total head movements = %d\n", sstf_hm);
    return 0;
}
 