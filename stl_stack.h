#ifndef STL_STACK_H
#define STL_STACK_H
#include"stl_deque.h"

template<class T,class Sequence = deque<T>>
class stack {
	friend bool operator==__STL_NULL_TMPL_ARGS(const stack&, const stack&);
	friend bool operator!=__STL_NULL_TMPL_ARGS(const stack&, const stack&);
public:
	typedef Sequence::value_type value_type;
	typedef Sequence::size_type size_type;
	typedef Sequence::reference reference;
	typedef Sequence::const_reference const_reference;
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_back(); }
protected:
	Sequence c;
};

template<class T,class Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
	return x.c == y.c;
}

template<class T, class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
	return x.c < y.c;
}

#endif