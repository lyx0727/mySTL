#ifndef STL_QUEUE_H
#define STL_QUEUE_H
#include"stl_deque.h"
#include"stl_vector.h"
#include"stl_heap.h"
#include<queue>

template<class T, class Sequence = deque<T>>
class queue {
	friend bool operator==__STL_NULL_TMPL_ARGS(const queue&, const queue&);
	friend bool operator!=__STL_NULL_TMPL_ARGS(const queue&, const queue&);
public:
	typedef Sequence::value_type value_type;
	typedef Sequence::size_type size_type;
	typedef Sequence::reference reference;
	typedef Sequence::const_reference const_reference;
	
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_back(); }
protected:
	Sequence c;
};

template<class T, class Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
	return x.c == y.c;
}

template<class T, class Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
	return x.c < y.c;
}

template<class T,class Sequence = vector<T>,
		class Compare = less<typename Sequence::value_type>>
class priority_queue {
public:
	typedef Sequence::value_type value_type;
	typedef Sequence::size_type size_type;
	typedef Sequence::reference reference;
	typedef Sequence::const_reference const_reference;

	priority_queue() :c() {}
	explicit priority_queue(const Compare& x) :c(),comp(x) {}
	template<class InputIterator,class Compare>
	priority_queue(InputIterator first,InputIterator last,const Compare& x)
		: c(first, last), comp(x) { make_heap(c.begin(), c.end(),comp); }
	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last)
		: c(first, last){ make_heap(c.begin(), c.end(), comp);}

	bool empty() { return c.empty(); }
	size_type size() { return c.size(); }
	const_reference top() const { return c.front(); }
	void push(const value_type& x) 
	{
		__STL_TRY{
			c.push_back(x);
			push_heap(c.begin(), c.end(),comp);
		}
		__STL_UNWIND(c.clear());
	}
	void pop()
	{
		__STL_TRY{
			c.pop_back();
			pop_heap(c.begin(), c.end(),comp);
		}
		__STL_UNWIND(c.clear());
	}
protected:
	Sequence c;
	Compare comp;	
};

#endif