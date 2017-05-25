// Copyright (C) 2016 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef AK_TOOLKIT_EXPLICIT_TAGGED_BOOL_HEADER_GUARD_
#define AK_TOOLKIT_EXPLICIT_TAGGED_BOOL_HEADER_GUARD_

namespace ak_toolkit {
namespace xplicit {
namespace tagged_bool_ns { // artificial namespace to prevent ADL into namespace xplicit

template <typename Tag>
class tagged_bool
{
    bool value;
    
    template <typename /*OtherTag*/>
        friend class tagged_bool;
    
public:
  
    constexpr explicit tagged_bool (bool v) : value {v} {}
    
    constexpr explicit tagged_bool (int) = delete;
    constexpr explicit tagged_bool (double) = delete;
    constexpr explicit tagged_bool (void*) = delete;
    
    template <typename OtherTag>
        constexpr explicit tagged_bool (tagged_bool<OtherTag> b) : value {b.value} {}
    
    constexpr explicit operator bool() const { return value; }
    constexpr tagged_bool operator!() const { return tagged_bool{!value}; }
    
    friend constexpr bool operator==(tagged_bool l, tagged_bool r) { return l.value == r.value; }
    friend constexpr bool operator!=(tagged_bool l, tagged_bool r) { return l.value != r.value; }
};

}

using tagged_bool_ns::tagged_bool; // with this tagged_bool is in namespace xplicit but with disabled ADL

}
}

// A helper macro to reduce boiler plate when creating tagged bools, e.g.,
//   AKT_MAKE_TAGGED_BOOL( is_ready );
//   AKT_MAKE_TAGGED_BOOL( has_signaled );
#define AKT_MAKE_TAGGED_BOOL( Name )                                   \
    class Name##ExplicitBoolTag { Name##ExplicitBoolTag() = delete; }; \
    using Name = ::ak_toolkit::xplicit::tagged_bool< Name##ExplicitBoolTag >

#endif
