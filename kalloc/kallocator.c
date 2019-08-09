#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "kallocator.h"

struct node {
   int size;
   void * mem;
   struct node *next;
};

struct KAllocator {
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
    struct node *empty;
    struct node *filled;

    // Some other data members you want, 
    // such as lists to record allocated/free memory
};

struct KAllocator kallocator;

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm) {
    assert(_size > 0);
    kallocator.aalgorithm = _aalgorithm;
    kallocator.size = _size;
    kallocator.memory = malloc((size_t)kallocator.size);
    kallocator.filled = NULL;
    insert( 1 , kallocator.size, kallocator.memory);
    // Add some other initialization 
}

void destroy_allocator() {
	delete_list(0);
	delete_list(1);

    free(kallocator.memory);

    // free other dynamic allocated memory to avoid memory leak
}

void* kalloc(int _size) {
    void* ptr = NULL;

    struct node * temp = find_chunk(_size);
   
    if(temp == NULL)
      printf("Allocation failed\n");

    else
    {
      insert(0,_size, temp->mem);
      ptr = temp->mem;
      //list_sort(0);
      list_merge(0);
      if(temp->size == _size)
        delete_from_list(1, temp);
      else
       {
          temp->size -= _size;
          temp->mem += _size;
       } 
    }
    // Allocate memory from kallocator.memory 
    // ptr = address of allocated memory

    return ptr;
}

void kfree(void* _ptr) {
    //printf("%p\n", _ptr);
    assert(_ptr != NULL);

    struct node *temp = find_chunk_del(_ptr);
    if(temp == NULL)
      printf("temp %p\n", temp);
    else
    {
      insert(1,temp->size, temp->mem);
      delete_from_list(0, temp);
      //list_sort(1);
      list_merge(1);

    }

    //list_print();
    
}

int compact_allocation(void** _before, void** _after) {
    struct node *fil, *newfil;
    fil = kallocator.filled;
    newfil = (struct node*)malloc(sizeof(struct node));
    newfil->mem = NULL;
    newfil->size = 0;
    void * mem_tracker = kallocator.memory;
    void * space_tracker = kallocator.memory;
    int i = 0;
    while(fil != NULL)
    {	
    	//due to ambigious description for the assignment 
    	//i was unable to figure out weather to send all the pointers int before after array or just the one that were changed
    	//i have inluded both but commeneted out one of em
    	//_before[i] = fil->mem;
    	if(mem_tracker < fil->mem)
    	{
    		_before[i] = fil->mem;
    		newfil->size += (fil->mem - space_tracker);
    		space_tracker = fil->mem;
    		move_data(fil, mem_tracker);
    		_after[i] = fil->mem;
    		i++;
    	}
    	mem_tracker = fil->mem +fil->size;
    	space_tracker += fil->size;
    	fil = fil->next;
    	//_after[i] = fil->mem;
    	///i++;
    	
    }
    
    newfil->size += (kallocator.memory + kallocator.size) - space_tracker;
    newfil->mem = kallocator.memory + (kallocator.size - available_memory());
    delete_list(1);
    kallocator.empty = newfil;
    // compact allocated memory
    // update _before, _after and compacted_size
    return i;
}

void move_data(struct node *from, void * to)
{
	int size = from->size;
	void * start = from->mem;
	from->mem = to;
	
	for(int i = 0; i<size; i++)
	{
		*(char *) to = *(char *)start;
		++start;
		++to;
	}

	return;
}

int available_memory() {
    int available_memory_size = 0;
    struct node *temp = kallocator.empty;
    while(temp != NULL)
    {
      available_memory_size += temp->size;
      temp = temp->next;
    }
    // Calculate available memory size
    return available_memory_size;
}

