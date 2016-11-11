# `not_null`

This tool allows you to indicate in function interface that a given pointer (raw or smart) is expected never to be null.
There is no runtime-check perform. The goal of this class is to be a distinct type from a raw pointer, and to cause
a compiler error if a raw pointer is accidentally passed to a function which expects a non-null value.

```c++
#include <ak_toolbox/not_null.hpp>
using ak_toolkit::xplicit::not_null;
using ak_toolkit::xplicit::as_not_null;

void process(not_null<int*> p); // function declaration
```

Now, if the caller forgets about the contract, he gets a compile-time error:

```c++
int i = 0;
process(&i); // error: type mismatch
```

You can silence the warning by performing an explicit type adaptation. THis is like 'sining a contract':

```c++
process(as_not_null(&i));
```

The caller can still cheat the system and pass a null pointer through `as_not_null`.
There is no runtime check performed. The system only protects against the inadvertant passage of a null pointer.

This tool also works with smart pointers:

```c++
void fun(not_null<std::unique_ptr<int>> p); // "I exect a non-null, I will delete"

process(std::make_unique<int>(0));              // error: type mismatch
process(as_not_null(std::make_unique<int>(0))); // ok
```
