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
// #define TLB_SIZE 16



// typedef struct {
//     int page_number;
//     int frame_number;
// } TLBentry;

// TLBentry TLB[TLB_SIZE];

// int TLB_entries = 0;
// int TLB_head = 0;
// int TLBHit = 0;

//  //function prototype
// int search_TLB(int page_number);
// void TLB_Add(int page_number, int frame_number);
// void TLB_Update(int page_number, int frame_number);

// // Search the TLB for entry corresponding to given page number.

// int search_TLB(int page_number) {
//     int i;
//     for (i = 0; i < TLB_entries; i++) {
//         if (TLB[(TLB_head + i) % TLB_SIZE].page_number == page_number) {
//             TLBHit++;
//             return TLB[(TLB_head + i) % TLB_SIZE].frame_number;
//         }
//     }
//     return -2;
// }

// // Add to TLB
// void TLB_Add(int page_number, int frame_number) {
//     if (TLB_entries < TLB_SIZE) {
//         TLB[TLB_entries] = (TLBentry) { page_number, frame_number };
//         TLB_entries++;
//     } else {
//         TLB[TLB_head] = (TLBentry) { page_number, frame_number };
//         TLB_head++;
//         TLB_head %= TLB_SIZE;
//     }
// }

// // Update TLB
// void TLB_Update(int page_number, int frame_number) {
//     int i;
//     for (i = 0; i < TLB_entries; i++) {
//         if (TLB[(TLB_head + i) % TLB_SIZE].page_number == page_number) {
//             TLB[(TLB_head + i) % TLB_SIZE] = (TLBentry) { page_number, frame_number };
//             return;
//         }
//     }
    
//     TLB_Add(page_number, frame_number);
// }



// void swap(int *a, int *b) {
//     int temp = *a;
//     *a = *b;
//     *b = temp;
// }
//  //bubble-sort being used for array in ascending order
// int* sort(const int requests[], int sorted[]) {
//     memcpy(sorted, requests, INT_COUNT*sizeof(int));
//     for (int i = 0; i < INT_COUNT-1; i++) {
//         for (int j = 0; j < INT_COUNT-i-1; j++) {
//             if (sorted[j] > sorted[j+1]) {
//                 swap(&sorted[j], &sorted[j+1]);
//             }
//         }
//     }
//     return sorted;
// }

int requests[INT_COUNT];
int sortedrequests[INT_COUNT];
int init_head;
int direction_right;
signed char *mmapfptr;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
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

int SCAN(int *sortedrequests){
    int head_movements = 0;
    int curr;
    int init_head_spot;
    int i = 0;
    int dir = direction_right;
    int just_switched = 0;
    sortedrequests = sort(requests, sortedrequests);

    // find initial head spot in sorted array and store it for later
    for (int i = 0; i < INT_COUNT; i++) {
        if(sortedrequests[i] == init_head)
        {
            curr = i;
            init_head_spot = i;
        }
    }

    // calculate total head movements
    if(init_head == INT_COUNT - 1 || init_head == 0)
    {
        head_movements = abs(sortedrequests[curr] - sortedrequests[0]);
    }
    else if (direction_right == 1)
    {
        head_movements = abs(sortedrequests[curr] - 299);
        head_movements = head_movements + abs(sortedrequests[0] - 299);
    }
    else
    {
        head_movements = abs(sortedrequests[curr] - 0);
        head_movements = head_movements + abs(sortedrequests[INT_COUNT - 1] - 0);
    }

    // iterate all entries to print them
    while(i <= INT_COUNT - 1)
    {
        // direction going left starting off from head
        while(i <= INT_COUNT - 1 && dir == 0 && curr >= 0)
        {
            printf("%d", sortedrequests[curr]);
            if(i < INT_COUNT - 1)
            {
                printf(", ");
            }
            curr --;
            i ++;
            if(curr == -1)
            {
                dir = 2;
                just_switched = 1;
            }
        }

        // direction going right starting off from head
        while(i <= INT_COUNT - 1 && dir == 1 && curr <= INT_COUNT - 1)
        {
            printf("%d", sortedrequests[curr]);
            if(i < INT_COUNT - 1)
            {
                printf(", ");
            }
            curr ++;
            i ++;
            if(curr == INT_COUNT)
            {
                dir = 3;
                just_switched = 1;
            }
        }

        // direction going left after going done right
        while(i <= INT_COUNT - 1 && dir == 3 && curr >= 0)
        {   
            if(just_switched == 1)
            {
                curr = init_head_spot - 1;
                just_switched = 0;
            }
            printf("%d", sortedrequests[curr]);
            if(i < INT_COUNT - 1)
            {
                printf(", ");
            }
            curr --;
            i ++;
        }

        // direction going right after going done left
        while(i <= INT_COUNT - 1 && dir == 2 && curr <= INT_COUNT - 1)
        {
            if(just_switched == 1)
            {
                curr = init_head_spot + 1;
                just_switched = 0;
            }
            printf("%d", sortedrequests[curr]);
            if(i < INT_COUNT - 1)
            {
                printf(", ");
            }
            curr ++;
            i ++;
        }
        
    }

    return head_movements;
}

