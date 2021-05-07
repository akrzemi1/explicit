# `tagged_bool`

This tool is used to avoid naked `bool`s in function parameters.

Normally, if you need two boolean pieces of information to be passed to your function, you could write:

```c++
void run(bool engineStarted, bool crewReady);
```

But this has a number of problems. In the call site, no-one can understand the meaning of the two parameters:

```c++
run(false, true);
```

It is easy to confuse the order of arguments. Also, this works for types other than `bool`:

```c++
run(vec.data(), vec.size()); // a pointer and an int
run(-99.99, +99.99);
```

Using `tagged_bool` avoids all these problems. It allows you to easily build your unique `bool`-like types

```c++
#include <ak_toolbox/tagged_bool.hpp>
namespace xplicit = ak_toolkit::xplicit;

class EngineStartedTag_; // never defined
class CrewReadyTag_;     // never defined

using EngineStarted = xplicit::tagged_bool<EngineStartedTag_>;
using CrewReady     = xplicit::tagged_bool<CrewReadyTag_>;
```

This creates two boolean types: `EngineStarted` and `CrewReady`. They can be used as in boolean expressions
but are not implicitly convertible from one another, or even `bool`, and there is no way to construct them form
an `int` or a `double` or a pointer:

```c++
EngineStarted es1 = true;       // error
EngineStarted es2 {vec.data()}; // error
EngineStarted es  {true};       // ok
CrewReady     cr  {false};      // ok
bool ans = cr && !es;           // ok

CrewReady     cr2 {es}          // ok    (explicit conversion)
CrewReady     cr3 = es;         // error (implicit conversion)
```

Types `EngineStartedTag_` and `CrewReadyTag_` guarantee that different instantiations of `tagged_bool` render different types.
These tags do not have to be complete types.

You declare and use these bool-types as follows:

```c++
void set_status(EngineStarted started, CrewReady ready); // function declaration
set_status(EngineStarted{true}, CrewReady{true});        // function call
```

Note that technically one could declare the tagged bool type and the tag class in a single declaration,
using an [elaborated type specifier](https://en.cppreference.com/w/cpp/language/elaborated_type_specifier).
It is not recommended however, due to the nasty behavior of this feature, which can result in two distinct 
tag types actually become one. 