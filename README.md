# Memory_management
Making custom memory management algorithms that can allocate, deallocate, and compact allocated memory

## Initilization 
User need to specify the size of memory in `bytes` at the start of the program using function ``` initialize_allocator(size, MANAGEMENT_ALGORITHM);```
* User can choose between 3 types of `MANAGEMENT_ALGORITHMS`
    * `BEST_FIT` : satisfies the allocation request from the available memory block that at least as large as the requested size and that results in the smallest remainder fragment.
    * `FIRST_FIT`: satisfies the allocation request from the first available memory block (from left) that is at least as large as the requested size.
    * `WORST_FIT`: satisfies the allocation request from the available memory block that at least as large as the requested size and that results in the largest remainder fragment.

## Allocating Memory
User need to use `kalloc(_size_);` function to allocate memory. This function returns a pointer to the allocated block of size `_size_`. If allocation fails then this function returns `NULL`. This function behaves similar to `malloc(_size_);`

## Deallocating Memory
User need to use `kfree(_Ptr_);` function to take away the ownership of the block pointed by `_Ptr_`.  This function behaves similar to `free(_Ptr_);`

#### Example Code 
```
int* p = (int*) kalloc(sizeof(int));
if(p != NULL) 
{
    // do_some_work(p);
    kfree(p);
}
```
## Compacting Memory
Continuous allocation and deallocation results in fragmentation of the memory. This can be solved by the function `compact_allocation( _before_, _after_);`. This function relocated all the data and moves all the fragmeneted free blocks to the end of the memory. `_before_` and `_after_` are arrays of `void*` pointers. `compact_allocation` functions writes the before and after addresses of the relocated blocks into these arrays. It is users job to allocate these arrays enough memory to hold all the addresses of the allocated blocks that have been relocated. `compact_allocation` function returns an `int` value that represents the number of blocks that have been relocated.
```
void* before[100];
void* after[100]; // in this example, total pointers is less than 100
int count = compact_allocation(before, after);
for(int i=0; i<count; ++i) 
{
// Update pointers
}
```

## Other Functionalities
User can use `print_statistics();` to print out statistics about the current state of memory. Output would look as following:
```
Allocated size = 40
Allocated chunks = 10
Free size = 60
Free chunks = 1
Largest free chunk size = 60
Smallest free chunk size = 60
```
