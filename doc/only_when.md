# `only_when`

A tool for defining a type akin to `T`, but which disables certain unwanted conversions to `T`. 

Suppose you want a function that takes a `filesystem::path`
(as in [Boost.Filesystem](http://www.boost.org/doc/libs/1_62_0/libs/filesystem/doc/index.htm)).
But `filesystem::path` converst from `std::string`, and you do not want your functions to be invoked with a `std::string`.

You need to define a type trait, which says when a given type is a string:

```c++
#include <ak_toolbox/only_when.hpp>
using ak_toolkit::xplicit::only_when;

template <typename, typename S>
  using is_a_non_string = std::negation<std::is_convertible<S, std::string>>;
```

Now, you can define a constrained `filesystem::path`:

```c++
using only_path = only_when<filesystem::path, is_a_non_string>;
```

And you can use this more strict type instead of `filesystem::path`:

```c++
void process(only_path p);  // function declaration

filesystem::path p {"file.txt"};
std::string s {"file.txt"};

process(p); // ok
process(s); // error
```
In a similar manner, you can use it to declare a function that takes by referece objects of class `X` but not of class derived from `X`:

```
template <typename T, typename U>
using is_not_aliased_t = 
  std::is_same<std::decay_t<U>, std::decay_t<T>>;

template <typename T>
using nonaliased = only_when<T, is_not_aliased_t>;

struct X {} x;
srtuct Y : X {} y;

void fun(nonaliased<const X&> x);
fun(x); // ok
fun(y); // error
```

## Alias `only_int`

This is an alias for `only_when<int, is_int_convertible_but_no_float>`. You can use it in function declarations to bind to `int` or poxies to `int` but not to `double`:

```c++
#include <ak_toolbox/only_when.hpp>
using ak_toolkit::xplicit::only_int;

struct FixedPoint
{
  // ...
  FixedPoint& operator *= (only_int s);
  FixedPoint& operator /= (only_int s);
};

FixedPoint p;
p *= 2;   // ok
p /= 2;   // ok
p *= 2.0; // error
p /= 2.0; // error
```
