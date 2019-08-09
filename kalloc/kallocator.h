#ifndef __KALLOCATOR_H__
#define __KALLOCATOR_H__

enum allocation_algorithm {FIRST_FIT, BEST_FIT, WORST_FIT};

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm);

void* kalloc(int _size);

void kfree(void* _ptr);

int available_memory(void);

void print_statistics(void);

int compact_allocation(void** _before, void** _after);

void insert(_Bool empty_list, int sizee, void* memm);

struct node * find_chunk(int sizee);

struct node * find_chunk_del(void* pointer);

void list_print(void);

void delete_from_list(_Bool empty_list, struct node *temp);

void insert_sort(struct node **head,_Bool empty_list, int sizee, void* memm);

void list_sort(_Bool empty_list);

void destroy_allocator(void);

struct node * firstfit(int sizeee);

struct node * bestfit(int sizeee);

struct node * worstfit(int sizeee);

void move_data(struct node *from, void * to);

void delete_list(_Bool empty_list);
void list_merge(_Bool empty_list);



#endif
