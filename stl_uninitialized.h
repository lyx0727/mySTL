#include"type_traits.h"

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////     uninitialized_fill_n     //////////////////////////
//////////////////////////////////////////////////////////////////////////////////
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, 
											Size n, const T& x) {
	return __unintialized_fill_n(first, n, x, value_type(first));
}

//POD : Plain Old Data
//scalar types or C struct
//has trival ctor/dtor/copy/assignment function
template<class ForwardIterator, class Size, class T,class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,
											Size n, const T& x, T1*) {
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
												Size n, const T& x, __true_type) {
	return fill_n(first, n, x);
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
												Size n, const T& x, __false_type) {
	ForwardIterator cur = first;
	//exception handling is ignored
	for (; n > 0; --n, ++cur) {
		construct(&*cur, x);
	}
	return cur;
}

//////////////////////////////////////////////////////////////////////////////////
///////////////////////////     uninitialized_copy     ///////////////////////////
//////////////////////////////////////////////////////////////////////////////////
template<class InputIterator,class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
										ForwardIterator result) {
	return __uninitialized_copy(first, last, result, value_type(result));
}

template<class InputIterator, class ForwardIterator,class T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last,
										ForwardIterator result,T*) {
	typedef typename __type_traits<T>::is_POD_type is_POD
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last,
											ForwardIterator result, __true_type) {
	return copy(first, last, result);
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last,
											ForwardIterator result, __false_type) {
	ForwardIterator cur = result;
	//exception handling is ignored
	for (; first != last; ++cur, ++first) 
		construct(&*cur, *first);
	return cur;
}

//for const char*
inline char* uninitialized_copy(const char* first, const char* last,
								char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

//for const wchar_t*
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, 
								wchar_t* result) {
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}

//////////////////////////////////////////////////////////////////////////////////
///////////////////////////     uninitialized_fill     ///////////////////////////
//////////////////////////////////////////////////////////////////////////////////
template<class ForwardIterator,class T>
inline ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last,
										const T& x) {
	return __uninitialized_fill(first, last, x, value_type(first));
}

template<class ForwardIterator, class T,class T1>
inline ForwardIterator __uninitialized_fill(ForwardIterator first, ForwardIterator last,
										const T& x,T1*) {
	typedef typename __type_traits<T>::is_POD_type is_POD
		return __uninitialized_fill_aux(first, last, x, is_POD());
}

template<class ForwardIterator,class T>
inline ForwardIterator __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
											 __true_type) {
	return fill(first, last, x);
}

template<class ForwardIterator>
inline ForwardIterator uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
											 __false_type) {
	ForwardIterator cur = first;
	//exception handling is ignored
	for (; cur != last; ++cur)
		construct(&*cur, x);
	return cur;
}