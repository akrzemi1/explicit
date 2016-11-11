# explicit [![Build Status](https://travis-ci.org/akrzemi1/explicit.svg?branch=master)](https://travis-ci.org/akrzemi1/explicit)

A set of small tools that allow you to state your intentions more explicitly in the interfaces.

## Tool `out_param`

A tool for indicating function *output parameters* in call sites:

```c++
void clear(out_param<std::string&> s) // output parameter in function declaration
{
    s.get().clear();
}

std::string s {"text"};
clear(out(s));                        // in function invokation
```

For more, [see here](doc/out_param.md).

## Tool `tagged_bool`

An alternative to type `bool` in function arguments. It allows to associate a name with the boolean type. No nasty implicit converisons from/to `int`, `double`, or pointers. Different instantiations of `tagged_bool` are not interconvertible:

you can declare:

```c++
using EngineStarted = tagged_bool<class EngineStartedTag>; // one boolean type
using CrewReady     = tagged_bool<class CrewReadyTag>;     // another boolean type

void set_status(EngineStarted started, CrewReady ready); // function declaration
set_status(EngineStarted{true}, CrewReady{true});        // function invokation
```
For more, [see here](doc/tagged_bool.md).

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
