
/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __STL_CONFIG_H
#define __STL_CONFIG_H

 //  �������ļ����ܱ�:
 //  (1)  �����������û�ж���bool, true, false����
 //  (2)  �����������֧��drand48()��������__STL_NO_DRAND48
 //       ע: drand48����˫���ȵ�α�����, ��Ϊ������48bit����, ����drand48
 //  (3)  �����������֧��static members of template classes(ģ���ྲ̬��Ա),
 //       ����__STL_STATIC_TEMPLATE_MEMBER_BUG
 //  (4)  �����������֧��'typename'�ؼ���, ��'typename'����Ϊ��(null macro)
 //  (5)  ���������֧��partial specialization of class templates(ģ����ƫ�ػ�),
 //       ����__STL_CLASS_PARTIAL_SPECIALIZATION
 //       �ο�����: http://msdn.microsoft.com/en-us/library/9w7t3kf1(v=VS.71).aspx
 //  (6)  ���������֧��partial ordering of function templates(ģ�庯���ػ����ȼ�),
 //       ����__STL_FUNCTION_TMPL_PARTIAL_ORDER
 //       �ο�����: http://msdn.microsoft.com/zh-cn/library/zaycz069.aspx
 //  (7)  ���������֧��calling a function template by providing its template
 //       arguments explicitly(��ʽָ������ģ�庯����ģ�����)
 //       ����__STL_EXPLICIT_FUNCTION_TMPL_ARGS
 //  (8)  ���������֧��template members of classes(��ģ���Ա),
 //       ����__STL_MEMBER_TEMPLATES
 //  (9)  �����������֧��'explicit'�ؼ���, ��'explicit'����Ϊ��(null macro)
 //  (10) ������������ܸ���ǰһ��ģ������趨�����Ĭ��ģ�����,
 //       ����__STL_LIMITED_DEFAULT_TEMPLATES
 //  (11) �������������ģ�庯����non-typeģ����������ƶ�������,
 //       ����__STL_NON_TYPE_TMPL_PARAM_BUG
 //  (12) �����������֧�ֵ�����ʹ��'->'������,
 //       ����__SGI_STL_NO_ARROW_OPERATOR
 //  (13) ���������(�ڵ�ǰ����ģʽ��)֧���쳣,
 //       ����__STL_USE_EXCEPTIONS
 //  (14) ������ǽ�STL�Ž������ռ���,
 //       ����__STL_USE_NAMESPACES
 //  (15) �����STL��SGI�ı������ϱ���, �����û�û��ѡ��pthreads����no threads,
 //       ��Ĭ��ʹ��__STL_SGI_THREADS
 //       ע: POSIX thread ���Ϊpthread, Posix�߳���һ��POSIX��׼�߳�.
 //  (16) �����STL��Win32ƽ̨�ı�������ʹ�ö��߳�ģʽ����,
 //       ����__STL_WIN32THREADS
 //  (17) �ʵ��Ķ��������ռ���صĺ�(__STD, __STL_BEGIN_NAMESPACE, ��)
 //  (18) �ʵ��Ķ����쳣��صĺ�(__STL_TRY, __STL_UNWIND, ��)
 //  (19) �����Ƿ���__STL_ASSERTIONS, ��__stl_assert����Ϊ���Ի��߿�(null macro)

#ifdef _PTHREADS
#   define __STL_PTHREADS
#endif

// ������������ṩ��STL��Ҫ��һЩ����,����__STL_NEED_XXX
# if defined(__sgi) && !defined(__GNUC__)
#   if !defined(_BOOL)
#     define __STL_NEED_BOOL
#   endif
#   if !defined(_TYPENAME_IS_KEYWORD)
#     define __STL_NEED_TYPENAME
#   endif
#   ifdef _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#     define __STL_CLASS_PARTIAL_SPECIALIZATION
#   endif
#   ifdef _MEMBER_TEMPLATES
#     define __STL_MEMBER_TEMPLATES
#   endif
#   if !defined(_EXPLICIT_IS_KEYWORD)
#     define __STL_NEED_EXPLICIT
#   endif
#   ifdef __EXCEPTIONS
#     define __STL_USE_EXCEPTIONS
#   endif
#   if (_COMPILER_VERSION >= 721) && defined(_NAMESPACES)
#     define __STL_USE_NAMESPACES
#   endif
#   if !defined(_NOTHREADS) && !defined(__STL_PTHREADS)
#     define __STL_SGI_THREADS
#   endif
# endif

# ifdef __GNUC__
#   include <_G_config.h>
#   if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 8)
#     define __STL_STATIC_TEMPLATE_MEMBER_BUG
#     define __STL_NEED_TYPENAME
#     define __STL_NEED_EXPLICIT
#   else
#     define __STL_CLASS_PARTIAL_SPECIALIZATION
#     define __STL_FUNCTION_TMPL_PARTIAL_ORDER
#     define __STL_EXPLICIT_FUNCTION_TMPL_ARGS
#     define __STL_MEMBER_TEMPLATES
#   endif
    /* glibc pre 2.0 is very buggy. We have to disable thread for it.
       It should be upgraded to glibc 2.0 or later. */
