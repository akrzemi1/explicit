// Copyright (C) 2016 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef AK_TOOLKIT_XPLICIT_LVALUE_REF_HEADER_GUARD_
#define AK_TOOLKIT_XPLICIT_LVALUE_REF_HEADER_GUARD_

#include <functional>
#include <type_traits>

#if defined AK_TOOLKIT_USING_BOOST
#include <boost/ref.hpp>
#endif

namespace ak_toolkit {
namespace xplicit {
namespace lvalue_ns {

template <typename T, typename U>
using is_reference_compatible = typename ::std::conditional<
  ::std::is_base_of<typename ::std::remove_cv<T>::type, typename ::std::remove_cv<U>::type>::value
  && ::std::is_convertible<U&, T&>::value,
  ::std::true_type,
  ::std::false_type
>::type;

template<typename T, typename U>
struct is_reference_wrapper_for : ::std::conditional<
    ::std::is_same<typename ::std::decay<U>::type, U>::value,
    std::false_type, 
    is_reference_wrapper_for<T, typename ::std::decay<U>::type>
  >::type
{};

template<typename T, typename U>
struct is_reference_wrapper_for<T, std::reference_wrapper<U>> : is_reference_compatible<T, U> {};

#if defined AK_TOOLKIT_USING_BOOST
template<typename T, typename U>
struct is_reference_wrapper_for<T, boost::reference_wrapper<U>> : is_reference_compatible<T, U> {};
#endif

template <typename T>
class lvalue_ref
{
  static_assert(!::std::is_reference<T>::value, "lvalue_ref<T> can only be instantiated with a non-reference type"); 
  T& _ref;
  
public:  
  lvalue_ref(T& v) : _ref(v) {}   // conversion from lvalue
  lvalue_ref(T const&&) = delete; // disabled conversion from rvalue
  
  template <typename U, typename ::std::enable_if<is_reference_wrapper_for<T, U>::value, bool>::type = true>
    lvalue_ref(U&& ref) : _ref(::std::forward<U>(ref)) {}
  
  T& get() const { return _ref; }
  operator T& () const { return _ref; }
};

}

using lvalue_ns::lvalue_ref; // this prevents inadvertent ADL

}
}

#endif