// Copyright (C) 2016 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef AK_TOOLKIT_EXPLICIT_TAGGED_BOOL_HEADER_GUARD_
#define AK_TOOLKIT_EXPLICIT_TAGGED_BOOL_HEADER_GUARD_

namespace ak_toolkit {
namespace xplicit {
namespace tagged_bool_ns {

template <typename Tag>
class tagged_bool
{
    bool value;
    
public:
  
    constexpr explicit tagged_bool (bool v) : value {v} {}
    
    constexpr explicit tagged_bool (int) = delete;
    constexpr explicit tagged_bool (double) = delete;
    constexpr explicit tagged_bool (void*) = delete;
    
    constexpr explicit operator bool() const { return value; }
    
    friend constexpr bool operator==(tagged_bool l, tagged_bool r) { return l.value == r.value; }
    friend constexpr bool operator!=(tagged_bool l, tagged_bool r) { return l.value != r.value; }
};

}

using tagged_bool_ns::tagged_bool;

}
}

#endif
