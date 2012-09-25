Whole Value Idiom
===================
The whole value idiom[1] supports type-rich programming which, as Bjarne Stroustrup advocates, is an important means to improve software reliability, adding to this: �but most current infrastructure software doesn�t systematically use the techniques I suggest.� [2]

Applying the whole value idiom where you would otherwise be tempted to use built-in types to represent domain values, gains you type checking and expressiveness.

This single-header library provides an easy, zero-overhead way to use the whole value idiom with built-in types in C++.[3]

Example
---------
```C++
#include "whole_value.h"

WV_DEFINE_QUANTITY_TYPE( Integer, int )

WV_DEFINE_TYPE( Year, Integer )
WV_DEFINE_TYPE( Day, Integer )

enum Month
{
    January=1, February, March, April, May, June,
    July, August, September, October, November, December
};

class Date
{
public:
   Date( Year year, Month month, Day day ) {}
   // ...
};

int main()
{
    Date today = Date( Year(2012), July, Day(21) );  // OK
//  today = Date( Day(21), July, Year(2012) );       // compile-time error
//  today = Date( July, Day(21), Year(2012) );       // compile-time error
}

// g++ -Wall -I../../include/ -I%BOOST_INCLUDE% -o wiki-example1 wiki-example1.cpp
// cl -nologo -W3 -EHsc -I../../include/ -I%BOOST_INCLUDE% wiki-example1.cpp
```

