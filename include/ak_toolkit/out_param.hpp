// Copyright (C) 2016 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef AK_TOOLKIT_OUT_PARAM_HEADER_GUARD_
#define AK_TOOLKIT_OUT_PARAM_HEADER_GUARD_

#include <type_traits>

namespace ak_toolkit {
namespace out_param_ns {

  template <typename T>
  class out_param
  {
    static_assert(std::is_lvalue_reference<T>::value, "only references to non-const");
    static_assert(!std::is_const<typename std::remove_reference<T>::type>::value, "only references to non-const");
    T& _ref;
  public:
    explicit out_param(T& ref) : _ref(ref) {}
    T& get() { return _ref; }
  };

  template <typename T>
  out_param<T&> out(T& ref)
  {
    static_assert(!std::is_const<T>::value, "only references to non-const");
    return out_param<T&>(ref);
  }

}

using out_param_ns::out_param; // this prevents inadvertent ADL
using out_param_ns::out;

}

#endif
