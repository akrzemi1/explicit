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

```c++
using EngineStarted = tagged_bool<class EngineStartedTag>; // one boolean type
using CrewReady     = tagged_bool<class CrewReadyTag>;     // another boolean type

void set_status(EngineStarted started, CrewReady ready); // function declaration
set_status(EngineStarted{true}, CrewReady{true});        // function invokation
```
For more, [see here](doc/tagged_bool.md).

## Tool `only_when`

A tool for disabling some unwanted implicit conversions to types present in your function signatures. For instance, you may want your function to take a `filesystem::path` but not allow an implicit conversion from `std::string`:

```c++
using only_path = only_when<filesystem::path, is_a_non_string>; // you define type trait is_a_non_string
void process(only_path p);  // function declaration
process(path);    // ok
process(string);  // error
```

For more, [see here](doc/only_when.md).

## Tool `only_int`

An alternative to type `int` in function arguments. It binds to `int`s and int proxies but not to `double`:

```c++
void scale(only_int i); // declaration
scale(2);     // ok
scale(2.5);   // error
```

For more, [see here](doc/only_when.md).

## Tool `lvalue_ref`

This allows the constructor parameters to bind to lvalues, but not to rvalues:

```c++
struct Processor
{
  Big const& _big;
  explicit Processor(lvalue_ref<const Big> b) : _big(b) {}
};

const Big b {};
Processor p {b}; // ok
Processor q {Big{}}; // error (temporary)
```

For more, [see here](doc/lvalue_ref.md).

## Tool `not_null`

This allows to indicate in function interface that a passed pointer is assumed never to be null:

```c++
void process(not_null<i*> p); // function declaration
int i = 0;
process(&i);                  // error: no implicit conversion to not_null
process(as_not_null(&i));     // ok: explicit adjustment
```

For more, [see here](doc/not_null.md).

## installation
It is a C++11 header-only library.

## License
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

## Acknowledgements
The idea of the generalized `only_when` was proposed by Vicente J. Botet Escriba. Tomasz Kamiński suggested the support for `std::reference_wrapper` in `rvalue_ref`.
