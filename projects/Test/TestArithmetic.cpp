/*
 * TestMain.cpp
 *
 * Created by Martin on 16 September 2012.
 * Copyright 2012 Universiteit Leiden. All rights reserved.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * These tests use the Catch test framework by Phil Nash.
 * - https://github.com/philsquared/Catch - MSVC8, 9, 2010, GCC 4.2, LLVM 4.0
 * - https://github.com/martinmoene/Catch - MSVC6 in addition to the above
 */

#include "catch.hpp"

//#define WV_USE_BOOST_OPERATORS
//#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_ARITHMETIC
#include "whole_value.h"

#include <iomanip>

// Define fundamental value types:
WV_DEFINE_ARITHMETIC_TYPE( Real, double )
WV_DEFINE_ARITHMETIC_TYPE( Integer, int )

// Define 'domain' value types:
WV_DEFINE_TYPE( MyReal, Real )
WV_DEFINE_TYPE( MyInt, Integer )
WV_DEFINE_TYPE( MyInt2, Integer )

#ifdef G_WV_COMPILER_IS_MSVC6
    // VC6 has no argument dependent lookup (ADL)
    using namespace wv;
#endif

/* ----------------------------------------------------------------------------
 * arithmetic operations:
 *
 * arithmetic type (dimensionless).
 *
 * -   ++x, x++, --x, x--
 *
 * -   x == x, x != x
 * -   x < x, x <= x,
 * -   x > x, x >= x,
 *
 * - [ T + x, T - x ]
 * - [ x + T, x - T ]
 * -   x + x, x - x
 *
 * - [ x * T, T * x, x / T, x % T ]
 * -   x * x, x / x, x % x
 */

namespace {

void operations()
{
    MyInt x; (void)x;

    ++x;
    --x;
    x++;
    x--;

    x == x;
    x != x;

    x < x;
    x <= x;
    x > x;
    x >= x;

    x += x;
    x -= x;
    x *= x;
    x /= x;
    x %= x;

    x + x;
    x - x;
    x * x;
    x / x;
    x % x;


#ifdef WV_ARITHMETIC_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE
    // involving constants right:

    x == 7;
    x != 7;

    x < 7;
    x <= 7;
    x > 7;
    x >= 7;

    x += 7;
    x -= 7;
    x *= 7;
    x /= 7;
    x %= 7;

    x + 7;
    x - 7;
    x * 7;
    x / 7;
    x % 7;

    // involving constants left:

    7 == x;
    7 != x;

    7 < x;
    7 <= x;
    7 > x;
    7 >= x;

//    7 += x;
//    7 -= x;
//    7 *= x;
//    7 /= x;
//    7 %= x;

    7 + x;
    7 - x;
    7 * x;
    7 / x;
    7 % x;

#endif
}

}

/* ----------------------------------------------------------------------------
 * arithmetic:
 *
 * Please note: the Catch test framework cannot handle arithmetic::operator==(),
 * hence we use REQUIRE( !!( expression-involving-whole-value ) ) where needed.
 */

TEST_CASE( "arithmetic/construction",
           "Quantity construction." )
{
    SECTION( "arithmetic/construction/default",
             "Default constructor creates value with default constructed base type." )
    {
        MyInt c;
        int i = int();
        REQUIRE( i == 0 );
        REQUIRE( c.value() == i );
    }

    SECTION( "arithmetic/construction/initialise",
             "Initializing constructor create whole value with given value." )
    {
        MyInt c( 33 );
        REQUIRE( c.value() == 33 );
    }

    SECTION( "arithmetic/construction/copy-assign",
             "Copy-assignment constructor creates whole value with given value." )
    {
        MyInt a( 33 );
        MyInt b( a  );
        MyInt c = a;
        REQUIRE( a.value() == b.value() );
        REQUIRE( a.value() == c.value() );
    }
}

