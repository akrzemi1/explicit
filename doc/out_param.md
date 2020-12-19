# `out_param`

This small class template `out_param` can be used to indicate in the function's signature that a given reference parameter
is an *output parameter*:

```c++
#include <ak_toolbox/out_param.hpp>
using namespace ak_toolkit::xplicit;

void assign(out_param<std::string&> s, // < output parameter
            std::string const& v)
{
    s.get() = v;
}
```

It does not do any processing. The only benefit of using it is that you cannot pass a reference/pointer to a function unless
you explicitly wrap it:

```c++
std::string s;
assign(out(s), "text");
```

This forces the function callers to be explicit about passing output arguments to it.


## Handling pointers

There is also a variant taking a pointer rather than reference:

```c++
#include <ak_toolbox/out_param.hpp>
using namespace ak_toolkit::xplicit;

void assign_ptr(out_param<std::string*> s, // < pointer output parameter
            std::string const& v)
{
    if (s.get())
        *s.get() = v;
}

int main()
{
    std::string s;
    std::string* p = &s;
    assign_ptr(out(p), "text");
}
```
In this case the pointer itself is passed by value. This means you can assign to the object
pointed to by the pointer, but you cannot change the address of the pointer. 
