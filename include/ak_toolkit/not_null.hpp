#ifndef AK_TOOLKIT_EXPLICIT_NOT_NULL_HEADER_GUARD_
#define AK_TOOLKIT_EXPLICIT_NOT_NULL_HEADER_GUARD_

#include <cstddef>
#include <utility>
#include <memory>

namespace ak_toolkit {
namespace xplicit {
namespace not_null_ns {

#define AK_TOOLKIT_RETURNS(...) noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

template <typename Ptr>
class not_null
{
    Ptr ptr;
public:
    explicit not_null(Ptr p) : ptr(::std::move(p)) {}
    explicit not_null(::std::nullptr_t) = delete;
    Ptr& get_nested() { return ptr; }
    Ptr const& get_nested() const { return ptr; }
    auto operator*() const AK_TOOLKIT_RETURNS(*ptr)
    auto operator*()  AK_TOOLKIT_RETURNS(*ptr)
    auto operator->() const AK_TOOLKIT_RETURNS(::std::addressof(*ptr))
    auto operator->() AK_TOOLKIT_RETURNS(::std::addressof(*ptr))
};

template <typename Ptr>
not_null<Ptr> as_not_null(Ptr p)
{
    return not_null<Ptr>{::std::move(p)};
}

void as_not_null(::std::nullptr_t) = delete;

#undef AK_TOOLKIT_RETURNS

}

using not_null_ns::not_null;
using not_null_ns::as_not_null;

}
}

#endif
