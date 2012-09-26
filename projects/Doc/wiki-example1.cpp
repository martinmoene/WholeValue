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