TEST_CASE( "arithmetic/assignment",
           "Quantity assignment." )
{
    MyInt a( 33 );
    MyInt b( 44 );
    a = b;
    REQUIRE( a.value() == 44 );
    REQUIRE( a.value() == b.value() );
}

TEST_CASE( "arithmetic/comparison",
           "Quantity comparison" )
{
    SECTION( "arithmetic/comparison/eq", "" )
    {
        REQUIRE( !!( MyInt(12) == MyInt(12) ) );
        REQUIRE(  !( MyInt(12) == MyInt(17) ) );
    }

    SECTION( "arithmetic/comparison/ne", "" )
    {
        REQUIRE( !!( MyInt(12) != MyInt(17) ) );
        REQUIRE(  !( MyInt(12) != MyInt(12) ) );
    }

    SECTION( "arithmetic/comparison/lt", "" )
    {
        REQUIRE( !!( MyInt(12) < MyInt(17) ) );
        REQUIRE(  !( MyInt(12) < MyInt(12) ) );
    }

    SECTION( "arithmetic/comparison/le", "" )
    {
        REQUIRE( !!( MyInt(12) <= MyInt(17) ) );
        REQUIRE(  !( MyInt(12) <= MyInt( 9) ) );
    }

    SECTION( "arithmetic/comparison/gt", "" )
    {
        REQUIRE( !!( MyInt(17) > MyInt(12) ) );
        REQUIRE(  !( MyInt(12) > MyInt(12) ) );
    }

    SECTION( "arithmetic/comparison/ge", "" )
    {
        REQUIRE( !!( MyInt(17) >= MyInt(12) ) );
        REQUIRE(  !( MyInt( 9) >= MyInt(12) ) );
    }

}

TEST_CASE( "arithmetic/unit-step",
           "Quantity increment and decrement." )
{
    SECTION( "arithmetic/unit-step/preincrement", "" )
    {
        { MyInt int7(7); REQUIRE( !!( ++int7 == MyInt(8) ) ); }
        { MyInt int7(7); REQUIRE(  !( ++int7 == MyInt(7) ) ); }
    }

    SECTION( "arithmetic/unit-step/postincrement", "" )
    {
        { MyInt int7(7); REQUIRE( !!( int7++ == MyInt(7) ) ); }
        { MyInt int7(7); REQUIRE(  !( int7++ == MyInt(8) ) ); }
    }

    SECTION( "arithmetic/unit-step/predecrement", "" )
    {
        { MyInt int7(7); REQUIRE( !!( --int7 == MyInt(6) ) ); }
        { MyInt int7(7); REQUIRE(  !( --int7 == MyInt(7) ) ); }
    }

    SECTION( "arithmetic/unit-step/postdecrement", "" )
    {
        { MyInt int7(7); REQUIRE( !!( int7-- == MyInt(7) ) ); }
        { MyInt int7(7); REQUIRE(  !( int7-- == MyInt(6) ) ); }
    }
}

