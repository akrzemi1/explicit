// Copyright (C) 2016-2020 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef AK_TOOLKIT_OUT_PARAM_HEADER_GUARD_
#define AK_TOOLKIT_OUT_PARAM_HEADER_GUARD_

#include <type_traits>

namespace ak_toolkit {
namespace xplicit {
namespace out_param_ns {

  template <typename T>
  class out_param
  {
    static_assert(std::is_lvalue_reference<T>::value || std::is_pointer<T>::value, "only references or pointers to non-const");
  };

  template <typename T>
  class out_param<T&>
  {
    static_assert(!std::is_const<T>::value, "only referemces to non-const");

    T& _ref;
  public:
    explicit out_param(T& ref) : _ref(ref) {}

    T& get() { return _ref; }
  };

  template <typename T>
  class out_param<T*>
  {
    static_assert(!std::is_const<T>::value, "only pointers to non-const");

    T* _ref;
    public:
    explicit out_param(T* ref) : _ref(ref) {}
    out_param(std::nullptr_t) : _ref(nullptr) {}

    T* get() { return _ref; }
  };


  template <typename T>
  out_param<T&> out(T& ref)
  {
    static_assert(!std::is_const<T>::value, "only references to non-const are allowed");
    return out_param<T&>(ref);
  }

  template <typename T>
  out_param<T*> out(T* ref)
  {
    static_assert(!std::is_const<T>::value, "only pointers to non-const are allowed");
    return out_param<T*>(ref);
  }

}

using out_param_ns::out_param; // this prevents inadvertent ADL
using out_param_ns::out;

}
}

#endif
