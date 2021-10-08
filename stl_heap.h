#ifndef STD_HEAP_H
#define STD_HEAP_H
#include"stl_iterator.h"

//there must be a new element added at last
template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, 
					RandomAccessIterator last) {
	__push_heap_aux(first, last, distance_type(first), value_type(first));
}

template<class RandomAccessIterator,class Distance,class T>
void __push_heap_aux(RandomAccessIterator first, 
					RandomAccessIterator last,
					Distance*, T*) {
	__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

template<class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
				Distance topIndex, T value) {
	Distance parent = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && *(first + parent) < value) {
		*(first + holeIndex) = *(first + parent);
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;
}

//the max element are put at the end
//client can pop it using the base-container's pop_back
template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__pop_heap_aux(first, last, value_type(first));
}

template<class RandomAccessIterator,class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last,T*) {
	__pop_heap(first, last - 1, last - 1,T(*(last - 1)),distance_type(first));
}

template<class RandomAccessIterator, class T,class Distance>
inline void __pop_heap(RandomAccessIterator first, 
						RandomAccessIterator last,
						RandomAccessIterator result,
						T value,Distance*) {
	*result = *last;
	__adjust(first, Distance(0), Distance(last - first), value);
}

template<class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex,
				Distance len, T value) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;	//right son
	
	while (secondChild < len) {
		if (*(first + secondChild) < *(first + secondChild - 1))
			--secondChild;	//switch to left son
		//Percolate down
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChilde = 2 * secondChild + 2;
	}
	if (secondChild == len) {	//only has left son
		//Percolate down
		*(first + holeIndex) = *(first + secondChild - 1);
		holeIndex = secondChild - 1;
	}
	__push_heap(first, holeIndex, topIndex, value);
	//or *(first + holeIndex) = value;
}

template<class RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
	while (last - first > 1)
		pop_heap(first, last--);
}

template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__make_heap(first, last, value_type(first),distance_type(first));
}

template<class RandomAccessIterator,class T,class Distance>
inline void __make_heap(RandomAccessIterator first, 
						RandomAccessIterator last,
						T*,Distance*) {
	if (last - first < 2) return; //size = 0 or 1
	Distance len = last - first;
	//percolate down
	Distance parent = (len - 2) / 2;
	while (true) {
		__adjust(first, parent, len, T(*(first + parent)));
		if (parent == 0) return;
		--parent;
	}
}

#endif