TEST_CASE( "arithmetic/arithmetic",
           "Quantity arithmetic." )
{
    SECTION( "arithmetic/arithmetic/add", "" )
    {
        MyInt value7(7);

        REQUIRE( !!( ( value7 += MyInt(5) ) == MyInt(12) ) );

        REQUIRE( !!( MyInt(7) + MyInt(5) == MyInt(12) ) );
        REQUIRE(  !( MyInt(7) + MyInt(7) == MyInt(12) ) );
    }

    SECTION( "arithmetic/arithmetic/subtract", "" )
    {
        MyInt value12(12);

        REQUIRE( !!( ( value12 -= MyInt(5) ) == MyInt(7) ) );

        REQUIRE( !!( MyInt(12) - MyInt(5) == MyInt( 7) ) );
        REQUIRE(  !( MyInt(12) - MyInt(7) == MyInt(12) ) );
    }

    SECTION( "arithmetic/arithmetic/multiply", "" )
    {
#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_ARITHMETIC
        MyInt value7(7);

        REQUIRE( !!( ( value7 *= 5 ) == MyInt(35) ) );

        REQUIRE( !!( MyInt(7) * 5 == MyInt(35) ) );
        REQUIRE(  !( MyInt(7) * 7 == MyInt(35) ) );

        REQUIRE( !!( 5 * MyInt(7) == MyInt(35) ) );
        REQUIRE(  !( 7 * MyInt(7) == MyInt(35) ) );
#endif
    }

    SECTION( "arithmetic/arithmetic/divide", "" )
    {
#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_ARITHMETIC
        MyInt value49(49);

        REQUIRE( !!( ( value49 /= 7 ) == MyInt(7) ) );

        REQUIRE( !!( MyInt(49) / 7 == MyInt(7) ) );
        REQUIRE(  !( MyInt(49) / 8 == MyInt(7) ) );
#endif
    }

    SECTION( "arithmetic/arithmetic/mod", "" )
    {
#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_ARITHMETIC
        MyInt value3(3);

        REQUIRE( !!( ( value3 %= 2 ) == MyInt(1) ) );

        REQUIRE( !!( MyInt(3) % 2 == MyInt(1) ) );
        REQUIRE(  !( MyInt(3) % 3 == MyInt(1) ) );
#endif
    }
}

TEST_CASE( "arithmetic/methods",
           "Quantity methods" )
{
    SECTION( "arithmetic/methods/as", "" )
    {
        MyInt a( 'a' );
#ifdef G_WV_COMPILER_IS_MSVC6
        char chr = a.as( char() );
#else
        char chr = a.as<char>();
#endif
        REQUIRE( a.value() == 'a' );
        REQUIRE( chr == 'a' );
    }
}

TEST_CASE( "arithmetic/functions",
           "Quantity free functions." )
{
    SECTION( "arithmetic/functions/abs", "" )
    {
        REQUIRE( !!( abs( MyInt(+12) ) == MyInt(+12) ) );
        REQUIRE( !!( abs( MyInt(-12) ) == MyInt(+12) ) );

        REQUIRE( Approx( abs( MyReal(+1.23 ) ).value() ) == +1.23 );
        REQUIRE( Approx( abs( MyReal(-1.23 ) ).value() ) == +1.23 );
    }

    SECTION( "arithmetic/functions/to_integer", "" )
    {
        REQUIRE( to_integer( MyInt(+12) ) == +12 );
        REQUIRE( to_integer( MyInt(-12) ) == -12 );
    }

    SECTION( "arithmetic/functions/to_real", "" )
    {
        REQUIRE( Approx( to_real( MyReal(+1.23) ) ) == +1.23 );
        REQUIRE( Approx( to_real( MyReal(-1.23) ) ) == -1.23 );
    }
}

namespace quantity_streaming {

WV_ADD_STREAM_INSERTION_OPERATOR( Real )
WV_ADD_STREAM_INSERTION_OPERATOR( Integer )

TEST_CASE( "arithmetic/streaming",
           "Quantity streaming." )
{
    SECTION( "arithmetic/streaming/integer", "" )
    {
        MyInt c( 321 );
        std::ostringstream os;
        os << c;
        REQUIRE( os.str() == "321" );
    }

    SECTION( "arithmetic/streaming/real", "" )
    {
        MyReal f( 1.23456 );
        std::ostringstream os;
        os << f;
        REQUIRE( os.str() == "1.23456" );
    }
}
} // namespace quantity_streaming

namespace quantity_streaming_iomanip {

WV_ADD_STREAM_INSERTION_OPERATOR_IOMANIP( Real, std::fixed << std::setprecision(2) )

TEST_CASE( "arithmetic/streaming/iomanip",
           "Quantity streaming io manipulators." )
{
    SECTION( "arithmetic/streaming/iomanip/integer", "" )
    {
        MyReal f( 1.23456789e3 );
        std::ostringstream os;
        os << f;
        REQUIRE( os.str() == "1234.57" );
    }
}
} // namespace quantity_streaming_iomanip

/*
 * end of file
 */
