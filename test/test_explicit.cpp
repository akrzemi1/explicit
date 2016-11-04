#include "ak_toolkit/lvalue_ref.hpp"
#include "ak_toolkit/only_when.hpp"
#include <string>

using namespace ak_toolkit::xplicit;

// test lvalue_ref.hpp

struct Big1 { std::string data; Big1(Big1&&) = delete; Big1() = default; };
struct Big2 { std::string data; Big2(Big2&&) = delete; Big2() = default; };

struct Processor
{
  Big1 const& _big1;
  Big2 const& _big2;
  explicit Processor(lvalue_ref<const Big1> b1, lvalue_ref<const Big2> b2) : _big1(b1), _big2(b2) {}
  bool process() { return _big1.data == _big2.data; } // this triggers UB on dangling references
};

struct ProxyReference
{
  int i;
  operator int&() & { return i; }
  operator const int&() const &{ return i; }
};

void static_test_proxy_ref()
{
  static_assert(std::is_convertible<ProxyReference&, lvalue_ref<int>>::value, "test failure");
  static_assert(std::is_convertible<ProxyReference const&, lvalue_ref<const int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference, lvalue_ref<int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference, lvalue_ref<const int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference const, lvalue_ref<const int>>::value, "test failure");
}

void static_lvalue_ref_conversions_succeed()
{
  static_assert(std::is_constructible<Processor, const Big1&, const Big2&>::value, "test failure");
  static_assert(std::is_constructible<Processor, std::reference_wrapper<const Big1>&, std::reference_wrapper<const Big2>>::value, "test failure");
}

void static_rvalue_ref_conversions_fail()
{
  static_assert(!std::is_constructible<Processor, Big1, const Big2&>::value, "test failure");
  static_assert(!std::is_constructible<Processor, const Big1&, const Big2&&>::value, "test failure");
}
  
void demonstrate_lvalue_ref()
{
  const Big1 b1 {}; // just shows usage
  const Big2 b2 {};
  Processor p {b1, b2};
  (void)p.process();
}

void test_lvalue_ref()
{
  static_test_proxy_ref();
  static_lvalue_ref_conversions_succeed();
  static_rvalue_ref_conversions_fail();
  demonstrate_lvalue_ref();
}

int main()
{
  test_lvalue_ref();
}
