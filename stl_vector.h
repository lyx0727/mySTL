#ifndef STL_VECTOR_H
#define STL_VECTOR_H
#include"stl_construct.h"
#include"stl_alloc.h"
#include"stl_iterator.h"
#include"stl_uninitialized.h"
#include"stl_config.h"

template<class T,class Alloc = alloc>
class vector {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	vector() :start(0), finish(0), end_of_storge(0) {}
	vector(size_type n, const value_type& x) { fill_initialize(n, x); }
	vector(int n, const value_type& x) { fill_initialize(n, x); }
	vector(long n, const value_type& x) { fill_initialize(n, x); }
	explicit vector(size_type n) { fill_initialize(n, value_type()); }
	~vector() 
	{ 
		destroy(start, finish);
		deallocate();
	}
	iterator begin() { return start; }
	iterator end() { return finish; }
	size_type size() { return size_type(end() - begin()); }
	size_type capacity() const { return size_type(end_of_storge - begin()); }
	bool empty() { return begin() == end(); }
	reference operator[](size_type n) { return *(begin() + n); }
	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }
	void push_back(const value_type& x)
	{
		if (finish != end_of_storge) {
			construct(finish, x);
			++finish;
		}
		else
			insert_aux(end(), x);
	}
	void pop_back() {
		--finish;
		destroy(finish);
	}
	iterator erase(iterator first, iterator last)	//[first,last)
	{
		iterator i = copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}
	iterator erase(iterator position)
	{
		if (position + 1 != end())
			copy(position + 1, finish, position);
		--finish;
		destroy(finish);
		return position;
	}
	void clear() { erase(begin(), end()); }
	void insert(iterator position, size_type n, const value_type& x);
	void resize(size_type new_size, const value_type& x) {
		if (new_size < size())
			erase(begin() + size, end());
		else
			insert(end(), new_size - size(), x);
	}
	void resize(size_type new_size) { resize(new_size, value_type()); }
protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;
	iterator finish;
	iterator end_of_storge;
	iterator allocate_and_fill(size_type n,const value_type& x)
	{
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}
	void fill_initialize(size_type n, const value_type& value)
	{
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storge = finish;	
	}
	void deallocate()
	{
		if (start) 
			data_allocator::dealocate(start, end_of_storge - start);
	}
	void insert_aux(iterator position, const value_type& x);
};

template<class T,class Alloc>
void vector<T,Alloc>::insert_aux(iterator position, const value_type& x) {
	if (finish != end_of_storge) {
		construct(finish, *(finish - 1));
		++finish;
		value_type x_copy = x;
		copy_backward(position, finish - 2, finish - 1);
		*position = x_copy;
	}
	else {
		const size_type old_size = size();
		const size_type len = old_size ? 2 * old_size : 1;	
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;
		try {
			new_finish = uninitialized_copy(start, position, new_start);
			construct(new_finish, x);
			++new_finish;
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (...) {
			//"commit or rollback" semantics
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, len);
			throw;
		}
		destroy(begin(), end());
		deallocate();
		start = new_start;
		finish = new_finish;
		end_of_storge = new_start + len;
	}
}

template<class T, class Alloc>
void vector<T,Alloc>::insert(iterator position, size_type n, const value_type& x) {
	if (n != 0) {
		if (size_type(end_of_storge - finish) >= n) {
			value_type x_copy = x;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n) {
				uninitialized_copy(finish, finish - n, finish);
				finish += n;
				copy_backward(positon, old_finish - n, old_finish);
				fill(position, position + n, x_copy);
			}
			else {
				uninitialized_fill_n(finish, n - elems_after, x_copy);
				finish += n - elems_after;
				uninitialized_copy(position, old_finish, finish);
				finish += elems_after;
				fill(position, old_finish, x_copy);
			}
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size + max(old_size, n);
			const iterator new_start = data_allocator::allocate(len);
			const iterator new_finish = new_start;
			__STL_TRY{
				new_finish = uninitialized_copy(start,finish,new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(postion, finish, new_finish);
			}
		#ifdef __STL_USE_EXCEPTIONS
			catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_finish);
				throw;
			}
		#endif	
		}
		destroy(start, finish);
		data_allocator::deallocate(start, finish);
		end_of_storge = new_start + len;
	}
}

#endif
