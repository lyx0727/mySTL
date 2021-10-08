#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

struct __true_type {};
struct __false_type {};

template<class type>
struct __type_traits {
	/*
		Never remove this
	*/
	typedef __true_type this_dummy_member_must_be_first;
	/*
		Remember to obey:
			-Reorder the members followed
			-Remove the members followed
			-Never rename them without changing the correspond one in compiler
			-New menmbers will be treated normally,unless you support in compiler
	*/
	typedef __false_type has_trival_default_constructor;
	typedef __false_type has_trival_copy_constructor;
	typedef __false_type has_trival_assignment_operator;
	typedef __false_type has_trival_destructor;
	typedef __false_type is_POD_type;
};

//followed are specialized versions
//to be continued... :)

#endif