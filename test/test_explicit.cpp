#include "ak_toolkit/lvalue_ref.hpp"
#include "ak_toolkit/only_when.hpp"
#include "ak_toolkit/tagged_bool.hpp"
#include "ak_toolkit/not_null.hpp"
#include "ak_toolkit/out_param.hpp"
#include "ak_toolkit/namespace_xpl.hpp"
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

struct filesystem_path
{
  filesystem_path(std::string const&) {}
};

template <typename, typename S>
struct is_a_non_string : std::conditional<
  std::is_convertible<S, std::string>::value,
  std::false_type, std::true_type>::type
{};

void static_test_only_when_convertible()
{
  typedef only_when<filesystem_path, is_a_non_string> only_path;

  static_assert(std::is_convertible<filesystem_path, only_path>::value, "test failure");
  static_assert(!std::is_convertible<std::string, only_path>::value, "test failure");
  static_assert(!std::is_convertible<const char*, only_path>::value, "test failure");

  static_assert(std::is_constructible<filesystem_path, std::string>::value, "test failure");
  static_assert(std::is_constructible<filesystem_path, const char *>::value, "test failure");
  static_assert(!std::is_constructible<only_path, std::string>::value, "test failure");
  static_assert(!std::is_constructible<only_path, const char *>::value, "test failure");
}

void test_only_when()
{
  static_test_only_int_convertible();
  static_test_only_when_convertible();
}

typedef xpl::tagged_bool<class BoolA_tag> BoolA; // using short namespace alias xpl
typedef tagged_bool<class BoolB_tag> BoolB;

void static_test_taged_bool_convertability()
{
  static_assert(!std::is_convertible<bool, BoolA>::value, "failed taged_bool");
  static_assert(!std::is_convertible<BoolA, bool>::value, "failed taged_bool");
  static_assert(!std::is_convertible<BoolA, BoolB>::value, "failed taged_bool");
  static_assert( std::is_convertible<BoolA, BoolA>::value, "failed taged_bool");

  static_assert( std::is_constructible<bool, BoolA>::value, "failed taged_bool");

  static_assert( std::is_constructible<BoolA, bool>::value, "failed taged_bool");
  static_assert( std::is_constructible<BoolA, BoolA>::value, "failed taged_bool");
  static_assert( std::is_constructible<BoolA, BoolB>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, int>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, void*>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, std::nullptr_t>::value, "failed taged_bool");
  static_assert(!std::is_constructible<BoolA, double>::value, "failed taged_bool");
}

void demonstrate_tagged_bool()
{
  constexpr BoolA a {true};
  constexpr BoolB b {false};

  static_assert (a && !b, "failed tagged_bool");
  assert (a);
  static_assert (!b, "failed tagged_bool");
  static_assert (a == a, "failed tagged_bool");
  static_assert ((!b) == !b, "failed tagged_bool");
  static_assert (a || b, "failed tagged_bool");

  if (a) assert (true);
  else   assert (false);

  constexpr BoolB ba {a};
  assert (ba);
  static_assert (ba && a, "failed tagged_bool");
  static_assert (ba == BoolB{a}, "failed tagged_bool");
  static_assert (ba != b, "failed tagged_bool");

  constexpr BoolA a1 {false};
  constexpr BoolA a2 = !a1;
  assert (a2);
}

void test_tagged_bool()
{
  static_test_taged_bool_convertability();
  demonstrate_tagged_bool();
}

// -----
int read_value(not_null<std::unique_ptr<int>> p)
{
    return *p;
}

void test_not_null_unique_ptr()
{
    int ans = read_value(as_not_null(std::unique_ptr<int>{new int{2}}));
    assert (2 == ans);
}

void test_not_null()
{
  test_not_null_unique_ptr();
}

// -------------

void assign(out_param<std::string&> obj, std::string const& value)
{
  obj.get() = value;
}

void assign_ptr(out_param<std::string*> obj, std::string const& value)
{
  if (obj.get())
    *obj.get() = value;
}

void test_out_param()
{
  std::string s;
  assign(out(s), "value");
  assert (s == "value");

  std::string z;
  assign_ptr(out(&z), "value");
  assert(z == "value");

  std::string * ptr = 0;
  assign_ptr(out(ptr), "value");
  // no effect, including no UB
}

int main()
{
  test_lvalue_ref();
  test_only_when();
  test_tagged_bool();
  test_not_null();
  test_out_param();
}
