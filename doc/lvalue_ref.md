# `lvalue_ref`

This allows the constructor parameters to bind to lvalues, but not to rvalues:

```c++
#include <ak_toolbox/lvalue_ref.hpp>
using ak_toolkit::xplicit::lvalue_ref;

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
  Processor q {b1, Big2{}};      // error (temporary)
  Processor r {b1, std::ref(b2)} // ok
}
```
