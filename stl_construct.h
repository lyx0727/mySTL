#ifndef STL_CONSTRUCT_H
#define STL_CONSTRUCT_H
#include <new.h>	//placement new
#include"type_traits.h"

//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////     construct      ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
template<class T1,class T2>
inline void construct(T1* p, const T2& value) {
	new(p) T1(value);
}

//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////      destroy      ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//version 1
template<class T>
inline void destroy(T* pointer) {
	pointer->~T();
}

//version 2
//[first,last)
template<class ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last) {
	__destroy(first, last, value_type(first));
}

//has trivial destructor?
template<class ForwardIterator,class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destroy_aux(first, last, trivial_destructor());
}

//if has non-trivial destructor
template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
	for (; first < last; ++first)
		destroy(&*first);	//call for destroy version 1
}

//if has trivial destructor
template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

//specialize for char* and wchar_t* by destroy version 2
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

#endif