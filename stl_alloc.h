#ifndef STL_ALLOC_H
#define STL_ALLOC_H
#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;//use alloc as the first allocator
#else
//second
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;
#endif /* ! __USE_MALLOC */

template<class T,class Alloc>
class simple_alloc {	//used as interface by all STL containers
public:
	static T* allocate(size_t n)
		{ return n == 0 ? 0 : (T*)Alloc::allocate(n * sizeof T);}
	static T* allocate(void)
		{ return (T*)Alloc::allocate(sizeof T);}
	static void deallocate(T* p,size_t n)
		{ if (n != 0) Alloc::deallocate(p, n * sizeof T);}
	static void deallocate(T* p)
		{ Alloc::deallocate(p, sizeof T);}
};

#if 0
#	include <new>
#	define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include <iostream>
#	define __THROW_BAD_ALLOC cerr << "out of memory" << endl;exit(1);
#endif

//malloc-based allocator,usually slower than default alloc
template<int inst>
class __malloc_alloc_template {
private:
	//oom:out of memory
	static void* oom_malloc(size_t);
	static void* oom_realloc(void *,size_t);
	static void (*__malloc_alloc_oom_handler)();
public:
	static void* allocate(size_t n)
	{
		void* result = malloc(n);
		if (result == 0) result = oom_malloc(n);
		return result;
	}
	static void deallocate(void* p, size_t /* n */)
	{
		free(p);
	}
	static void* reallocate(void* p,size_t /* old_sz */,size_t new_sz)
	{
		void* result = realloc(p,new_sz);
		if (result == 0) result = oom_realloc(p,new_sz);
		return result;
	}
	//imitate c++'s set_new_handler()
	static void (*set_malloc_handler(void (*f)()))()
	{
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return old;
	}
};

//malloc out of memory handling
//initialized by 0,set by customer
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
	void (*my_malloc_handler)();
	void* result;
	for (;;) {	//try again and again...
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (my_malloc_handler == 0) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();//try to relase memory
		result = malloc(n);
		if (result) return result;
	}
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p,size_t n) {
	void (*my_malloc_handler)();
	void* result;
	for (;;) {	//try again and again...
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (my_malloc_handler == 0) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();//try to relase memory
		result = realloc(p,n);
		if (result) return result;
	}
}

//set inst 0
typedef __malloc_alloc_template<0> malloc_alloc;

enum { __ALIGN = 8};
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES/__ALIGN};		//number of free-lists

template<bool threads,int inst>
class __default_alloc_template {
private:
	static size_t ROUND_UP(size_t bytes)
	{
		return 	(bytes + __ALIGN - 1) & ~(__ALIGN - 1);
	}
	union obj 
	{
		union obj* free_list_link;
		char client_data[1];	/* The client sees this. */
	};
	static obj* volatile free_list[__NFREELISTS];
	static size_t FREELIST_INDEX(size_t bytes) 
	{
		return (bytes + __ALIGN - 1) / __ALIGN - 1;
	}
	static void* refill(size_t n);
	static char* chunk_alloc(size_t size, int& nobjs);
	//Chunk allocation state
	static char* start_free;
	static char* end_free;
	static size_t heap_size;

public:
	static void* allocate(size_t n)	//n must > 0
	{
        if(n > (size_t) __MAX_BYTES)    //if >128 use the first allocater
            return malloc_alloc::allocate(n);

        obj* volatile *my_free_list = free_list + FREELIST_INDEX(n);
        obj* result = *my_free_list;
        if(result == 0){   //no avalible free list
            void* r = refill(ROUND_UP(n));
            return r;
        }
		*my_free_list = result->free_list_link;
		return result;
	}
	static void deallocate(void* p, size_t n) 
	{
		if (n > (size_t)__MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}
		obj* volatile *my_free_list = free_list + FREELIST_INDEX(n);
		obj* q = (obj*)p;
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}
	static void* reallocate(void* p,size_t old_sz,size_t new_sz);
};
//defination and initialization
template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;
template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;
template<bool threads, int inst>
__default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] =
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

//return a n-size obj,if possible,add nodes to free list
// 8 | n
template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n) {
	int nobjs = 20;
	//nobjs is pass-by-reference
	char* chunk = chunk_alloc(n, nobjs);
	if (nobjs == 1) return chunk;
	//add nodes
	obj* volatile* my_free_list = free_list + FREELIST_INDEX(n);
	obj* result = (obj*)chunk;	//return the first to customer
	obj* next_obj, *current_obj;
	*my_free_list = next_obj = (obj*)(chunk + n);
	for (int i = 1; ; ++i) {
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);
		if (i == nobjs - 1) {
			current_obj->free_list_link = 0;
			break;
		}
		else {
			current_obj->free_list_link = next_obj;
		}
	}
	return result;
}

// 8 | size
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs) {
	char* result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;
	if (bytes_left >= total_bytes) {	//enough for need
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size) {	//not enough,so give all I have :)
		result = start_free;
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		start_free += total_bytes;
		return result;
	}
	else {
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		if (bytes_left > 0) {
			obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
			(obj*)start_free->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		//heap
		start_free = (char*)malloc(bytes_to_get);
		if (start_free = 0) {
			for (int i = size; i <= __MAX_BYTES; i += __ALIGN) {
				obj* volatile* my_free_list = free_list + FREELIST_INDEX(i);
				obj* p = *my_free_list;
				if (p != 0) {
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					return chunk_alloc(size, nobjs);
				}
			}
			end_free = 0;
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc(size, nobjs);
	}
}

#endif