void print_statistics() {
    int allocated_size = 0;
    int allocated_chunks = 0;
    int free_size = 0;
    int free_chunks = 0;
    int smallest_free_chunk_size = kallocator.size;
    int largest_free_chunk_size = 0;

    // Calculate the statistics
    struct node *temp = kallocator.empty;
    struct node *smallest = kallocator.empty;
    struct node *largest = kallocator.empty;
    while(temp != NULL)
    {
      free_size += temp->size;
      free_chunks++;
      if(temp->size <= smallest->size)
        smallest = temp;
      if(temp->size >= largest->size)
        largest = temp;
      temp = temp->next;
    }
    if(smallest == NULL)
    smallest_free_chunk_size = 0;
    else 
    smallest_free_chunk_size = smallest->size;
    
    if(largest == NULL)
    largest_free_chunk_size = 0;
    else
    largest_free_chunk_size = largest->size;

    temp = kallocator.filled;

    while(temp != NULL)
    {
      allocated_size += temp->size;
      allocated_chunks++;
      temp = temp->next;
    }

    printf("Allocated size = %d\n", allocated_size);
    printf("Allocated chunks = %d\n", allocated_chunks);
    printf("Free size = %d\n", free_size);
    printf("Free chunks = %d\n", free_chunks);
    printf("Largest free chunk size = %d\n", largest_free_chunk_size);
    printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);
}

void insert(_Bool empty_list, int sizee, void* memm)
{
    struct node *temp, **head;
    if(empty_list == 1)
        head = &kallocator.empty;
    else
        head = &kallocator.filled;

     temp = (struct node*)malloc(sizeof(struct node));
       
     if (*head == NULL) {
        *head = temp;
        temp->size = sizee;
        temp->mem = memm;
        temp->next = NULL;
        return;
     }
     
     temp->size = sizee;
     temp->mem = memm;
     temp->next = *head;
     *head = temp;
     return;
}



struct node * find_chunk(int sizee)
{
  if(kallocator.empty == NULL)
    return NULL;
  if(available_memory() < sizee)
    return NULL;

  if(kallocator.aalgorithm == FIRST_FIT)
    return firstfit(sizee);
  else if(kallocator.aalgorithm == BEST_FIT)
    return bestfit(sizee);
  else if(kallocator.aalgorithm == WORST_FIT)
    return worstfit(sizee);
  else 
    return NULL;
}

struct node * firstfit(int sizeee)
{
  struct node * temp;
  temp = kallocator.empty;
  while(temp != NULL)
  {
    if(temp->size >= sizeee)
      return temp;
    temp = temp->next;
  }
  return NULL;
}

struct node * bestfit(int sizeee)
{
  struct node *temp, *t;
  temp = kallocator.empty;
  t = NULL;
  int s = kallocator.size;
  while(temp != NULL)
  {
    if(temp->size >= sizeee && (temp->size - sizeee) < s)
    {
      t = temp;
      s = temp->size - sizeee;
    }
    temp = temp->next;
  }
  return t;
}

struct node * worstfit(int sizeee)
{
  struct node *temp, *t;
  temp = kallocator.empty;
  t = NULL;
  int s = -1;
  while(temp != NULL)
  {
    if(temp->size >= sizeee && (temp->size - sizeee) > s)
    {
      t = temp;
      s = temp->size - sizeee;
    }
    temp = temp->next;
  }
  return t;
}



void delete_from_list(_Bool empty_list, struct node *temp) 
{
   struct node **head, *t;
  
    if(empty_list == 1)
        head = &kallocator.empty;
    else
        head = &kallocator.filled;
     
   if (*head == NULL) {
      //printf("Linked list is already empty.\n");
      return;
   }
   
   if (*head == temp) {
      *head = (*head)->next;
      free(temp);
      //printf("deleted from end successfully.\n");
      return;
   }
   
   t = *head;
   
  while(t->next != NULL)
    {
        if (t->next == temp)
            break;
        else
            t = t->next;
    }
   
   t->next = temp->next;
   free(temp);
   return;
}




struct node * find_chunk_del(void* pointer)
{
  if(kallocator.filled == NULL)
    return NULL;
  else if(kallocator.filled->mem == pointer)
    return kallocator.filled;
  else 
  {
    struct node * temp = kallocator.filled;
    while(temp->next != NULL)
    {
      if(temp->next->mem == pointer)
        return temp->next;
      else
        temp = temp->next;
    }
  }
    return NULL;
}

