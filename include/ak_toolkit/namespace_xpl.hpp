// Copyright (C) 2020 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef AK_TOOLKIT_NAMESPACE_XPL_HEADER_GUARD_
#define AK_TOOLKIT_NAMESPACE_XPL_HEADER_GUARD_


// This provides a shorter namespace alternative to ak_toolkit::xplicit
// Use it when you are not affraid of clashes, in case a different
// library should also use namespace xpl.
namespace ak_toolkit { namespace xplicit {} }
namespace xpl = ak_toolkit::xplicit;

#endif
