Whole Value Idiom
===================
The whole value idiom was identified by Ward Cunningham[1] and Martin Fowler[2]. This idiom supports type-rich programming which, as Bjarne Stroustrup advocates, is an important means to improve software reliability, adding to this: “but most current infrastructure software doesn’t systematically use the techniques I suggest.” [3]

Applying the whole value idiom where you would otherwise be tempted to use built-in types to represent domain values, gains you type checking and expressiveness.

This single-header library provides an easy way to use the whole value idiom with built-in types in C++.[4]

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
- Michael Kenniston's [Quantity Library](http://home.xnet.com/%7Emsk/quantity/quantity.html) ([Rationale](http://home.xnet.com/%7Emsk/quantity/quantity.html), Quantity [folder](http://www.xnet.com/%7Emsk/quantity)).
I plan to release a version of this library that works with VC6 and more recent  compilers.

Usage
-------
```C++
#define WV_USE_BOOST_OPERATORS // optional
#include "whole_value.h"
TBD
```

Dependencies
-------------------------
[Boost.Operators](http://www.boost.org/doc/libs/1_51_0/libs/utility/operators.htm) can be used to generate part of the logical and arithmetic operators of the `bits` and `quantity` class templates in this library. Note however that this is optional. If Boost.Operators is not used, the relevant operators are directly included in the classes in the conventional way.

To use Boost.Operators, `#define WV_USE_BOOST_OPERATORS` before inclusion of header file `whole_value.h`. Boost version 1.46.1 is known to work with the compilers mentioned below.

Compilers known to work
-------------------------
* GCC 4.5.2
* MSVC6 - requires `using namespace wv;` for streaming operators (VC6 lacks [ADL](http://en.wikipedia.org/wiki/Argument-dependent_name_lookup)).

### References
[1] Ward Cunningham. [The CHECKS Pattern Language of Information Integrity](http://c2.com/ppr/checks.html). C2 Wiki, 1994.<br>[2] Martin Fowler. [Quantity Pattern](http://martinfowler.com/eaaDev/quantity.html). Website.<br>[3] Bjarne Stroustrup. [Software Development for Infrastructure](http://www2.research.att.com/%7Ebs/Computer-Jan12.pdf). Computer, January 2012.<br>[4] This library is inspired on the presentation by Mark Radford: [Design Experiences in C++, p.23](http://www.twonine.co.uk/articles/GenProgGoesLive.pdf#Page=23). 2005.
