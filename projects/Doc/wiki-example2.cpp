#include "whole_value.h"
WV_DEFINE_SAFE_BOOL_TYPE(SafeBool)
WV_DEFINE_SAFE_BOOL_TYPE(SafeBool2)

int main()
{
    SafeBool safebool, safebool_( true );
    SafeBool2 safebool2;

    bool b = safebool;  // Ok
//  int i = safebool;   // compile-time error

    if ( safebool ) {}  // Ok

    if ( safebool == safebool_ ) {}  // Ok
//  if ( safebool == safebool2 ) {}  // compile-time error
}

// g++ -Wall -I../../include/ -I%BOOST_INCLUDE% -o wiki-example2 wiki-example2.cpp && wiki-example2
// cl -nologo -W3 -EHsc -I../../include/ -I%BOOST_INCLUDE% wiki-example2.cpp && wiki-example2
