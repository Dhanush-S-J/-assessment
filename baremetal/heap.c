/*Dhanush S J*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HEAP_SIZE (128 * 1024) // 128 KB heap
#define MIN_BLOCK_SIZE (1 * 1024) // minimum block size is 1 KB
#define MAX_BLOCKS 128 //total block
// Block metadata structure
typedef struct block
{
     size_t size;        // size of the block
     int free;           // 1 if the block is free, 0 if it's occupied
     struct block* next; // sointer to the next block
} block_t;

static uint8_t heap[HEAP_SIZE];   // heap memory
static block_t* free_list = NULL; // list of free blocks

void init_heap() {
     free_list = NULL; // Start with no blocks

     // create blocks in the heap[128 blocks]
     for (size_t i = 0; i < MAX_BLOCKS; i++) {
	  block_t* new_block = (block_t*)(heap + i * MIN_BLOCK_SIZE);
	  new_block->size = MIN_BLOCK_SIZE - sizeof(block_t); // set size for the block
	  new_block->free = 1; // mark as free
	  new_block->next = free_list; // link to the next block in the free list
	  free_list = new_block; // update free_list to point to the new block
     }
}


// Malloc function (finds the first free block that fits the size)
void* malloc_custom(size_t size)
{
     if (size == 0 || size > (HEAP_SIZE - sizeof(block_t))) return NULL;
     
     block_t* current = free_list;

     // Find the first suitable free block
     int len=(size/128);
     int count=0;
     while (current != NULL)
     {
	  if (current->free && current->size >= size) 
	  {
	       while(count++<=len)  //to return preffered block of memory
	       {
		    current->free = 0; // mark the block as used
		    current = current->next;
	       }

	       return (void*)((uint8_t*)current + sizeof(block_t)); // return pointer to the block data
	  }
	  count=0;
     }

     // No suitable block found
     return NULL;
}

// Free function
void free_custom(void* ptr) 
{
     if (!ptr) return;

     block_t* block = (block_t*)((uint8_t*)ptr - sizeof(block_t)); // get the block metadata
     block->free = 1; // mark the block as free
}


int main() 
{
     init_heap();

     void* ptr1 = malloc_custom(128 * sizeof(int));   // allocates 1 KB block
     void* ptr2 = malloc_custom(1000 * sizeof(uint8_t)); // allocates 1 KB block
     void* ptr3 = malloc_custom(128 * 8 * 1024);      // request whole heap



     free_custom(ptr1);  // frees the 1 KB block
     free_custom(ptr2);  // frees the other 1 KB block
     free_custom(ptr3);  // frees enire block memory



     return 0;
}

/*    Assessment expilnation

     * initialize structure.
*initialized heap of size(128*1024)=128 blocks of memory
* in init_heap() function we need make sure that 128 blocks are created
->each block of size 1204 bytes
->make sure each free member of block should be initialized to 1 indicating its free block.
->each block memory is allocated based on minusing structure members as it takes extra size in block
->each block is adding to list by insert first.
*in malloc() function
->comparing size must be within rage of allocated memory.
->allocate requested memory and make free as 0,so while next allocation that memory cant be used.
->at last free allocated block of memory
*/