void list_sort(_Bool empty_list)
{
  struct node ** head;
    if(empty_list == 1)
        head = &kallocator.empty;
    else
        head = &kallocator.filled;
  struct node *hea = *head;
  struct node *largest = *head;
  struct node *newhead = NULL;

  while(*head != NULL)
    {
        while(hea != NULL)
        {
            if (hea->mem > largest->mem)
                largest = hea;
            hea = hea->next;
        }
        insert_sort(&newhead, empty_list, largest->size, largest->mem);
        delete_from_list(empty_list,largest);
        hea = *head;
        largest = *head;
    }
    *head = newhead;

}

void insert_sort(struct node **head, _Bool empty_list, int sizee, void* memm)
{
     struct node *temp = (struct node*)malloc(sizeof(struct node));
     void *ext;
     
     if (*head == NULL) {
        *head = temp;
        temp->size = sizee;
        temp->mem = memm;
        temp->next = NULL;
        return;
     }
     
     ext = memm + sizee;
     
     if(ext == (*head)->mem && empty_list)
     {
        (*head)->mem = memm;
        (*head)->size += sizee;
        free(temp);
     }
     else
     {
       temp->size = sizee;
       temp->mem = memm;
       temp->next = *head;
       *head = temp;
     } 
}

void list_merge(_Bool empty_list)
{
  struct node ** head;
    if(empty_list == 1)
        head = &kallocator.empty;
    else
        head = &kallocator.filled;

    struct node * counter = *head;
    struct node * hea = *head;
    struct node * temp = NULL;

    if(counter!=NULL)
    {
    	counter = counter->next;
    	while(counter != NULL)
    	{
    		if(counter->mem < (*head)->mem)
    		{
    			temp = counter;
    		}
    		counter = counter->next;
    	}
    	
    	if(temp != NULL)
    	{
    		*head = (*head)->next;
    		hea->next = temp->next;
    		temp->next = hea;
    		if(empty_list)
    		{
    			if(hea->mem == (temp->mem +temp->size))
    			{
    				temp->size += hea->size;
    				temp->next = hea->next;
    				free(hea);
    				hea = temp->next;
    				//temp = temp->next;
    			}
    			if(hea!=NULL)
    			{
    			if(hea->mem == (temp->mem + temp->size))
	    			{
	    				temp->size += hea->size;
	    				temp->next = hea->next;
	    				free(hea);
	    			}
    			}
    		}
    	}
    }

}

void delete_list(_Bool empty_list)
{
	struct node **head, *prev;
    if(empty_list == 1)
        head = &kallocator.empty;
    else
        head = &kallocator.filled;

    prev = *head;

    while(*head != NULL)
    {
    	*head = (*head)->next;
    	free(prev);
    	prev = *head;
    }
}	


void list_print()
{
	//function to keep track of data inside linked lists
  struct node * emp = kallocator.empty;
  struct node * fil = kallocator.filled;

  
  int i = 0;

  printf("printing Empty\n");
  while(emp != NULL)
  {
    printf("Node %d Mem size %d and mem Loc %p\n", i, emp->size, emp->mem);
    emp = emp->next;
    i++;
  }

  i = 0;
  printf("printing Filled\n");
  while(fil != NULL)
  {
    printf("Node %d Mem size %d and mem Loc %p\n", i, fil->size, fil->mem);
    fil = fil->next;
      i++;
  }

  // emp = kallocator.empty;
  // fil = kallocator.filled;

  // printf("printing Empty\n");
  // i = 0;
  // while(emp != NULL)
  // {
  //   printf("Node %d current addres %p and next node %p\n", i, emp, emp->next);
  //   emp = emp->next;
  //   i++;
  // }

  // i = 0;
  // printf("printing Filled\n");
  // while(fil != NULL)
  // {
  //   printf("Node %d current addres %p and next node %p\n", i, fil, fil->next);
  //   fil = fil->next;
  //     i++;
  // }

}