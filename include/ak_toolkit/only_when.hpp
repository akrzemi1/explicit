#ifndef AK_TOOLKIT_XPLICIT_ONLY_WHEN_HEADER_GUARD_
#define AK_TOOLKIT_XPLICIT_ONLY_WHEN_HEADER_GUARD_

#include <type_traits>
#include <utility>

namespace ak_toolkit {
namespace xplicit {
namespace only_when_ns {

#define AK_TOOLKIT_ENABLE_IF(...) typename ::std::enable_if<(__VA_ARGS__), bool>::type = true

template <typename T, template <typename> class TypePred>
class only_when
{
  T _val;
  
public:
  template <typename I, AK_TOOLKIT_ENABLE_IF(TypePred<I>::value)>  
    only_when (I&& v) : _val(::std::forward<I>(v)) {} 

  template <typename I, AK_TOOLKIT_ENABLE_IF(!TypePred<I>::value)>
    only_when (I&&) = delete;
    
  T get() const { return _val; }
};

#undef AK_TOOLKIT_ENABLE_IF


# if (defined __GNUC__) && (!defined __clang__) && (__GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ <= 7)

template <typename T>
struct is_signed_integral : ::std::conditional<
  ::std::is_signed<T>::value && ::std::is_integral<T>::value,
  ::std::true_type,
  ::std::false_type>::type {};  
  
# else
  
template <typename T>
using is_signed_integral = typename ::std::conditional<
  ::std::is_signed<T>::value && ::std::is_integral<T>::value,
  ::std::true_type,
  ::std::false_type>::type;

# endif
  
}

using only_when_ns::only_when;           // this prevents inadvertent ADL
using only_when_ns::is_signed_integral;  //

using only_int = only_when<int, is_signed_integral>;

# if (defined __GNUC__) && (!defined __clang__) && (__GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ <= 7)

  // sorry not supported
  
# else
  
template <typename T>
  using only_lvalue = only_when<T&, ::std::is_lvalue_reference>;

# endif

}
}

#endif
