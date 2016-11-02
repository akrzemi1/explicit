#include "ak_toolkit/lvalue_ref.hpp"

using namespace ak_toolkit::xplicit;

// test lvalue_ref.hpp

struct Big1 { Big1(Big1&&) = delete; Big1() = default; };
struct Big2 { Big2(Big2&&) = delete; Big2() = default; };

struct Processor
{
  Big1 const& _big1;
  Big2 const& _big2;
  explicit Processor(lvalue_ref<const Big1> b1, lvalue_ref<const Big2> b2) : _big1(b1), _big2(b2) {}
};

static_assert(std::is_constructible<Processor, const Big1&, const Big2&>::value, "test failure");
static_assert(!std::is_constructible<Processor, Big1, const Big2&>::value, "test failure");
static_assert(!std::is_constructible<Processor, const Big1&, const Big2&&>::value, "test failure");
static_assert(std::is_constructible<Processor, std::reference_wrapper<const Big1>&, std::reference_wrapper<const Big2>>::value, "test failure");

void test_lvalue_ref()
{
  const Big1 b1 {}; // just shows usage
  const Big2 b2 {};
  Processor p {b1, b2};
}

int main()
{
  test_lvalue_ref();
}
