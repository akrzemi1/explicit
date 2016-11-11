#ifndef AK_TOOLKIT_XPLICIT_ONLY_WHEN_HEADER_GUARD_
#define AK_TOOLKIT_XPLICIT_ONLY_WHEN_HEADER_GUARD_

#include <type_traits>
#include <utility>

namespace ak_toolkit {
namespace xplicit {
namespace only_when_ns {

#define AK_TOOLKIT_ENABLE_IF(...) typename ::std::enable_if<(__VA_ARGS__), bool>::type = true

template <typename T, template <typename, typename> class TypePred>
class only_when
{
  T _val;
  
public:
  template <typename U, AK_TOOLKIT_ENABLE_IF(TypePred<T, U>::value)>  
    only_when (U&& v) : _val(::std::forward<U>(v)) {} 

  template <typename U, AK_TOOLKIT_ENABLE_IF(!TypePred<T, U>::value)>
    only_when (U&&) = delete;
    
  T get() const { return _val; }
};

#undef AK_TOOLKIT_ENABLE_IF


# if (defined __GNUC__) && (!defined __clang__) && (__GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ <= 7)

template <typename, typename T>
struct is_signed_integral : ::std::conditional<
  ::std::is_signed<T>::value && ::std::is_integral<T>::value,
  ::std::true_type,
  ::std::false_type>::type {};  
  
# else
  
template <typename, typename T>
using is_signed_integral = typename ::std::conditional<
  ::std::is_signed<T>::value && ::std::is_integral<T>::value,
  ::std::true_type,
  ::std::false_type>::type;

# endif
  
namespace detail {

template <typename I, typename T>
std::false_type test_no_narrowing(long long);

template <typename I, typename T>
auto test_no_narrowing(int) -> decltype(I{::std::declval<T>()}, ::std::true_type{}) ;

template <typename I>
struct int_no_double_test
{
  int_no_double_test(I) {}
  int_no_double_test(long double) = delete;
  int_no_double_test(unsigned long long) = delete;
};
  
} // namespace detail
  
# if defined __GNUC__ && ! defined __clang__
#   if (__GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ < 7)
template <typename I, typename T>
struct is_int_convertible_but_no_float : ::std::conditional< ::std::is_convertible<T, I>::value
                                                            && ::std::is_constructible<detail::int_no_double_test<I>, T>::value,
  ::std::true_type, ::std::false_type>::type {}  ;
#   else
template <typename I, typename T>
struct is_int_convertible_but_no_float : ::std::conditional<decltype(detail::test_no_narrowing<I, T>(1))::value 
                                                            && ::std::is_convertible<T, I>::value
                                                            && ::std::is_constructible<detail::int_no_double_test<I>, T>::value,
  ::std::true_type, ::std::false_type>::type {}  ;
#   endif
# else
template <typename I, typename T>
struct is_int_convertible_but_no_float : ::std::conditional<decltype(detail::test_no_narrowing<I, T>(1))::value
                                                            && ::std::is_convertible<T, I>::value,
  ::std::true_type, ::std::false_type>::type {}  ;
# endif
  
template <typename T, typename U>
struct is_lvalue_ref_or_wrapper_ : ::std::conditional<
  ::std::is_convertible<U&&, T&>::value && !::std::is_convertible<U&&, T&&>::value,
  ::std::true_type,
  ::std::false_type
>::type {};
  
}

using only_when_ns::only_when;                       // this prevents inadvertent ADL
using only_when_ns::is_signed_integral;              //
using only_when_ns::is_int_convertible_but_no_float; //
using only_when_ns::is_lvalue_ref_or_wrapper_;       //

typedef only_when<int, is_int_convertible_but_no_float> only_int;

# if (defined __GNUC__) && (!defined __clang__) && (__GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ <= 7)

  // sorry not supported
  
# else
  
template <typename T>
  using only_lvalue = only_when<T&, is_lvalue_ref_or_wrapper_>;

# endif

}
}

#endif
