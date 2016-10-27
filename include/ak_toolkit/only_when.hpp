#ifndef AK_TOOLKIT_OUT_PARAM_HEADER_GUARD_
#define AK_TOOLKIT_OUT_PARAM_HEADER_GUARD_

#include <type_traits>
#include <utility>

namespace ak_toolkit {
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

template <typename T>
using is_signed_integral = typename ::std::conditional<
  ::std::is_signed<T>::value && ::std::is_integral<T>::value,
  ::std::true_type,
  ::std::false_type>::type;

}

using only_when_ns::only_when;           // this prevents inadvertent ADL
using only_when_ns::is_signed_integral;  //

using only_int = only_when<int, is_signed_integral>;

template <typename T>
  using only_lvalue = only_when<T&, ::std::is_lvalue_reference>;

}

#endif
