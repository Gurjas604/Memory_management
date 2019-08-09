# Memory_management
Making custom memory management algorithms that can allocate, deallocate, and compact allocated memory

## Initilization 
User need to specify the size of memory in `bytes` at the start of the program using function ``` initialize_allocator(size, MANAGEMENT_ALGORITHM);```
* User can choose between 3 types of `MANAGEMENT_ALGORITMS`
    * `BEST_FIT` : satisfies the allocation request from the available memory block that at least as large as the requested size and that results in the smallest remainder fragment.
    * `FIRST_FIT`: satisfies the allocation request from the first available memory block (from left) that is at least as large as the requested size.
    * `WORST_FIT`: satisfies the allocation request from the available memory block that at least as large as the requested size and that results in the largest remainder fragment.

## Allocating Memory
User need to use `kalloc(_size_);` function to allocate memory. This function returns a pointer to the allocated block of size `_size_`

#### Example Code 
```
int* p = (int*) kalloc(sizeof(int));
if(p != NULL) 
{
    // do_some_work(p);
    kfree(p);
}
```