// int CSCAN(int *sortedrequests, int requests[]){
//     int head_movements = 0;
//     int curr = init_head;
//     int i = 0;
//     int j = INT_COUNT - 1;
//     int upper = 299;


//     while (i <= j) {
//         if (direction_right == 1) {
//             if (sortedrequests[i] >= curr) {
//                 head_movements += abs(sortedrequests[i] - curr);
//                 printf("%d, ", requests[i]);
//                 curr = sortedrequests[i];
//                 i++;
//             }
//             else {
//                 head_movements += upper;
//                 curr = 300;
//                 direction_right = 0;
//             }
//         }
//         else {
//             if (sortedrequests[j] <= curr) {
//                 head_movements += abs(sortedrequests[j] - curr);
//                 printf("%d, ", requests[j]); 
//                 curr = sortedrequests[j];
//                 j--;
//             }
//             else {
//                 head_movements += abs(curr);
//                 curr = 0;
//                 direction_right = 1;
//             }
//         }
        
//     }
    

//     return head_movements;
// }



int LOOK(int* sortedrequests){
    int current_position = init_head;
    int head_movements = 0;
    int i;
    if (direction_right == 1) {
        // Service sortedrequests to left of inital position
        for (i = INT_COUNT-1; i >= 0; i--) {
            if (sortedrequests[i] <= current_position) {
                printf("%d ", sortedrequests[i]);
                head_movements += current_position - sortedrequests[i];
                current_position = sortedrequests[i];
            }
        }
        // Service sortedrequests to the right of initial position
        for (i = 0; i < INT_COUNT; i++) {
            if (sortedrequests[i] > current_position) {
                printf("%d ", sortedrequests[i]);
                head_movements += sortedrequests[i] - current_position;
                current_position = sortedrequests[i];
            }
        }
    } else {
        // Service sortedrequests to the right of initial position
        for (i = 0; i < INT_COUNT; i++) {
            if (sortedrequests[i] >= current_position) {
                printf("%d ", sortedrequests[i]);
                head_movements += sortedrequests[i] - current_position;
                current_position = sortedrequests[i];
            }
        }
        // Service sortedrequests to left of initial position
        for (i = INT_COUNT-1; i >= 0; i--) {
            if (sortedrequests[i] < current_position) {
                printf("%d ", sortedrequests[i]);
                head_movements += current_position - sortedrequests[i];
                current_position = sortedrequests[i];
            }
        }
    }
    return head_movements;
}

// int CLOOK(int* sortedrequests){
//     int current_position = init_head;
//     int head_movements = 0;
//     int i = 20;

//     if (direction_right == 0) {
//         // Service sortedrequests to the left of initial position
//         for (i = INT_COUNT-1; i >= 0; i--) {
//             if (sortedrequests[i] <= current_position) {
//                 printf("%d ", sortedrequests[i]);
//                 head_movements += current_position - sortedrequests[i];
//                 current_position = sortedrequests[i];
//             }
//         }
        
//         head_movements += current_position;
//         current_position = 0;
//         for (i = 0; i < INT_COUNT; i++) {
//             if (sortedrequests[i] >= current_position) {
//                 printf("%d ", sortedrequests[i]);
//                 head_movements += sortedrequests[i] - current_position;
//                 current_position = sortedrequests[i];
//             }
//         }
//     } else {
//         // Service sortedrequests to the right of initial position
//         for (i = 0; i < INT_COUNT; i++) {
//             if (sortedrequests[i] >= current_position) {
//                 printf("%d ", sortedrequests[i]);
//                 head_movements += sortedrequests[i] - current_position;
//                 current_position = sortedrequests[i];
//             }
//         }
        
//         head_movements += 299 - current_position;
//         current_position = 299;
//         for (i = INT_COUNT-1; i >= 0; i--) {
//             if (sortedrequests[i] <= current_position) {
//                 printf("%d ", sortedrequests[i]);
//                 head_movements += current_position - sortedrequests[i];
//                 current_position = sortedrequests[i];
//             }
//         }
//     }
//     return head_movements;
// }

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

    memcpy(sortedrequests, requests, INT_COUNT*sizeof(int));
    qsort(sortedrequests, INT_COUNT, sizeof(int), cmpfunc);

    // do SCAN
    printf("\nSCAN DISK SCHEDULING ALGORITHM:\n\n");
    int scan_hm = SCAN(sortedrequests);
    printf("\n\nSCAN - Total head movements = %d\n", scan_hm);

    // printf("\nC-SCAN DISK SCHEDULING ALGORITHM:\n\n");
    // int cscan_hm = CSCAN(sortedrequests, requests);
    // printf("\n\nC-SCAN - Total head movements = %d\n", cscan_hm);

    printf("\nLOOK DISK SCHEDULING ALGORITHM:\n\n");
    int look_hm = LOOK(sortedrequests);
    printf("\n\nLOOK - Total head movements = %d\n", look_hm);

    // printf("\nC-LOOK DISK SCHEDULING ALGORITHM:\n\n");
    // int clook_hm = CLOOK(sortedrequests);
    // printf("\n\nC-LOOK- Total head movements = %d\n", clook_hm);

    return 0;
}
 
