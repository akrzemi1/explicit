# explicit
A set of small tools that allow you to state your intentions more explicitly in the code

## Tool `out_param`

This small class template `out_param` can be used to indicate in the function's signature that a given reference parameter is an *output parameter*:

```c++
#include <ak_toolbox/out_param.hpp>
using namespace ak_toolkit::xplicit;

void assign(out_param<std::string&> s, // < output parameter
            std::string const& v)
{
    s.get() = v;
}
```

It does not do any processing. The only benefit of using it is that you cannot pass a reference to a function unless you explicitly wrap it:

```c++
std::string s;
assign(out(s), "text");
```

This forces the function callers to be explicit about passing output arguments to it.

## Tool `tagged_bool`

This is used to avoid naked `bool`s as function parameters.

Instead of declaration:

```c++
void set_status(bool engineStarted, bool crewReady);
```

you can declare:

```c++
#include <ak_toolbox/taged_bool.hpp>
using namespace ak_toolkit::xplicit;

using EngineStarted = tagged_bool<class EngineStartedTag>;
using CrewReady = tagged_bool<class CrewReadyTag>;

void set_status(EngineStarted started, CrewReady ready);
```

and use it:

```
set_status(EngineStarted{true}, CrewReady{true});
```

These types work with boolean expressions, and avoid common implicit convesion gotchas like a pointer being converted to `bool`.

## Tool `lvalue_ref`

This allows the constructor parameters to bind to lvalues, but not to rvalues:

```c++
class Processor
{
  Big1 const& _big1;
  Big2 const& _big2;
  
public:  
  explicit Processor(lvalue_ref<const Big1> b1, lvalue_ref<const Big2> b2)
    : _big1(b1), _big2(b2) {}
};

int main()
{
  const Big1 b1 {};
  const Big2 b2 {};
  Processor p {b1, b2};          // ok
  Processor q {b1, Big2{}};      // error
  Processor r {b1, std::ref(b2)} // ok
}
```

## installation
It is a C++11 header-only library.

## License
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

## Acknowledgements
The idea of the generalized `only_when` was proposed by Vicente J. Botet Escriba. Tomasz Kamiński suggested the support for `std::reference_wrapper` in `rvalue_ref`.