#   if !defined(_NOTHREADS) && __GLIBC__ >= 2 && defined(_G_USING_THUNKS)
#     define __STL_PTHREADS
#   endif
#   ifdef __EXCEPTIONS
#     define __STL_USE_EXCEPTIONS
#   endif
# endif

       // Sun C++ compiler
# if defined(__SUNPRO_CC)
#   define __STL_NEED_BOOL
#   define __STL_NEED_TYPENAME
#   define __STL_NEED_EXPLICIT
#   define __STL_USE_EXCEPTIONS
# endif

// TODO: �����û�ҵ�����, �����֪�������������������ϵ��, Thank U
# if defined(__COMO__)
#   define __STL_MEMBER_TEMPLATES
#   define __STL_CLASS_PARTIAL_SPECIALIZATION
#   define __STL_USE_EXCEPTIONS
#   define __STL_USE_NAMESPACES
# endif

// _MSC_VER ����΢��������İ汾
// MS VC++ 10.0 _MSC_VER = 1600
// MS VC++ 9.0 _MSC_VER = 1500
// MS VC++ 8.0 _MSC_VER = 1400
// MS VC++ 7.1 _MSC_VER = 1310
// MS VC++ 7.0 _MSC_VER = 1300
// MS VC++ 6.0 _MSC_VER = 1200
// MS VC++ 5.0 _MSC_VER = 1100
# if defined(_MSC_VER)
#   if _MSC_VER > 1000
#     include <yvals.h>
#   else
#     define __STL_NEED_BOOL
#   endif
#   define __STL_NO_DRAND48
#   define __STL_NEED_TYPENAME
#   if _MSC_VER < 1100
#     define __STL_NEED_EXPLICIT
#   endif
#   define __STL_NON_TYPE_TMPL_PARAM_BUG
#   define __SGI_STL_NO_ARROW_OPERATOR
#   ifdef _CPPUNWIND
#     define __STL_USE_EXCEPTIONS
#   endif
#   ifdef _MT
#     define __STL_WIN32THREADS
#   endif
# endif

# if defined(__BORLANDC__)
#   define __STL_NO_DRAND48
#   define __STL_NEED_TYPENAME
#   define __STL_LIMITED_DEFAULT_TEMPLATES
#   define __SGI_STL_NO_ARROW_OPERATOR
#   define __STL_NON_TYPE_TMPL_PARAM_BUG
#   ifdef _CPPUNWIND
#     define __STL_USE_EXCEPTIONS
#   endif
#   ifdef __MT__
#     define __STL_WIN32THREADS
#   endif
# endif


# if defined(__STL_NEED_BOOL)
typedef int bool;
#   define true 1
#   define false 0
# endif

# ifdef __STL_NEED_TYPENAME
#   define typename
# endif

# ifdef __STL_NEED_EXPLICIT
#   define explicit
# endif

# ifdef __STL_EXPLICIT_FUNCTION_TMPL_ARGS
#   define __STL_NULL_TMPL_ARGS <>
# else
#   define __STL_NULL_TMPL_ARGS
# endif

# ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
#   define __STL_TEMPLATE_NULL template<>
# else
#   define __STL_TEMPLATE_NULL
# endif

// __STL_NO_NAMESPACES is a hook so that users can disable namespaces
// without having to edit library headers.
# if defined(__STL_USE_NAMESPACES) && !defined(__STL_NO_NAMESPACES)
#   define __STD std
#   define __STL_BEGIN_NAMESPACE namespace std {
#   define __STL_END_NAMESPACE }
#   define __STL_USE_NAMESPACE_FOR_RELOPS
#   define __STL_BEGIN_RELOPS_NAMESPACE namespace std {
#   define __STL_END_RELOPS_NAMESPACE }
#   define __STD_RELOPS std
# else
#   define __STD
#   define __STL_BEGIN_NAMESPACE
#   define __STL_END_NAMESPACE
#   undef  __STL_USE_NAMESPACE_FOR_RELOPS
#   define __STL_BEGIN_RELOPS_NAMESPACE
#   define __STL_END_RELOPS_NAMESPACE
#   define __STD_RELOPS
# endif

# ifdef __STL_USE_EXCEPTIONS
#   define __STL_TRY try
#   define __STL_CATCH_ALL catch(...)
#   define __STL_RETHROW throw
#   define __STL_NOTHROW throw()
#   define __STL_UNWIND(action) catch(...) { action; throw; }
# else
#   define __STL_TRY
#   define __STL_CATCH_ALL if (false)
#   define __STL_RETHROW
#   define __STL_NOTHROW
#   define __STL_UNWIND(action)
# endif

#ifdef __STL_ASSERTIONS
# include <stdio.h>
# define __stl_assert(expr) \
    if (!(expr)) { fprintf(stderr, "%s:%d STL assertion failure: %s\n", \
			  __FILE__, __LINE__, # expr); abort(); }
#else
# define __stl_assert(expr)
#endif

#endif /* __STL_CONFIG_H */

// Local Variables:
// mode:C++
// End:
