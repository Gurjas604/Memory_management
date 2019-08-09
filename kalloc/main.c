#include <stdio.h>
#include "kallocator.h"

int main(int argc, char* argv[]) 
{
    initialize_allocator(10000, FIRST_FIT);
    // initialize_allocator(10000, BEST_FIT);
    // initialize_allocator(10000, WORST_FIT);
   // printf("Using first fit algorithm on memory size 100\n");
    int* p[2500] = {NULL};
    for(int i=0; i<2500; ++i) {
        p[i] = kalloc(sizeof(int));
        if(p[i] == NULL) {
            //printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        //printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }

    //list_print();
    //print_statistics();

    for(int i=0; i<2500; ++i) {
         if(i%2 == 0)
             continue;

        //printf("Freeing p[%d]\n", i);
        kfree(p[i]);
        p[i] = NULL;
    }

    p[1] = kalloc(8);
    list_print();
    //kfree(p[2]);
	//list_print();
    //printf("available_memory %d\n", available_memory());

   //  void* before[100] = {NULL};
   //  void* after[100] = {NULL};

   // int x =  compact_allocation(before, after);

    //print_statistics();
     //printf("%d\n", x);
    //list_print();
    destroy_allocator();

    return 0;
}
