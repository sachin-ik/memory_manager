/* The c file contains different functions needed 
for dynamic memory management. The file contains 6 functions

my_malloc()-use for allocating memory space
my_realloc()-use for reallocating memory size
my_calloc()-use for allocating memory space with type and size as parameters
my_free()-use for removing the space allocated
free_space_in_my_heap()-use for finding heap space available
defragment_my_heap()-use for alligning data in sequence after free()

*/

#include<stdio.h>
#include<stdbool.h>

#define HEAP_SIZE 1000							//total size of the heap
										
typedef struct mem_header						//data structure which olds data about the memory blocks
{
	struct mem_header* next;					//used to link next element
	int block_size;								//allocated memory to that block
	bool available;								//if available=1 then the block is free, else it is alloted
	int available_bytes;						//total number of bytes available after each allocation
	void* heap_pointer;							//points to the actual memory in heap
		
}mem_head;			

int heap_size = HEAP_SIZE;						//Global variable holds available bytes in a heap block
char heap_block[HEAP_SIZE];						//heap block of size(bytes) HEAP_SIZE
mem_head mem_addr;								//starting point of linked list to hold data about heap blocks

void 
initialize()									//initializes the values for the first pointer of the list
{
	mem_addr.next = NULL;
	mem_addr.block_size = 0;
	mem_addr.available = 1;
	mem_addr.available_bytes = HEAP_SIZE;
	mem_addr.heap_pointer = heap_block;			//heap_pointer pointing to first byte in heap block
}


/*function to move data from one block to another */
void my_mem_move(void *dest, void *src, size_t n)	
{
	int i;
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
 
   // Create a temporary array to hold data of src
   char temp[n];
 
   // Copy data from csrc[] to temp[]
   for (i=0; i<n; i++)
       temp[i] = csrc[i];
 
   // Copy data from temp[] to cdest[]
   for (i=0; i<n; i++)
       cdest[i] = temp[i];
 
}


/* my_malloc allocates memory block to the pointer.
It takes size_t type as parameter which gives number of bytes size*/
void* 
my_malloc(size_t size)
{
	if(size <= 0 || size > HEAP_SIZE)									//size should be positive value and within available size
	{
		printf("SIZE INPUT ERROR\n");
		return NULL;													//returns NULL when memory not alloted
	}

	mem_head* temp = &mem_addr;											//temporary pointer to first link initialized list

	while(temp)															//checks till the list contains NULL pointer
	{
		if(temp->available == 1)
		{
			
			if(temp->available_bytes > size)							//allocating memory of requested size 
			{
			
				heap_size -= size;										//updates free bytes available
				/*updating alloted memory block pointer */
				temp->block_size = size;								
				temp->available = 0;
				temp->next = (temp) + sizeof(mem_head);
				
				/*updating the next pointer */
				mem_head* next_mem_addr = temp->next;
				next_mem_addr->available = 1;
				next_mem_addr->available_bytes = heap_size;
				next_mem_addr->heap_pointer = ((temp->heap_pointer) + size);	//move the heap_pointer to next free byte
				next_mem_addr->next = NULL;
				//printf("memory of size %d is alloted\n",size);	
				return temp->heap_pointer;
			
			}
			//printf("size exceeds not alloted\n");
			return NULL;														//return NULL if memory size required is more than available
		}	
	temp = (temp) + sizeof(mem_head);											//increment the pointer to next 
	}
		
	return NULL;																//return NULL if memory not available
}
/* END OF MY_MALLOC FUNCTION*/

/*my_free function make the block of memory available for allocating */
void 
my_free(void* ptr)												
{
	if(ptr!=0)
	{
		mem_head* tmp = &mem_addr;

		while(tmp->heap_pointer != ptr)											//finding the pointer which holds data of memory at ptr
			{
				tmp=tmp->next;
			}
		tmp->available = 1;														//make the block available for allocation
		heap_size += tmp->block_size;											//update free bytes available
	}
	
	printf("memory is not alloted to this pointer\n");
}
/* END OF MY_FREE FUNCTION*/

/* free_space_in_my_heap() gives number of free bytes available*/
size_t 
free_space_in_my_heap (void)								
{
	return heap_size;
}
/* END of function*/


/* mycalloc allocates memory similar to my_malloc with different parameters*/
void* 
my_calloc(size_t nmemb, size_t size)										//it takes total size of type nmemb and size of nmemb as parameters
{
	int i;
	void* heap_ptr_mem=my_malloc((size)*nmemb);
	char* temp=heap_ptr_mem;
	for(i=0;i<size;i++)
	{
		*temp=0;
		temp++;
	}
	
return heap_ptr_mem;
}
/* END OF MY_CALLOC */


