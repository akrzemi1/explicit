#include "ak_toolkit/lvalue_ref.hpp"
#include "ak_toolkit/only_when.hpp"
#include "ak_toolkit/tagged_bool.hpp"
#include <string>
#include <cassert>

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

# if (defined __GNUC__) && (!defined __clang__) && (__GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ < 8 || __GNUC__ == 4 && __GNUC_MINOR__ == 8 && __GNUC_PATCHLEVEL__ < 1)

void static_test_proxy_ref(){}

# else 

struct ProxyReference
{
  int i;
  operator int&() & { return i; }
  operator const int&() const & { return i; }
  operator const int&() const && = delete;
};

void static_test_proxy_ref()
{
  static_assert(std::is_convertible<ProxyReference&, lvalue_ref<int>>::value, "test failure");
  static_assert(std::is_convertible<ProxyReference const&, lvalue_ref<const int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference&&, lvalue_ref<int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference&&, lvalue_ref<const int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference const&&, lvalue_ref<const int>>::value, "test failure");
}
# endif

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

struct SuperInt
{
  operator int () const { return 1; }
};

struct SuperDouble
{
  operator double () const { return 1.5; }
};

struct NoConv {};


struct Rational
{
  int numerator, denominator;
  Rational(only_int num, only_int den = 0) : numerator(num.get()), denominator(den.get()) {}
};

void static_test_only_int_convertible()
{
  static_assert(std::is_constructible<Rational, int>::value, "failed only_int");
  static_assert(std::is_constructible<Rational, short>::value, "failed only_int");
  static_assert(std::is_constructible<Rational, SuperInt>::value, "failed only_int");
  
  static_assert(!std::is_constructible<Rational, float>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, double>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, long double>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, SuperDouble>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, NoConv>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, unsigned>::value, "failed only_int");
}

void test_only_int()
{
  static_test_only_int_convertible();
}

typedef tagged_bool<class BoolA_tag> BoolA;
typedef tagged_bool<class BoolB_tag> BoolB;

void static_testtaged_bool_convertability()
{
  static_assert(!std::is_convertible<bool, BoolA>::value, "failed taged_bool");
  static_assert(!std::is_convertible<BoolA, bool>::value, "failed taged_bool");
  static_assert( std::is_convertible<BoolA, BoolA>::value, "failed taged_bool");
  
  static_assert( std::is_constructible<bool, BoolA>::value, "failed taged_bool");
  
  static_assert( std::is_constructible<BoolA, bool>::value, "failed taged_bool");
  static_assert( std::is_constructible<BoolA, BoolA>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, BoolB>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, int>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, void*>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, std::nullptr_t>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, double>::value, "failed taged_bool");
}

void demonstrate_tagged_bool()
{
  BoolA a {true};
  BoolB b {false};
  
  assert (a && !b);
  assert (a);
  assert (!b);
  assert (a == a);
  assert (!b == !b);
  assert (a || b);
  if (a) assert (true);
  else   assert (false);  
}

void test_tagged_bool()
{
  static_testtaged_bool_convertability();
  demonstrate_tagged_bool();
}

int main()
{
  test_lvalue_ref();
  test_only_int();
  test_tagged_bool();
}
