#ifndef STL_LIST_H
#define STL_LIST_H
#include"stl_iterator.h"
#include"type_traits.h"
#include"stl_alloc.h"

template<class T>
struct __list_node{
	typedef	void* void_pointer;
	void_pointer prev;
	void_pointer next;
	T data;
};

template<class T,class Ref,class Ptr>
struct __list_iterator{
	typedef __list_iterator<T, T&, T*> iterator;
	typedef __list_iterator<T, Ref, Ptr> self;

	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef size_t size_type;
	typedef __list_node<T>* link_type;

	link_type node;
	__list_iterator() {}
	__list_iterator(link_type x):node(x) {}
	__list_iterator(const iterator& x):node(x.node) {}
	bool operator==(const self& x) const { return node == x.node; }
	bool operator!=(const self& x) const { return node != x.node; }
	reference operator*() const { return (*node).data; }
	pointer operator->() const { return &(operator*()); }
	self& operator++() 
	{
		node = (link_type)((*node).next);
		return *this;
	}
	self operator++(int)
	{
		self tmp = *this;
		++* this;
		return tmp;
	}
	self& operator--()
	{
		node = (link_type)((*node).prev);
		return *this;
	}
	self operator--(int)
	{
		self tmp = *this;
		--* this;
		return tmp;
	}
};

template<class T,class Alloc = alloc>
class list {
public:
	typedef __list_node<T> list_node;
	typedef list_node* link_type;
	typedef __list_iterator<T,T&,T*> iterator;
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	list() { empty_initialize(); }
	iterator begin() { return (link_type)((*node).next); }
	iterator end() { return node; }
	reference front() { return *begin(); }
	reference back() { return *(--end()); }
	bool empty() { return node->next == node;}
	size_type size() { return distance(begin(), end());}

	iterator insert(iterator position,const T& x)
	{
		link_type tmp = create_node(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(link_type(positon.node->prev))->next = tmp;
		position.node->prev = tmp;
		return tmp;
	}
	void push_front(const T& x) { insert(begin(), x); }
	void push_back(const T& x) { insert(end(), x); }

	iterator erase(iterator position)
	{
		link_type next_node = link_type(position.node->next);
		link_type prev_node = link_type(position.node->prev);
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destory_node(position.node);
		return iterator(next_node);
	}
	void pop_front() { erase(begin()); }
	void pop_back() { erase(--end()); }
	void clear();
	void remove(const T&);
	void unique();

	//transfer [first,last) to the front of position
	void transfer(iterator position, iterator first, iterator last)
	{
		if (position != last) {
			*(link_type((*last.node).prev)).next = position.node;
			*(link_type((*first.node).prev)).next = last.node;
			*(link_type((*position.node).prev)).next = first.node;
			link_type tmp = link_type((*position.node).prev);
			(*position.node).prev = ги*lastt.node).prev;
			(*last.node).prev = (*first.node).prev;
			(*first.node).prev = tmp;
		}
	}
	//x must != *this
	void splice(iterator position, list& x)	
	{
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}
	//put i before position
	void splice(iterator position, list&, iterator i)
	{
		iterator j = i;
		++j;
		if (j == position || i == position) return;
		transfer(position, i, j);
	}
	//put [first,last) before position
	//position must be out of [first,last)
	void splice(iterator position, list&, iterator first, iterator last)
	{
		if (first != last)
			transfer(position, first, last);
	}
	void merge(list&);
	void reverse();
	void sort();
	void swap(list&,x) { swap(*this, x); }
protected:
	typedef simple_alloc<list_node, Alloc> list_node_allocator;
	link_type node;
	link_type get_node() { return list_node_allocator::allocate(); }
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	link_type create_node(const T& x)
	{
		link_type p = get_node();
		construct(p, x);
		return p;
	}
	void destroy_node(link_type p)
	{
		destroy(&p->data);
		put_node(p);
	}
	void empty_initialize()
	{
		node = get_node();
		node->next = node;
		node->prev = node;
	}
};

template<class T,class Alloc>
void list<T, Alloc>::clear() {
	link_type cur = (link_type)node->next;
	while (cur != node) {
		link_type tmp = cur;
		cur = (link_type)cur->next;
		destory_node(tmp);
	}
	node->next = node;
	node->prev = node;
}

template<class T, class Alloc>
void list<T, Alloc>::remove(const T& value) {
	iterator first = begin();
	iterator last = end();
	while (first != last) {
		iterator next = first;
		++next;
		if (*first == value) erase(first);
		first = next;
	}
}

template<class T, class Alloc>
void list<T, Alloc>::unique() {
	iterator first = begin();
	iterator last = end();
	if (first == last) return;	//empty
	iterator next = first;
	while (++next != last) {
		if (*first == *next)
			erase(next);
		else
			first = next;
		next = first;
	}	
}

//both *this and x must be sorted
template<class T, class Alloc>
void list<T, Alloc>::merge(list& x) {
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = x.begin();
	iterator last2 = x.end();
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *last2) {
			iterator next = first2;
			transfer(first1, first2, ++next);
			first2 = next;
		}
		else 
			++first1;
		if (first2 != last2)
			transfer(last1, first2, last2);
	}
}

template<class T, class Alloc>
void list<T, Alloc>::reverse() {
	//size == 0 || size == 1
	if (node->next == node || link_type(node->next)->next == node) return;
	iterator first = begin();
	++first;
	while (first != end()) {
		iterator old = first;
		++first;
		transfer(begin(), old,first);
	}
}

//merge sort
template<class T, class Alloc>
void list<T, Alloc>::sort() {
	if (node->next == node || link_type(node->next)->next == node) return;
	list<T, ALloc> counter[64], carry;
	int fill = 0;
	while (!empty()) {
		carry.splice(carry.begin(), *this, begin());
		int i = 0;
		while (i < fill && !counter[i].empty()) {
			counter[i].merge(carry);
			carry.swap(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill) ++fill;
	}
	for (int i = 1; i < fill; ++i)
		counter[i].merge(counter[i - 1]);
	swap(counter[fill - 1]);
}
#endif