Other Libraries
-----------------
If dimensions play a prime role in your computations, e.g. they are primarily physics-oriented, then you may be better off with libraries such as
- [Boost.Units](http://www.boost.org/doc/libs/1_51_0/libs/units/) for zero-overhead dimensional analysis and unit/quantity manipulation and conversion.
- Michael Kenniston's [Quantity Library](http://home.xnet.com/%7Emsk/quantity/quantity.html) ([Rationale](http://home.xnet.com/%7Emsk/quantity/quantity.html), Quantity [folder](http://www.xnet.com/%7Emsk/quantity)).<br>
I plan to release a version of this library that works with VC6 and more recent  compilers.

Usage
-------
This library provides the following class templates that hold a value:
* `whole_value` - just holds a value
* `bits` - provides bitwise operations
* `arithmetic` - provides arithmetic operations
* `quantity` - provides a subset of arithmetic operations
* `safe_bool` - a class template that implements the [Safe Bool idiom](http://www.artima.com/cppsource/safebool.html)

Depending on your needs, you can define value types that do not interact with each other, or types that allow some interaction, for example with types that are convertible to the underlying type. The following examples range from strict to less so.

### Type definitions

```C++
#include "whole_value.h"
WV_DEFINE_VALUE_TYPE(Value, double)
```
This type allows construction (default, initializer, copy) and assignment.

```C++
#include "whole_value.h"
WV_DEFINE_BITS_TYPE(Bits, unsigned int)
```
This type allows construction (default, initializer, copy), assignment, equality comparisons and bitwise operations xor, and, or, shift-left and shift-right.

```C++
#include "whole_value.h"
WV_DEFINE_ARITHMETIC_TYPE(Real, double)
```
This type allows construction (default, initializer, copy), assignment, and all comparison and arithmetic operations.

```C++
#include "whole_value.h"
WV_DEFINE_QUANTITY_TYPE(Quantity, double)
```
This type allows construction (default, initializer, copy), assignment, all comparison operations and a subset of the arithmetic operations. Think of this type as representing a dimension: the available operations do not change the type's dimension. For example, the multiplication `Quantity *= Quantity` is not available (nor is `Quantity * Quantity`).

```C++
#include "whole_value.h"
WV_DEFINE_SAFE_BOOL_TYPE(SafeBool)
```
This creates a boolean type that has no adverse interactions with other types ([Safe Bool idiom](http://www.artima.com/cppsource/safebool.html)).

### Sub-type definitions

```C++
#include "whole_value.h"
WV_DEFINE_TYPE(A, Quantity)
WV_DEFINE_TYPE(B, Quantity)

```
TBD - Quantity is the base type of Types A and B. This allows A and B to be compared, but not to replace one with the other. Passing arguments...

### Interaction with underlying type

To allow interaction with the underlying type or types that are convertible to that, you can define the following preprocessor symbols before inclusion of the header file `whole_value.h`

```C++
// define:
#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE
// or, one or more of:
#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_WHOLE_VALUE
#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_ARITHMETIC
#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY_ALLOW_

#include "whole_value.h"
```

### Output

To make a type defined with `WV_DEFINE...` streamable, define the desired operator with or without io manipulators as follows.

```C++
#include "whole_value.h"
#include <iostream>

WV_DEFINE_QUANTITY_TYPE(Quantity, double)
WV_DEFINE_TYPE(Acceleration, Quantity)
WV_DEFINE_TYPE(Speed, Quantity)

WV_ADD_STREAM_INSERTION_OPERATOR(Quantity)
// or
WV_ADD_STREAM_INSERTION_OPERATOR_IOMANIP(Quantity, std::fixed << std::setprecision(2) )

int main()
{
    Acceleration acc(9.8);
    Speed spd(330);
    std::cout << "acc:" << acc ", spd:" << spd << std::endl;
}
```
To stream sub types, you only need to define a streaming operator for the base class.

### Convenience functions

```C++
template < typename T, typename U >
inline whole_value<T,U> abs( whole_value<T,U> const & x );

template < typename T, typename U >
inline arithmetic<T,U> abs( arithmetic<T,U> const & x );

template < typename T, typename U >
inline quantity<T,U> abs( quantity<T,U> const & x );

template < typename T, typename U >
inline T to_value( whole_value<T,U> const & x );

template < typename T, typename U >
inline long to_integer( whole_value<T,U> const & x );

template < typename T, typename U >
inline double to_real( whole_value<T,U> const & x );
```

Dependencies
-------------------------
[Boost.Operators](http://www.boost.org/doc/libs/1_51_0/libs/utility/operators.htm) can be used to generate part of the logical and arithmetic operators of the `bits`, `arithmetic` and `quantity` class templates in this library. Note however that this is optional. If Boost.Operators is not used, the relevant operators are directly included in the classes in the conventional way.

To use Boost.Operators, `#define WV_USE_BOOST_OPERATORS` before inclusion of header file `whole_value.h`.

```C++
#define WV_USE_BOOST_OPERATORS // optional
#include "whole_value.h"
```

Boost version 1.46.1 is known to work with the compilers mentioned below.

Performance
------------

```
Relative performance

Compiler        Option   double   Real   Derived
------------------------------------------------
GCC 4.5.2         -O2      1       1        1
MS VC6/VS6        -O2      .       .        .
MS VC8/VS2005     -O2      .       .        .
MS VC2010/VS2010  -O2      .       .        .

WV_DEFINE_ARITHMETIC_TYPE(Real, double)
WV_DEFINE_TYPE(Derived, Real)
```

Compilers known to work
-------------------------
* GCC 4.5.2
* MS VC6/VS6 - requires `using namespace wv;` for convenience functions and streaming operators (VC6 lacks [ADL](http://en.wikipedia.org/wiki/Argument-dependent_name_lookup)).
* MS VC8/VS2005
* MS VC2010/VS2010

Ideas for improvement
-----------------------
* Immutable type variants


### References
[1] The whole value idiom was identified by Ward Cunningham[4] and Martin Fowler[5].<br>[2] Bjarne Stroustrup. [Software Development for Infrastructure](http://www2.research.att.com/%7Ebs/Computer-Jan12.pdf). Computer, January 2012.<br>[3] This library is inspired on the presentation by Mark Radford: [Design Experiences in C++, p.23](http://www.twonine.co.uk/articles/GenProgGoesLive.pdf#Page=23). 2005.<br>[4] Ward Cunningham. [The CHECKS Pattern Language of Information Integrity](http://c2.com/ppr/checks.html). C2 Wiki, 1994.<br>[5] Martin Fowler. [Quantity Pattern](http://martinfowler.com/eaaDev/quantity.html). Website.