/* defragment_my_heap function takes no parameters it shifts all data blocks as
sequence from start of memory block and free bytes to other side*/
void 
defragment_my_heap (void)
{
	mem_head *tmp1 = &mem_addr;
	mem_head *tmp2;
	void* t;
	int swap_var;
	while(tmp1)
	{
	
		if(tmp1->available && tmp1->next)
		{
		
			tmp2 = tmp1->next;
			//printf("%u\n",tmp2->next);
			//printf("%d\n",tmp2->available);
			if(!tmp2->available)
			{
				 //printf("303\n");
				
				 my_mem_move(tmp1->heap_pointer, tmp2->heap_pointer, tmp2->block_size);
				 tmp1->available = 0;
				 t=tmp1->heap_pointer;
				 printf("tmp1=%u\n",t);
				 
				 tmp2->heap_pointer = (t + tmp2->block_size);
				 printf("tmp2=%u\n",tmp2->heap_pointer);
				 tmp2->available = 1;
				 printf("tmp1=%u\n",tmp1->heap_pointer);
				 
				 swap_var = tmp2->block_size;
				 tmp2->block_size = tmp1->block_size;
				 tmp1->block_size=swap_var;
				 
				// tmp1->heap_pointer = tmp2->heap_pointer;
				 //tmp1->block_size = tmp2->block_size;
				 //tmp1->next->block_size=tmp1->block_size;
				 //tmp1->next->available=1;
				 //tmp1->next->next=tmp2->next;
			}
			//printf("4\n");
			tmp1->block_size = (tmp1->block_size + tmp2->block_size);
			tmp1->next = tmp2->next;
		}	
		
		tmp1 = tmp1->next;
	}	
}
/* END OF FUNCTION */

/* my_realloc reallocates memory with different size.
it takes heap_pointer and new size as parameters*/
void * 
my_realloc (void * ptr, size_t size)
{
	int minimum_size;
	void* newptr;
	
	mem_head* tmp = &mem_addr;

	while(tmp->heap_pointer != ptr)
	{
		tmp = tmp->next;
	}
	
//	printf("%u\n",tmp->heap_pointer);
	
	newptr = my_malloc(size);												//allocate memory to newptr first then checking size
	
//	printf("%u\n",tmp->heap_pointer);
	
	
	mem_head *new_tmp = &mem_addr;

	while(new_tmp->heap_pointer != newptr)
	{
		new_tmp = new_tmp->next;
	}
	
	if(newptr == NULL)														//if memory not available return NULL
		return NULL;
	
	if(tmp != NULL)															//if the memory pointer is present
	{
		minimum_size = tmp->block_size;										
		
		if(size < minimum_size)
		{
			minimum_size = size;											//finding minimum block size between newptr and ptr
		}
		
		my_mem_move(new_tmp->heap_pointer, tmp->heap_pointer, minimum_size);	//move the data to the original pointer if size is available
		
		my_free(tmp->heap_pointer);											//free the newptr 
	}
	
return tmp->heap_pointer;													//function returns pointer to memory block
}
/* END OF FUNCTION MY_REALLOC*/

int 
main()
{	
	initialize();
	int* m = (int *) my_malloc(sizeof(int)*25);
	char* str;
	float* n = (float *) my_calloc(220, sizeof(float));
	
	int i;
	int* tmp;
	int* t1;

	printf("int alloc=%u\n", m);

//	my_free(n);
	printf("float alloc=%u\n", n);
	
	//my_free(n);
	str = (char *) my_malloc(sizeof(char)*10);
	printf("string alloc=%u\n", str);

	tmp = m;
	t1 = m;
	
		for(i = 0; i < 25; i++)
		{
			*m = i;
			m++;
		}
	
		for(i = 0; i < 25; i++)
		{
		//printf("%u=%d\n",tmp,*tmp);
			tmp++;
		
		}
//	printf("%u\n",m);
//	my_realloc(m, sizeof(int)*10);
//	printf("int after realloc=%u\n", m);
	my_free(n); 	
	printf("str after free n=%u\n", str);
	defragment_my_heap();
	printf("str after defragment=%u\n", str);	
	*str = 'a';
	printf("free space=%d\n", free_space_in_my_heap());
//	printf("%u=%c\n",str,*str);
	//printf("%u\n",&heap_block[104]);
//	printf("%u to %u\n",&heap_block[0],&heap_block[999]);
	return 0;
}
